// smtp server

#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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
	char buf[100],domain[20],myEmailAddr[25], sender[25];
	int k,i=0,j,m=0;
	socklen_t len;
	int sockfd;
	struct sockaddr_in server;

	memset(&server,0,sizeof(server));

	sockfd=socket(AF_INET,SOCK_DGRAM,0); //udp socket connection
	die(sockfd, "Error in socket creation");

	//settings for local server to receive mails
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(3500);

	k=bind(sockfd,(struct sockaddr*)&server,sizeof(server)); //server set up.
	die(k,"Error in binding");
	len=sizeof(server);

	//get the 'to' address
	k=recvfrom( sockfd , buf , sizeof(buf) , 0 , (struct sockaddr*)&server , &len);
	die(k,"Error in receiving");

	strcpy(myEmailAddr,buf);
	i=0;
	while(myEmailAddr[i]!='@')
		i++;
	i++;
	while(myEmailAddr[i]!='\0')
		domain[m++]=myEmailAddr[i++]; //extract the domain name
	domain[m-1]='\0';

	printf("Receiving Mail...\n");
	printf("Domain verified << %s >>\n",domain);

	k=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,&len); //get the sender's address
	die(k,"Error in receiving");

	strcpy(sender,buf);
	printf("From: %s\n",sender);

	k=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,&len); //get the subject of the email
	die(k,"Error in receiving");

	printf("Subject : %s\n",buf);
	printf("Message Body: \n\t");

	k=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,&len); //get the body of the email
	die(k,"Error in receiving");
	printf("%s",buf);

	while(strcmp(buf,".\n")!=0) //while not '.'
	{
		k=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&server,&len);
		die(k,"Error in receiving");
		printf("%s",buf);
	}
	printf("\nMail received successfully from %s to %s\n",sender, myEmailAddr);
	close(sockfd);
	exit(0);
	return 0;
}