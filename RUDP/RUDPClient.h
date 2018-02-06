/*************************************************************************
    > File Name: RUDPClient.h
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/

#ifndef __RUDPCLIENT_H__
#define __RUDPCLIENT_H__

#include <udt/udt.h>

class RUDPClient {

public :
	
	RUDPClient() = default;
	
	RUDPClient(struct sockaddr &addr, UDTSOCKET &cli) : _addr_info(addr), _cli_handler(cli) {};
	
	/*
		读取数据
		@para-1 : 缓存区
		@para-2 : 长度
		@return :
			-1  : 失败
			 0  : 对方关闭连接
		    >0  : 接收到的数据长度 
	*/
	int recv(char *buf, int len);
	
	/*
		发送数据
		@para-1 : 发送数据的缓存区
		@para-2 : 待发送数据长度
		@return : 
			-1  : 失败
			>0  : 成功发送数据的长度 
	*/
	int send(const char *buf, int len);
	
public :
	int _addr_len; 
	struct sockaddr _addr_info;
	UDTSOCKET _cli_handler;
};

#endif
