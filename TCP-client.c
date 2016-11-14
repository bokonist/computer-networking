//client tcp/ip
#include <sys/socket.h> //for sockets
#include <netinet/in.h> // networking
#include <arpa/inet.h> //for inet_addr()
#include <sys/types.h> //for type variables
#include <unistd.h> //unix standard
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char buf[100];
	int k;
	int sock_desc;
	struct sockaddr_in client;//holds socket address
	memset(&client,0,sizeof(client)); //set the values to 0

	sock_desc=socket(AF_INET,SOCK_STREAM,0);//get an endpoint for communication. AF_INET is for ipv4. SOCK_STREAM is for TCP (2-way comms). 0 is the protocol
	if(sock_desc==-1)
	{
		printf("Error in socket creation");
		exit(1);
	}

	client.sin_family=AF_INET; //ipv4
	client.sin_addr.s_addr=INADDR_ANY;
	/*	INADDR_ANY is used when you don't need to bind a socket to a specific IP. 
		When you use this value as the address when calling bind(), 
		the socket accepts connections to all the IPs of the machine. */
	client.sin_port=3001;//setting port number
	k=connect(sock_desc,(struct sockaddr*)&client,sizeof(client)); //initiate connection on a socket
	if(k==-1)
	{
		printf("Error in connecting to server");
		exit(1);
	}
	printf("\n\nEnter data to be send :\t");
	gets(buf);
	/* ssize_t send(int sockfd, const void *buf, size_t len, int flags) */
	k=send(sock_desc,buf,100,0);
	if(k==-1)
	{
		printf("Error in sending");
		exit(1);
	}
	close(sock_desc); //close the file descriptor
	exit(0);
	return 0;
}
