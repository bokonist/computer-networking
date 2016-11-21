//ftp server

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

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
    struct sockaddr_in clientaddr,server;
    int passive_serversockfd,sockfd,clientsize,n,f,rc,k;
    char filename[100],filedata[30000];
    fflush(stdin);

    passive_serversockfd=socket(AF_INET,SOCK_STREAM,0); //tcp socket connection

    memset(&server,0,sizeof(server));

    //local server settings
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(2000);


    k=bind(passive_serversockfd,(struct sockaddr*)&server,sizeof(server)); //set up server
    die(k,"Error in binding");
    sizeof(server);

    k=listen(passive_serversockfd,5);
    die(k,"Error in listening");
    clientsize=sizeof(clientaddr);

    sockfd=accept(passive_serversockfd,(struct sockaddr*)&clientaddr,&clientsize);

    n=read(sockfd, filename,  100);
/*    read(int fd, void *buf, size_t count);
      read()  attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.
      returns the number of bytes read */

    filename[n]='\0'; //mark end of filename
    printf("The requested file from the client is %s\n",filename);
    if(access(filename,F_OK)) //access return 0 if file exists    
    {
        strcpy(filedata,"File does not exist\n");
        printf("%s\n",filedata);
        n=strlen(filedata);
    }
    else
    {
        //int open(const char *pathname, int flags);
        f=open(filename,O_RDWR); // open in read/write mode
        n=read(f,filedata,30000); //read the data to a buffer
        printf("The contents of the file: \n");
        printf("%s",filedata);
    }
    write(sockfd,filedata,n); //send it to client
    close(sockfd);
    close(passive_serversockfd);
    return 0;
}
