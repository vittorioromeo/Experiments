#include "shared.h"

int main()
{
	SocketDescriptor listenfd = 0;
	SocketDescriptor connfd = 0;
	SocketAddressIn serv_addr;

	char sendBuff[1025];
	int numrv;

	makeSocketTCP(&listenfd);
	printf("Socket created\n");

	makeSocketAddressAny(&serv_addr, 5200);
	printf("Address created\n");

	bindSocket(&listenfd, &serv_addr);
	printf("Socket bound\n");

	if(setListener(&listenfd, 10) == -1)
	{
		printf("Failed to listen\n");
		return -1;
	}
	printf("Socket listening\n");

	while(1)
	{
		tryAcceptClient(&connfd, &listenfd);
		printf("Socket accepted client\n");
		
		strcpy(sendBuff, "Message from server");
		trySendStr(&connfd, sendBuff);
		printf("Socket sent message\n");

		closeClient(&connfd);
		printf("Socket closed client\n");

		sleep(1);
	}

	printf("Server shutdown\n");

	return 0;
}