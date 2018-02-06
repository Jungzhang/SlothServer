/*************************************************************************
    > File Name: TCPProxy.cpp
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/
 
#include <cstring>
#include <cstdio>

#include "TCPProxy.h"

int TCPProxy::init(unsigned short port, int addr)
{
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (_sock < 0)
	{
		// TODO: ��Ϊ��־���
		perror("socket : "); 
		return -1;
	}
	
	// ��ʼ����ַ��Ϣ
	memset(&_srv_addr, 0, sizeof(_srv_addr));
	_srv_addr.sin_family = AF_INET;
	_srv_addr.sin_port = htons(port);
	_srv_addr.sin_addr.s_addr = addr;
	
	if (connect(_sock, (struct sockaddr *)&_srv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("connect : ");
		return -1;
	} 
	
	return 0;
}

int TCPProxy::recv(char *buf, int len)
{
	return ::recv(_sock, buf, len, 0);
}

int TCPProxy::send(const char *buf, int len)
{
	return ::send(_sock, buf, len, 0);
}

