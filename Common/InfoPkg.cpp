/*************************************************************************
      > File Name: InfoPkg.cpp
      > Author: Raiden  
      > Mail: jungzhang@xiyoulinux.org
      > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/

#include <memory>
#include <cstring>

#include "InfoPkg.h"

std::shared_ptr<char> InfoPkg::serialize()
{
	std::shared_ptr<char> buf(new char[6]);
	
	if (buf == nullptr)
	{
		return nullptr; 
	}
	
	memcpy(buf.get(), &this->_remoteAddr, 4);
	memcpy(buf.get() + 4, &this->_port, 2);
	
	return buf;
}

int InfoPkg::deserialize(const char *data, int len)
{
	// 包不完整
	if (len != 6)
	{
		return -1;
	} 
	
	memcpy(&this->_remoteAddr, data , 4);
	memcpy(&this->_port, data + 4, 2);
	
	return 0;
}

int InfoPkg::size()
{
	return 6;
}

