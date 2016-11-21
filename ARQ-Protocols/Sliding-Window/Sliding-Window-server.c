//sliding window server
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
    int prevSeq=0,packets=-1, lastAcknowledgedPacket=0; //for holding the previous sequence number
    k=recv(sockfd,a,sizeof(a),0); //get number of packets to be received
    die(k,"Error in receiving");
    packets=atoi(a);
    printf("Receiving %d packets..\n",packets);
    while(lastAcknowledgedPacket != packets)
    {
        i=WINDOW_SIZE;
        while(i--) //get WINDOW_SIZE number of packets
        {
            k=recv(sockfd,a,sizeof(a),0); //get packet
            die(k,"Error in receiving");

            if(atoi(a)==0) //end of transmission
                goto end;

            if(atoi(a)!=prevSeq+1) //out of sequence packet recieved
            {
                printf("Packet arrived out of order. Sending %d\n", (0-prevSeq)-1);
                sprintf(b, "%d", 0-prevSeq-1); //send negative frame number as negative ack
                k=send(sockfd,b,sizeof(b),0); // send that shit to client
                die(k,"Error in sending ack");
                break; //break out. the client will be requested to retransmit from the next frame of the negatively acked frame
            }
            else // correct sequence
            {
                printf("Acknowledging frame %s..\n",a);
                //in theory, you should actually code for ack loss too. here we're just emulating packet loss.
                lastAcknowledgedPacket = atoi(a);
                prevSeq=lastAcknowledgedPacket;
            }
        }
        printf("Requesting frame #%d\n",lastAcknowledgedPacket+1 );
        sprintf(a,"%d",lastAcknowledgedPacket+1);
        k=send(sockfd,a,sizeof(a),0); //send the frame number of the next wanted frame. i.e first frame of the next window or a lost packet
        die(k,"Error in sending next frame number");
    }
end:sprintf(a,"%d",0);
    k=send(sockfd,a,sizeof(a),0); //signify end of transmission
    die(k,"Error in sending next frame number");

    printf("End of transmission\n");
    close(sockfd);
    close(passive_sockfd);
    return 0;
}