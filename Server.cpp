/*************************************************************************
    > File Name: Server.cpp
    > Author: Raiden  
    > Mail: jungzhang@xiyoulinux.org
    > Created Time: Thu Sep 28 00:52:47 2017
 ************************************************************************/
 
#include <cstdlib>
#include <iostream>

#include "RUDP/RUDPServer.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		// TODO: 改为日志输出
		std::cerr << "Usage: " << argv[0] << " " << "port" << std::endl; 
	}
	
	int port = atoi(argv[1]);
	// TODO: 业务逻辑
	RUDPServer server;
	server.init(port);
	server.accept(); 
	
	return EXIT_SUCCESS;
}

