#include "shared.h"

int main(void)
{
    // int sockfd = 0,n = 0;
    SocketDescriptor sockfd;
    SocketAddressIn serv_addr;

    char recvBuff[1024];

    // memset(recvBuff, '0' ,sizeof(recvBuff));

    /*if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
        {
            printf("\n Error : Could not create socket \n");
            return 1;
        }*/
    makeSocketTCP(&sockfd);
    printf("Socket created\n");

    makeSocketAddressIP(&serv_addr, "127.0.0.1", 5200);
    printf("Address created\n");

    /*serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");*/

    /*if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        {
            printf("\n Error : Connect Failed \n");
            return 1;
        }*/

    connectSocket(&sockfd, &serv_addr);
    printf("Socket connected\n");

    tryReceive(&sockfd, recvBuff, sizeof(recvBuff));
    printf("Data received\n");

    close(sockfd);

    return 0;
}