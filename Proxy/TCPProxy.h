/*************************************************************************
    > File Name: TCPProxy.h
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/
 
#ifndef __TCPPROXY_H__
#define __TCPPROXY_H__

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

class TCPProxy {

public :
	
	/*
		接收数据
		@para-1 : 接收缓存区
		@para-2 : 长度
		@return : 
			 =0 : 连接关闭 
			 <0 : 失败
			 >0 : 成功接收到数据的长度 
	*/
	int recv(char *buf, int len);
		
	/*
		发送数据
		@para-1 : 发送缓存区
		@para-2 : 长度
		@return : 
			<=0 : 失败
			 >0 : 成功发送的数据长度  
	*/
	int send(const char *buf, int len);
	
	/*
		初始化连接前的信息
		@para-1 : 服务端的端口
		@return : 
			  0 : 成功
			 -1 : 失败 
	*/
	int init(unsigned short port, int addr);
	
public :
	int _sock;     // 与服务端连接后获得的Socket fd
	struct sockaddr_in _srv_addr;  // 服务端信息 
};

#endif
