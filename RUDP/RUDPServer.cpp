/*************************************************************************
    > File Name: RUDPServer.cpp
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/

#include <iostream>

#include <string>
#include <cstring>
#include <cstdio>

#include <unistd.h>
#include <netdb.h>

#include "RUDPServer.h"
#include "../Common/InfoPkg.h"
#include "RUDPClient.h"
#include "../Proxy/TCPProxy.h"

int RUDPServer::init(unsigned short port)
{
	UDT::startup();
	struct addrinfo srv_info;
	memset(&srv_info, 0, sizeof(struct addrinfo));
	srv_info.ai_flags = AI_PASSIVE;
	srv_info.ai_family = AF_INET;
	srv_info.ai_socktype = SOCK_STREAM;
	
    std::string port_str = std::to_string(port);
	
	if (getaddrinfo(nullptr, port_str.c_str(), &srv_info, &_srv_info) != 0)
	{
		// TODO: 改成日志输出
		printf("illegal port number or port is busy.\n");
		return -1; 
	}
	
	_srv_handler = UDT::socket(_srv_info->ai_family, _srv_info->ai_socktype, _srv_info->ai_protocol);
	
	if (UDT::ERROR == UDT::bind(_srv_handler, _srv_info->ai_addr, _srv_info->ai_addrlen))
	{
		// TODO: 改成日志输出
		printf("bind : %s\n", UDT::getlasterror().getErrorMessage());
		return -1; 
	}
	
	UDT::listen(_srv_handler, 10);
	
	// TODO: 改成日志输出
	printf("Server is ready at port : %d\n", port);
}

int RUDPServer::accept(int thread_count)
{
	// udt epoll 
	int epid = UDT::epoll_create();
	if (epid == UDT::ERROR)
	{
		// TODO: 改为日志输出
		printf("epoll create failed, %s\n", UDT::getlasterror().getErrorMessage());
		return -1; 
	}
	
	int events = UDT_EPOLL_IN;
	int ret = UDT::epoll_add_usock(epid, _srv_handler, &events);
	
	if (ret == UDT::ERROR)
	{
		// TODO: 改成日志输出
		printf("epoll_add_uscock \"_srv_handler\" failed: %s\n", UDT::getlasterror().getErrorMessage()); 
		return -1;
	}
	
	// 管道和线程 
	int pips[thread_count][2];
	std::vector<std::thread> threads;
	
	// 创建管道和线程并detach线程组 
	ret = createThreadAndPipe(pips, threads, thread_count); 
	if (ret < 0)
	{
		return -1;
	}
	
	int next = 0;
	while(true)
	{
		std::set<UDTSOCKET> readfds;
		ret = UDT::epoll_wait(epid, &readfds, NULL, -1, NULL, NULL);
		
		if(ret == UDT::ERROR)
		{
			// TODO: 改成日志输出
			printf("epoll_wait failed, %s\n", UDT::getlasterror().getErrorMessage());
			break; 
		}
		
		RUDPClient cli;
		cli._cli_handler = UDT::accept(*readfds.begin(), &cli._addr_info, &cli._addr_len);
		
		if (cli._cli_handler == UDT::ERROR)
		{
			// TODO: 改为日志输出
			printf("accept failed, %s\n", UDT::getlasterror().getErrorMessage());
			continue; 
		}
		
		// TODO: del
		std::cout << "accept _cli_handler = " << cli._cli_handler << std::endl;
		
		// 发送给worker线程, 采用round robin算法做负载均衡
		addTask(pips, next, thread_count, cli); 
	}
}

void RUDPServer::worker(int *pip)
{	
	// 创建epoll 
	int epid = UDT::epoll_create();
	
	if (epid == UDT::ERROR)
	{
		// TODO: 改为日志输出 
		printf("epoll_create failed: %s\n", UDT::getlasterror().getErrorMessage());
		exit(-1);
	}
	
	// 加入通知用的管道 
	int events = UDT_EPOLL_IN;
	UDT::epoll_add_ssock(epid, pip[0], &events);
	
	// 两个map, 用来快速查找udtsock和syssock的对应关系(一个udtsock关联着一个syssock)
	std::map<UDTSOCKET, SYSSOCKET> udt2sys;
	std::map<SYSSOCKET, UDTSOCKET> sys2udt; 
	
	while(true)
	{
		std::set<UDTSOCKET> udtfds;
		std::set<SYSSOCKET> sysfds;
		int ret = UDT::epoll_wait(epid, &udtfds, NULL, -1, &sysfds, NULL);
		
		if (ret == UDT::ERROR)
		{
			// TODO: 改为日志输出
			printf("epoll_wait failed, %s\n", UDT::getlasterror().getErrorMessage());
			continue; 
		}
		
		// 检查udtfds 
		for (auto &it : udtfds)
		{
			char buf[10240];
			int len = UDT::recv(it, buf, 10240, 0);
			if (len > 0)
			{
				// 将接收到的数据发送给TCP的应用服务端
				if(::send(udt2sys[it], buf, len, 0) > 0)
				{
					continue;
				}
			}
			// 从epoll监听列表中删除udtsock 
			UDT::epoll_remove_usock(epid, it);
			// 从epoll监听列表中删除syssock
			UDT::epoll_remove_ssock(epid, udt2sys[it]);
			// 关闭
			::close(udt2sys[it]);
			UDT::close(it);
			// 从udt2sys和sys2udt中删除元素
			removeItem(udt2sys[it], it, sys2udt, udt2sys); 
		}
		
		// 检查sysfds
		for (auto &it : sysfds)
		{
			char buf[10240];
			// 管道
			if (it == pip[0])
			{
				RUDPClient cli;
				if (::read(pip[0], &cli, sizeof(cli)) != sizeof(cli))
				{
					continue;
				}
				//超时时间  
    			struct timeval timeout;  
    			timeout.tv_sec = 2; 
    			timeout.tv_usec = 0;
				// 分配的新连接到了
				// 设置超时
				//UDT::setsockopt(cli._cli_handler, 0, UDT_RCVTIMEO, &timeout, sizeof(timeout));
				// 接收起始包
				InfoPkg info;
				int len = UDT::recv(cli._cli_handler, buf, 6, 0);
				// TODO: del
				std::cout << "_cli_handler = " << cli._cli_handler << "  len = " << len <<std::endl;
				if (len != 6)
				{
					// TODO: 写入日志
					printf("recv header package failed, %s\n", UDT::getlasterror().getErrorMessage());
					UDT::close(cli._cli_handler);
					continue;
				}
				info.deserialize(buf, 6);
				// 建立TCPProxy
				TCPProxy proxy;
				if (proxy.init(info._port, info._remoteAddr) < 0)
				{
					// TODO: 写入日志
					printf("build TCPProxy failed\n"); 
					UDT::close(cli._cli_handler);
					continue;
				}
				// 加入两个map中
				udt2sys[cli._cli_handler] = proxy._sock;
				sys2udt[proxy._sock] = cli._cli_handler;
				
				// 加入epoll关心列表
				UDT::epoll_add_usock(epid, cli._cli_handler, &events);
				UDT::epoll_add_ssock(epid, proxy._sock, &events); 
			}
			else
			{
				int len = ::recv(it, buf, 10240, 0);
				// TODO: del
				std::cout << "sys2udt[it] = " << sys2udt[it] << "   len = " << len << std::endl;
				if (len >= 0)
				{
					if (UDT::send(sys2udt[it], buf, len, 0) > 0)
					{
						continue;
					}
					else
					{
						// TODO: 改为日志输出 
						printf("udt send failed, %s\n", UDT::getlasterror().getErrorMessage());
					}
				}
				// 从epoll监听列表中删除udtsock 
				UDT::epoll_remove_usock(epid, sys2udt[it]);
				// 从epoll监听列表中删除syssock
				UDT::epoll_remove_ssock(epid, it);
				// 关闭
				::close(sys2udt[it]);
				UDT::close(it);
				// 从udt2sys和sys2udt中删除元素
				removeItem(it, sys2udt[it], sys2udt, udt2sys);
			}
		}
	}
}

int RUDPServer::createThreadAndPipe(int (*pips)[2], std::vector<std::thread> &threads, int thread_count)
{
	// 创建thread_count个管道
	for (int i = 0; i < thread_count; ++i)
	{
		int ret = pipe(pips[i]);
		if (ret < 0 )
		{
			// TODO: 改成日志输出 
			perror("pipe : ");
			return -1; 
		}
	}
	
	// 创建线程
	for (int i = 0; i < thread_count; ++i)
	{
		threads.push_back(std::move(std::thread(&RUDPServer::worker, this, pips[i])));
	}
	
	// detach thread
	for (auto it = threads.begin(); it != threads.end(); ++it)
	{
		it->detach();
	}
	
	return 0;
}

int RUDPServer::addTask(int (*pips)[2], int &next, const int &thread_count, RUDPClient cli)
{
	//TODO: del
	std::cout << "addTask _cli_handler = " << cli._cli_handler << std::endl;
    int ret = ::write(pips[next][1], &cli, sizeof(cli));
	if ((ret < 0) || (ret != sizeof(cli)))
	{
		// TODO: 改为日志输出 
		perror("write failed");
		return -1;
	}
	++next;
	if (next >= thread_count)
	{
		next = 0;
	}
	return 0;
}


void RUDPServer::removeItem(SYSSOCKET sysfd, UDTSOCKET udtfd, 
	 std::map<SYSSOCKET, UDTSOCKET> &sys2udt, std::map<UDTSOCKET, SYSSOCKET> &udt2sys)
{
	auto it = sys2udt.begin();
	while(it != sys2udt.end())
	{
		if (it->first == sysfd)
		{
			it = sys2udt.erase(it);
			break; 
		}
		else
		{
			++it;
		}
	}
	
	auto tmp = udt2sys.begin();
	while(tmp != udt2sys.end())
	{
		if (tmp->first == udtfd)
		{
			tmp = udt2sys.erase(tmp);
			break;
		}
		else
		{
			++tmp;
		}
	}
} 
