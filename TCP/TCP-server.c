//server tcp/ip
#include <sys/socket.h> //for sockets
#include <netinet/in.h> // networking
#include <arpa/inet.h> //for inet_addr()
#include <sys/types.h> //for type variables
#include <unistd.h> //unix standard
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	socklen_t len;
	int sock_desc , temp_sock_desc;
	struct sockaddr_in server,client;
	memset(&server,0,sizeof(server)); //set the values to 0
	memset(&client,0,sizeof(client));

	sock_desc=socket(AF_INET,SOCK_STREAM,0); //get an endpoint for communication. AF_INET is for ipv4. SOCK_STREAM is for TCP (2-way comms). 0 is the protocol
	die(sock_desc,"error in socket creation");

	server.sin_family=AF_INET; // ipv4 family
	server.sin_addr.s_addr= inet_addr("127.0.0.1");//ip address
	server.sin_port=3001;//port no. give an arbirtary one.

	k=bind(sock_desc,(struct sockaddr*)&server,sizeof(server)); //bind a name to a socket.
	/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
	die(k,"Error in binding");

	k=listen(sock_desc,20); //listen for comms on a socket. int listen(sockfd, int backlog)
	/*	listen() marks the socket referred to by sockfd as a passive socket, 
		that is, as a socket that will be used to accept incoming connection requests using accept(). backlog is the max no of pending connections*/
	die(k,"error in listening");
	len=sizeof(client);

//	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	temp_sock_desc = accept(sock_desc,(struct sockaddr*)&client, &len); //accept an incoming connection in the listen backlog
	die(temp_sock_desc,"Error in temporary socket creation");
	

	while(1)
	{
//ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		k=recv(temp_sock_desc,buf,100,0); //receive data from sockfd to buf with max size=100
		die(k,"Error in receiving");

		if(strncmp(buf,"end",3)==0)
		{
			printf("Client has ended the chat\n");
			break;
		}
		printf("Client : %s",buf);
		printf("Server [me]: ");
		fgets(buf,100,stdin);
		if(strncmp(buf,"end",3)==0)
		{
			k=send(temp_sock_desc,buf,100,0);
			die(k,"Error in sending terminate signal");
			printf("You ended the chat successfully\n");
			break;
		}
		k=send(temp_sock_desc,buf,100,0);
		die(k,"Error in sending");
	}
	close(temp_sock_desc); //close the temp file descriptor
	exit(0);
	return 0;
}

