/*************************************************************************
    > File Name: RUDPClient.cpp
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/

#include <udt/udt.h>
#include "RUDPClient.h"

int RUDPClient::recv(char *buf, int len)
{
	return UDT::recv(_cli_handler, buf, len, 0);
}

int RUDPClient::send(const char *buf, int len)
{
	return UDT::send(_cli_handler, buf, len, 0);
} 

