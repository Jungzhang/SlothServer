/*************************************************************************
    > File Name: RUDPServer.h
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/
 
#ifndef __RUDPSERVER_H__
#define __RUDPSERVER_H__

#include <vector>
#include <thread>
#include <map>

#include "RUDPClient.h"

class RUDPServer{
public : 
	
	/*
		初始化服务端相关信息
		@para-1 : 需要监听的端口
		@return : 
			  0 : 成功
			 -1 : 失败 
	*/
	int init(unsigned short port);
	
	/*
		主线程,接受来自网卡上的连接请求并将accept的结果发送给worker线程 
		该函数是一个死循环, 但是当其有失败的调用时有返回值
		@para-1 : 创建的线程数量,一般和CPU核数相等 
		@return :
			 -1 : 失败 
	*/
    int accept(int thread_count = std::thread::hardware_concurrency());

private :
	
	/*
		工作线程, 负责处理服务端的业务, 是一个线程函数
		@para-1 : 传入一个int *类型, 是一个匿名管道, 用于主线程向work线程传递accept得到的新连接 
	*/
	void worker(int *pip);
	
	/*
		创建管道和线程
		@para-1 : 保存管道的数组
		@para-2 : 保存线程的数组 
		@return :
			  0 : 成功
			 -1 : 失败 
	*/
	int createThreadAndPipe(int (*pips)[2], std::vector<std::thread> &threads, int thread_count);
	
	/*
		将accept得到的UDTSOCKET发送给worker
		@para-1 : 保存管道的数组
		@para-2 : 下一个线程 
		@para-3 : 总线程数 
		@para-4 : 客户端信息
		@return :
			  0 : 成功
			 -1 : 失败 
	*/
	int addTask(int (*pips)[2], int &next, const int &thread_count, RUDPClient cli);
	
	/*
		分别删除sys2udt和udt2sys中的数据
		@para-1 : syssock fd
		@para-2 : usock fd
		@para-3 : sys2udt map
		@para-4 : udt2sys map 
	*/
	void removeItem(SYSSOCKET sockfd, UDTSOCKET udtfd, 
		 std::map<SYSSOCKET, UDTSOCKET> &sys2udt, std::map<UDTSOCKET, SYSSOCKET> &udt2sys);
	
private :
	struct addrinfo *_srv_info = nullptr;
	UDTSOCKET _srv_handler;
};


#endif 
