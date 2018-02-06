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
	包结构：
		4字节：远端应用服务器IP 
		2字节：远端应用服务器端口 
*/

class InfoPkg {

public :

	InfoPkg() = default;

	InfoPkg(int remoteAddr, unsigned int port) : _remoteAddr(remoteAddr), _port(port) { }
	
	// 序列化
	std::shared_ptr<char> serialize();
	
	// 反序列化
	int deserialize(const char *data, int len);
	
	// 长度
	int size(); 
	
public :
	int _remoteAddr;           // 远端应用服务器IP 
	unsigned short _port;       // 远端应用服务器端口 
};


#endif
  
