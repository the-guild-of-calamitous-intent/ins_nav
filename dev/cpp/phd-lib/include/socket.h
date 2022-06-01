
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netinet/tcp.h>


#ifndef KEVINS_SOCKET_H
#define KEVINS_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

	int initSocket(char *addr, int port);
	int recvSocket(int sd, void *buf, int len, unsigned int flags);
	int initServerSocket(unsigned short port);
	int waitForClient(int servSock);
	
#ifdef __cplusplus
};
#endif

#endif
