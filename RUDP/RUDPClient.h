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
		��ȡ����
		@para-1 : ������
		@para-2 : ����
		@return :
			-1  : ʧ��
			 0  : �Է��ر�����
		    >0  : ���յ������ݳ��� 
	*/
	int recv(char *buf, int len);
	
	/*
		��������
		@para-1 : �������ݵĻ�����
		@para-2 : ���������ݳ���
		@return : 
			-1  : ʧ��
			>0  : �ɹ��������ݵĳ��� 
	*/
	int send(const char *buf, int len);
	
public :
	int _addr_len; 
	struct sockaddr _addr_info;
	UDTSOCKET _cli_handler;
};

#endif
