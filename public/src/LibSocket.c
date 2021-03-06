#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "LibSocket.h"
#include "LibLoger.h"

#define MAX_LISTEN_QUE  	10


int StartupTcpSocket(const unsigned int port)
{
    int listenfd;
    struct sockaddr_in servaddr;

    if( (listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
		error("sock error:");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
		error("blind error:");
        return -1;
    }

    if( listen(listenfd, MAX_LISTEN_QUE) == -1)
    {
        error("listen error:");
        return -1;
    }
	
    return listenfd;
}

int StartupUdpSocket(const unsigned int port)
{
    int listenfd;
    struct sockaddr_in servaddr;

    if( (listenfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1 )
    {
		error("sock error:");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
		error("blind error:");
        return -1;
    }

    if( listen(listenfd, MAX_LISTEN_QUE) == -1)
    {
        error("listen error:");
        return -1;
    }
	
    return listenfd;
}


