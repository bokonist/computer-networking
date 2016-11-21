//sliding window client
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#define WINDOW_SIZE 5

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
void displayWindow(int lastAck, int numberOfFrames) //function to display current state of sending
{
    printf("\n");
    int i;
    for (i = 1; i <= numberOfFrames; ++i)
    {
        if(i==lastAck+1 || i== lastAck+WINDOW_SIZE+1)
            printf("|");
        printf("%d",i);
    }
    if(lastAck+WINDOW_SIZE+1 > numberOfFrames)
        printf("|");
    printf("\n");
}

int main()
{
    int sockfd,localsockfd,frames,wl,c=1,x,j,n,p=0,e=0, timeout=5,k;
    struct sockaddr_in server ,localserver;

    sockfd=socket(AF_INET,SOCK_DGRAM,0); //udp connection socket
    die(sockfd,"Error in socket creation");

    localsockfd=socket(AF_INET,SOCK_DGRAM,0); //udp connection socket
    die(localsockfd,"Error in socket creation");

    //set server settings in address struct
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(6500);

    //set local server settings in address struct
    localserver.sin_family=AF_INET;
    localserver.sin_addr.s_addr=inet_addr("127.0.0.1");
    localserver.sin_port=htons(6501);

    k=bind(localsockfd,(struct sockaddr *) &localserver, sizeof(localserver)); //set up local server
    die(k,"Error in binding");

    k=connect(sockfd,(struct sockaddr *) &server, sizeof(server)); //connect to destination
    die(k,"Error in connect()");

    printf("\nUDP Connection Established and local server is running.\n");
    printf("\nEnter the number of Frames: ");
    scanf("%d",&frames);

    int currentFrame=1, lastAcknowledgedFrame=0, i=0, prevAck=0;
    displayWindow(prevAck,frames);

    for(i=0; i< WINDOW_SIZE && currentFrame<=frames; i++,currentFrame++) //one window is sent, starting with currentFrame
    {
        sprintf(a,"%d",currentFrame);
        k=send(sockfd,a,sizeof(a),0); //send the frame number as the message
        die(k,"Error in sending frame");
        printf("Sent frame #%d\n",currentFrame);
    }

    while(lastAcknowledgedFrame<frames) //wait till the last frame is acknowledged
    {
        k=recv(localsockfd,a,sizeof(a),0); //receive an ack
        die(k,"Error in receiving ack");

		printf("Recieved ack for frame #%s\n",a);
        lastAcknowledgedFrame= atoi(a); //update lastAcknowledgedFrame
        displayWindow(lastAcknowledgedFrame,frames);
        if(currentFrame<=frames) //slide the window forward. send another frame if any left.
        {
            sprintf(a,"%d",currentFrame);
            k=send(sockfd,a,sizeof(a),0); //send the frame number as the message
            die(k,"Error in sending frame");
            printf("Sent frame #%d\n",currentFrame++);
        }

    }
    sprintf(a,"%d",0); //send a 0 to signify end of transmission
    k=send(sockfd,a,sizeof(a),0);
    die(k,"Error in sending frame");
    close(localsockfd);
    close(sockfd);
    return 0;
}