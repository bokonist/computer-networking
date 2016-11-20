//server udp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int sockfd, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int i;

    //create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //configure server settings in address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(7891);
    // htons is to convert host byte order to network byte order. i.e it makes the endianness compatible.
 

    //bind socket with address struct
    bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    //initialize size variable to be used later on
    addr_size = sizeof serverAddr;

    //keep receiving messages and print it
    while(1)
    {
//      size_t   recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr,         socklen_t *addrlen);
        nBytes = recvfrom(sockfd,      buffer,    1024,        0,       (struct sockaddr *)&serverAddr, &addr_size);
        if(strncmp(buffer,"end",3)==0)
        {
            printf("Client ended the session\n");
            break;
        }
        printf("Client: %s",buffer);
    }
    close(sockfd);
    return 0;
}