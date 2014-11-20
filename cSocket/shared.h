#ifndef SHARED_SOCKET_CRAP
#define SHARED_SOCKET_CRAP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

typedef int SocketDescriptor;
typedef struct sockaddr SocketAddress;
typedef struct sockaddr_in SocketAddressIn;
typedef unsigned long SocketAddressIP;
typedef int SocketPort;
typedef int ErrorCode;

void makeSocketTCP(SocketDescriptor* mSD)
{
	*mSD = socket(AF_INET, SOCK_STREAM, 0);
}

void makeSocketUDP(SocketDescriptor* mSD)
{
	*mSD = socket(AF_INET, SOCK_DGRAM, 0);
}

void makeSocketAddressIP(SocketAddressIn* mSA, const char* mIP, SocketPort mPort)
{
	mSA->sin_family = AF_INET;
	mSA->sin_port = htons(mPort);
	mSA->sin_addr.s_addr = inet_addr(mIP);
}

void makeSocketAddressAny(SocketAddressIn* mSA, SocketPort mPort)
{
	mSA->sin_family = AF_INET;
	mSA->sin_port = htons(mPort);
	mSA->sin_addr.s_addr = htonl(INADDR_ANY);
}

void bindSocket(SocketDescriptor* mSD, const SocketAddressIn* mSA)
{
	bind(*mSD, (const SocketAddress*) mSA, sizeof(SocketAddressIn));
}

ErrorCode connectSocket(SocketDescriptor* mClient, const SocketAddressIn* mSA)
{
	return connect(*mClient, (SocketAddress*) mSA, sizeof(SocketAddressIn));
}

ErrorCode setListener(SocketDescriptor* mSD, size_t mBacklogSize)
{
	return listen(*mSD, mBacklogSize);
}

void tryAcceptClient(SocketDescriptor* mAccepted, SocketDescriptor* mListener)
{
	*mAccepted = accept(*mListener, (SocketAddress*) NULL, NULL);
}

void trySendStr(SocketDescriptor* mSender, char* mStrBuffer)
{
	write(*mSender, mStrBuffer, strlen(mStrBuffer));
}

void tryReceive(SocketDescriptor* mReceiver, char* mBuffer, size_t mBufferSize)
{
	size_t readBytesCount = read(*mReceiver, mBuffer, mBufferSize);

	while(readBytesCount > 0)
	{
		mBuffer[readBytesCount] = 0;
		
		if(fputs(mBuffer, stdout) == EOF)
		{
			printf("\n Error : Fputs error");
		}

		printf("\n");
	}
 
	if(readBytesCount == 0)
	{
		printf("\n Read Error \n");
	}
}

void closeClient(SocketDescriptor* mClient) 
{
	close(*mClient);
}

#endif