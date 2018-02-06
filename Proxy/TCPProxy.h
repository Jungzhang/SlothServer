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
		��������
		@para-1 : ���ջ�����
		@para-2 : ����
		@return : 
			 =0 : ���ӹر� 
			 <0 : ʧ��
			 >0 : �ɹ����յ����ݵĳ��� 
	*/
	int recv(char *buf, int len);
		
	/*
		��������
		@para-1 : ���ͻ�����
		@para-2 : ����
		@return : 
			<=0 : ʧ��
			 >0 : �ɹ����͵����ݳ���  
	*/
	int send(const char *buf, int len);
	
	/*
		��ʼ������ǰ����Ϣ
		@para-1 : ����˵Ķ˿�
		@return : 
			  0 : �ɹ�
			 -1 : ʧ�� 
	*/
	int init(unsigned short port, int addr);
	
public :
	int _sock;     // ���������Ӻ��õ�Socket fd
	struct sockaddr_in _srv_addr;  // �������Ϣ 
};

#endif
