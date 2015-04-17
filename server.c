/*************************************************************************
	> File Name: server.c
	> Author: xiaowei
	> Mail:871343234@qq.com 
	> Created Time: 2015年04月14日 星期二 22时34分32秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>
void handler(int sig)
{
  printf("recvice a sig=%d\n",sig);
  exit(EXIT_SUCCESS);
}
struct packet
{
int len;
char buf[1024];
};
ssize_t readn(int fd,void *buf,size_t count)
{
	size_t nleft =count;
	size_t nread;
	char *bufp = buf;
	while(nleft > 0)
	{
		if((nread = read(fd,bufp,nleft)) <0)
		{	if(errno == EINTR)
				continue;
		return -1;
	    }
		else if(nread ==0 )
			return count - nleft;
		bufp +=nread;
		nleft-=nread;
	}
	return count;
}
ssize_t writen(int fd,const void *buf,size_t count)
{

	size_t nleft =count;
	size_t nwritten;
	char *bufp = buf;
	while(nleft > 0)
	{
		if((nwritten = write(fd,bufp,nleft)) <0)
		{	if(errno == EINTR)
				continue;
		return -1;
	    }
		else if(nwritten ==0 )
			continue;
		bufp += nwritten;
		nleft -= nwritten;
	}
   return count;
}

int main()
{
	int listenfd;
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) <0)
		   //ERR_EXIT("socket");
   {
		printf("listenfd failed\n");
     	exit(EXIT_FAILURE); 
   }
	else
		printf("listenfd sucess\n");
    /*if((listenfd = socket(PF_INET,SOCK_STREAM,0)) < 0) */
		
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =htons(49875);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int on = 1;
	if (setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
	{
		printf("failed");
     	exit(EXIT_FAILURE); 
	}
	//int addr_len = sizeof(servaddr); 
	if(bind(listenfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
		 //ERR_EXIT("bind");
	{
		 printf("bind failed\n");
	     exit(EXIT_FAILURE);
	}
	else
		 printf("bind sucess\n");

	 if(listen(listenfd,SOMAXCONN)<0)
	     //ERR_EXIT("listen");
	{
		printf("listen failed\n");
		exit(EXIT_FAILURE); 
	}
	 else 
		 printf("listen sucess\n");
     struct sockaddr_in peeraddr;
	 int peerlen = sizeof(peeraddr);
	 int conn;
	 pid_t pid;
	 while(1)
	 {
	 if((conn = accept(listenfd,(struct sockaddr *)&peeraddr,&peerlen)) < 0)
	
	 {//ERR_EXIT("accept");
		 printf("accept failed\n");
		 exit(EXIT_FAILURE); 
	 }
	 else
     printf("cliport=%d cliip=%s\n",ntohs(peeraddr.sin_port),inet_ntoa(peeraddr.sin_addr));
    pid =  fork();
	if(pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid == 0)
	{
        
		signal(SIGUSR1,handler);
		//char sendbuf[1024] ={0};
		struct packet sendbuf;
		int n;
		memset(&sendbuf,0,sizeof(sendbuf));
		while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin) !=NULL)
		{
			n = strlen(sendbuf.buf);
			sendbuf.len = htonl(n);
			writen(conn,&sendbuf,4+n);
			memset(&sendbuf,0,sizeof(sendbuf));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		//char recvbuf[1024];
		struct packet recvbuf;
		int n;
		while(1)
		{
			memset(&recvbuf,0,sizeof(recvbuf));
			int len = readn(conn,&recvbuf.len,4);
			if(len == -1)
			{
			perror("read");
			exit(EXIT_FAILURE);
			}
			else if(len < 4)
			{
				printf("peer close %d\n",len);
				break;
			}
            n = ntohl(recvbuf.len);
			len = readn(conn,recvbuf.buf,n);
			if(len == -1)
			{
			perror("read");
			exit(EXIT_FAILURE);
			}
			else if(len < n)
			{
				printf("peer close %d\n",len);
				break;
			}
			fputs(recvbuf.buf,stdout);
		}
	  kill(pid,SIGUSR1);
      exit(EXIT_SUCCESS);
	}
 }
	 return 0;
}
