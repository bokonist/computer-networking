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

    sockfd=socket(AF_INET,SOCK_STREAM,0); //tcp connection socket
    die(sockfd,"Error in socket creation");


    //server settings
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(6501);

    k=connect(sockfd,(struct sockaddr*)&server,sizeof(server)); //initiate connection on a socket
    die(k,"Error in connecting to server");

    printf("\nTCP Connection Established.\n");
    printf("\nEnter the number of Frames: ");
    scanf("%d",&frames);
    sprintf(a,"%d",frames);
    k=send(sockfd,a,sizeof(a),0);//send the number of packets to be sent
    die(k,"Error in sending number of packets");

    int currentFrame=1, lastAcknowledgedFrame=0, i=0, prevAck=0;
    int choice;
    displayWindow(prevAck,frames);

    for(i=0; i< WINDOW_SIZE && currentFrame<=frames; i++,currentFrame++) //one window is sent, starting with currentFrame
    {
        sprintf(a,"%d",currentFrame);
        printf("Send frame #%s? (1/0) ",a);
        scanf("%d",&choice);
        if(choice==1)
        {
            k=send(sockfd,a,sizeof(a),0);//send the frame number as the message
            die(k,"Error in sending frame");
            printf("Sent frame #%d\n",currentFrame);
        }
        //scanf("%d",&choice); //discard \n. unix bug
    }
    while(atoi(a)!=0) //wait till end of transmission signal
    {
        k=recv(sockfd,a,sizeof(a),0); //receive an ack
        die(k,"Error in receiving ack");
        if(atoi(a)<0) //negative ack received
        {
            printf("Negative ack received. Retransmitting from frame #%d\n", (0-atoi(a)));
            currentFrame=(0-atoi(a));
        }
        for(i=0; i< WINDOW_SIZE && currentFrame<=frames; i++,currentFrame++) // transmit the next window
        {
            sprintf(a,"%d",currentFrame);
            printf("Send frame #%s? (1/0) ",a);
            scanf("%d",&choice);
            if(choice==1)
            {
                k=send(sockfd,a,sizeof(a),0);//send the frame number as the message
                die(k,"Error in sending frame");
                printf("Sent frame #%d\n",currentFrame);
            }
            //scanf("%d",&choice); //discard \n. unix bug
        }
    }
    strcpy(a,"0");
    k=send(sockfd,a,sizeof(a),0); //send 0 as frame number to signify end of transmission
    die(k,"Error in sending frame");
    printf("Finished transmission\n");
    close(sockfd);
    return 0;
}