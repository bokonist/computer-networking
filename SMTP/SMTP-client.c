//smtp client

#include "sys/socket.h"
#include "netinet/in.h"
#include <arpa/inet.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <unistd.h>

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
	char buf[100];
	int k;
	int sockfd;
	struct sockaddr_in server;

	memset(&server,0,sizeof(server)); 

	sockfd=socket(AF_INET,SOCK_DGRAM,0); //udp socket connection
	die(sockfd,"Error in socket creation");

	//server settings of remote server
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(3500);

	printf("To : ");
	fgets(buf,100,stdin);
	k=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server)); //send recepient address
	die(sockfd,"Error in sending");

	strcpy(buf,"\0");
	printf("From: ");
	fgets(buf,100,stdin);
	k=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server));
	die(sockfd,"Error in sending");

	strcpy(buf,"\0");
	printf("\nSUBJECT : ");
	fgets(buf,100,stdin);
	k=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server));
	die(sockfd,"Error in sending");

	strcpy(buf,"\0");
	printf("Message Body : ");
	while(strcmp(buf,".\n")!=0)
	{
		strcpy(buf,"\0");
		fgets(buf,100,stdin);
		k=sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server));
		die(sockfd,"Error in sending");
	}
	close(sockfd);
	return 0;
}
