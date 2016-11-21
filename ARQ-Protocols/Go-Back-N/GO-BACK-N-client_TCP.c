//gobackn client
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

    int currentFrame=1, lastAcknowledgedFrame=0, i=0, prevAck=0;
    displayWindow(prevAck,frames);

    for(i=0; i< WINDOW_SIZE && currentFrame<=frames; i++,currentFrame++) //one window is sent, starting with currentFrame
    {
        sprintf(a,"%d",currentFrame);
        k=send(sockfd,a,sizeof(a),0);//send the frame number as the message
        die(k,"Error in sending frame");
        printf("Sent frame #%d\n",currentFrame);
    }

    while(lastAcknowledgedFrame<frames) //wait till the last frame is acknowledged
    {
        k=recv(sockfd,a,sizeof(a),0); //receive an ack
        die(k,"Error in receiving ack");

        printf("Recieved ack for frame #%s\n",a);
        if(atoi(a) != prevAck+1 ) // if it is out of sequence from the previous ack, we know that an ack was lost/not sent
        {
            printf("Acknowledgment lost in transmission. Going back to previous acknowledged frame\n");
            currentFrame=prevAck+1; //go back to the previously acknowledged frame
            displayWindow(prevAck,frames);
            if(currentFrame<=frames) // retransmit a frame from that point
            {
                sprintf(b,"%d",currentFrame);
                k=send(sockfd,a,sizeof(a),0); //send the frame number as the message
                die(k,"Error in sending frame");
                printf("Sent frame #%d\n",currentFrame++);
            }
            displayWindow(prevAck,frames);
            printf("waiting for ack of frame#%d\n",currentFrame-1);
            a="0";
            while(atoi(a)!=currentFrame-1) //discard the future useless acks till the one we retransmitted has been acknowledged
            {
                 k=recv(sockfd,a,sizeof(a),0); //receive an ack
                printf("discarding ack of frame #%s\n",a);
                die(k,"Error in receiving ack");
            }

            prevAck=currentFrame-1; //update prevAck
            displayWindow(prevAck,frames);
            for(i=0; i< WINDOW_SIZE && currentFrame<=frames; i++,currentFrame++) //send a whole window after recovery from ack loss
            {
                sprintf(a,"%d",currentFrame);
                k=send(sockfd,a,sizeof(a),0); //send the frame number as the message
                die(k,"Error in sending frame");
                printf("Sent frame #%d\n",currentFrame);
            }
        }
        else //acks received perfectly in sequence
        {
            prevAck= atoi(a); //update prevAck
            lastAcknowledgedFrame= atoi(a); //update lastAcknowledgedFrame
            displayWindow(prevAck,frames);

            if(currentFrame<=frames) //slide the window forward. send another frame if any left.
            {
                sprintf(a,"%d",currentFrame);
                k=send(sockfd,a,sizeof(a),0); //send the frame number as the message
                die(k,"Error in sending frame");
                printf("Sent frame #%d\n",currentFrame++);
            }
        }
    }
    close(sockfd);
    return 0;
}