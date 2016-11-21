//ftp client

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
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
    struct sockaddr_in serveraddr;
    int clientsock,n,rdret,length,k;
    char filename[20],filedata[30000];
    memset(&serveraddr,0,sizeof(serveraddr));

    //server setting to receive file
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port= htons(2000);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    clientsock=socket(AF_INET,SOCK_STREAM,0); //tcp socket connection

    k=connect(clientsock,(struct sockaddr*)&serveraddr,sizeof(serveraddr)); //connect to server
    die(k,"Error in connecting");
    printf("List files on server before requesting?(y/n)\n");
    char choice;
    choice = getchar();
    if(choice == 'y')
    {
        system("ls -l --human-readable");
    }
    scanf("%c",&choice); //discard the \n. unix bug
    printf("Enter the name of the file : "); //request file
    scanf("%s",filename);
    write(clientsock,filename,strlen(filename));

    k=read(clientsock,filedata,30000);
    die(k,"Error in reading");

    printf("Contents of the file requested is\n%s",filedata);
    close(clientsock);
    return 0;    
}