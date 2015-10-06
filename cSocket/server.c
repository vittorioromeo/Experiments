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

    tryAcceptClient(&connfd, &listenfd);
    printf("Socket accepted client\n");

    int dio;

    for(dio = 0; dio < 100; ++dio)
    {


        sprintf(sendBuff, "Message from server %d", dio);
        trySendStr(&connfd, sendBuff);
        printf("Socket sent message\n");


        // sleep(10);
    }

    closeClient(&connfd);
    printf("Socket closed client\n");

    close(listenfd);
    close(connfd);
    printf("Server shutdown\n");

    return 0;
}