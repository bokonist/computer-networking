//gobackn server
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

#define W 5
#define P1 50
#define P2 10

char a[10];
char b[10];
void die(int val, const char* message)
{
    if(val==-1)
    {
        printf("%s\n",message);
        exit(1);
    }
}

int main()
{
    struct sockaddr_in localserver, server;
    int localsockfd,sockfd,i,k,j,c=1,f;

    localsockfd = socket(AF_INET,SOCK_DGRAM,0); //udp connection socket
    die(localsockfd,"Error in socket creation");

    sockfd = socket(AF_INET,SOCK_DGRAM,0); //udp connection socket
    die(sockfd,"Error in socket creation");

    //local server settings to recieve frames
    localserver.sin_family=AF_INET;
    localserver.sin_port=htons(6500);
    localserver.sin_addr.s_addr=inet_addr("127.0.0.1");

    //remote server to send acknowledgements to
    server.sin_family=AF_INET;
    server.sin_port=htons(6501);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    k=bind(localsockfd,(struct sockaddr *) &localserver, sizeof(localserver)); //set up local server to get frames
    die(k,"Error in binding");

    k=connect(sockfd,(struct sockaddr *) &server, sizeof(server)); //connect to destination to send acknowledgements
    die(k,"Error in connect()");

    printf("\nUDP Connection Established and local server is running.\n");
    char choice;

    while(1)
    {
        k=recv(localsockfd,a,sizeof(a),0); //get packet
        die(k,"Error in receiving");
        printf("Send acknowledgement for frame %s? (y/n) ",a);
        scanf("%c", &choice);
        if(choice == 'y')
        {
            printf("Acknowledged frame %s..\n",a);
            k=send(sockfd,a,sizeof(a),0); //send the frame number as the ack
            die(k,"Error in sending ack");
        }
        scanf("%c", &choice); //discard the \n in getchar(). unix bug
    }
    close(sockfd);
    close(localsockfd);
    return 0;
}