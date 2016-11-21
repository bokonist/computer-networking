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
    int passive_sockfd,sockfd,i,k,j,c=1,f;

    passive_sockfd = socket(AF_INET,SOCK_STREAM,0); //TCP connection socket
    die(passive_sockfd,"Error in socket creation");

    //server settings
    server.sin_family=AF_INET;
    server.sin_port=htons(6501);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t len;
    len=sizeof(server);

    k=bind(passive_sockfd,(struct sockaddr *) &server, len); //set up local server to get frames
    die(k,"Error in binding");

    k=listen(passive_sockfd,20);
    die(k,"Error in listening");
    

    sockfd=accept(passive_sockfd,(struct sockaddr *)&server, &len);
    die(sockfd,"Error in accepting connection");

    printf("\nTCP Connection Established.\n");
    char choice;

    while(1)
    {
        k=recv(sockfd,a,sizeof(a),0); //get packet
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
    close(passive_sockfd);
    return 0;
}