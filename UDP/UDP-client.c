//client udp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    int sockfd, nBytes;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    //create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //configure settings in address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    serverAddr.sin_port = htons(7891);
    // htons is to convert host byte order to network byte order. i.e it makes the endianness compatible.

    //initialize size variable to be used later on
    addr_size = sizeof serverAddr;
    printf("Send data to server. Send \"end\" to end session\n");
    while(1)
    {
        printf("Me:  ");
        fgets(buffer,1024,stdin);
        if(strncmp(buffer,"end",3)==0)
        {
            printf("Session ended.\n");
            nBytes = strlen(buffer) + 1;
// ssize_t  sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,   socklen_t addrlen);
            sendto(sockfd,     buffer,          nBytes,      0,        (struct sockaddr *)&serverAddr,     addr_size);
            break;
        }
        nBytes = strlen(buffer) + 1;
        //send message to server
        sendto(sockfd,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
    }
    close(sockfd);
    return 0;
}
