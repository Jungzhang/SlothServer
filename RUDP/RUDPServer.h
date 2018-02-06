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
		��ʼ������������Ϣ
		@para-1 : ��Ҫ�����Ķ˿�
		@return : 
			  0 : �ɹ�
			 -1 : ʧ�� 
	*/
	int init(unsigned short port);
	
	/*
		���߳�,�������������ϵ��������󲢽�accept�Ľ�����͸�worker�߳� 
		�ú�����һ����ѭ��, ���ǵ�����ʧ�ܵĵ���ʱ�з���ֵ
		@para-1 : �������߳�����,һ���CPU������� 
		@return :
			 -1 : ʧ�� 
	*/
    int accept(int thread_count = std::thread::hardware_concurrency());

private :
	
	/*
		�����߳�, ���������˵�ҵ��, ��һ���̺߳���
		@para-1 : ����һ��int *����, ��һ�������ܵ�, �������߳���work�̴߳���accept�õ��������� 
	*/
	void worker(int *pip);
	
	/*
		�����ܵ����߳�
		@para-1 : ����ܵ�������
		@para-2 : �����̵߳����� 
		@return :
			  0 : �ɹ�
			 -1 : ʧ�� 
	*/
	int createThreadAndPipe(int (*pips)[2], std::vector<std::thread> &threads, int thread_count);
	
	/*
		��accept�õ���UDTSOCKET���͸�worker
		@para-1 : ����ܵ�������
		@para-2 : ��һ���߳� 
		@para-3 : ���߳��� 
		@para-4 : �ͻ�����Ϣ
		@return :
			  0 : �ɹ�
			 -1 : ʧ�� 
	*/
	int addTask(int (*pips)[2], int &next, const int &thread_count, RUDPClient cli);
	
	/*
		�ֱ�ɾ��sys2udt��udt2sys�е�����
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
