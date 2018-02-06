/*************************************************************************
      > File Name: InfoPkg.h
      > Author: Raiden  
      > Mail: jungzhang@xiyoulinux.org
      > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/

#ifndef __INFOPKG_H__
#define __INFOPKG_H__

#include <memory>

/*
	���ṹ��
		4�ֽڣ�Զ��Ӧ�÷�����IP 
		2�ֽڣ�Զ��Ӧ�÷������˿� 
*/

class InfoPkg {

public :

	InfoPkg() = default;

	InfoPkg(int remoteAddr, unsigned int port) : _remoteAddr(remoteAddr), _port(port) { }
	
	// ���л�
	std::shared_ptr<char> serialize();
	
	// �����л�
	int deserialize(const char *data, int len);
	
	// ����
	int size(); 
	
public :
	int _remoteAddr;           // Զ��Ӧ�÷�����IP 
	unsigned short _port;       // Զ��Ӧ�÷������˿� 
};


#endif
  
