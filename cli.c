/*************************************************************************
	> File Name: cli.c
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
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
void handler(int sig)
{
	printf("recvice a sig = %d\n",sig);
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
	int sock;
	if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <0)
	{//ERR_EXIT("socket");
		   printf("sock failed\n");
		   exit(EXIT_FAILURE);
	}
	else
		printf("sock sucess\n");
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =htons(49875);
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	 if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) <0)
	 {
		printf("connect failed\n");
		exit(EXIT_FAILURE);\
	 }
	 else
		 printf("connect sucess\n");
	 pid_t pid;
	 pid = fork();
	 if (pid == -1)
	 {
	 }
	 if (pid == 0)
	 {
		 //char recvbuf[1024] = {0};
		 struct packet recvbuf;
		 int n;
		 while(1)
		{
          memset(&recvbuf,0,sizeof(recvbuf));
		  int len = readn(sock,&recvbuf.len,4);
		  if(len == -1)
		  {
			  perror("read");
			  exit(EXIT_FAILURE);
		  }
		  if(len < 4)
		  {
			  printf("peer close\n");
		      break;
		  }
         n = ntohl(recvbuf.len);
		 len = readn(sock,recvbuf.buf,n);
		   if(len == -1)
		  {
			  perror("read");
			  exit(EXIT_FAILURE);
		  }
		  if(len < 4)
		  {
			  printf("peer close\n");
		      break;
		  }
			  fputs(recvbuf.buf,stdout);
		}
		close(sock);
		kill(getppid(),SIGUSR1);
        exit(EXIT_SUCCESS);
	 }
	 else
	 {
        signal(SIGUSR1,handler);
        //char sendbuf[1024]={0};
		struct packet sendbuf;
        memset(&sendbuf,0,sizeof(sendbuf));
        int n;
		while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin) != NULL)
		{

			n = strlen(sendbuf.buf);
			sendbuf.len = htonl(n);
			//writen(sock,sendbuf,sizeof(sendbuf));
            writen(sock,&sendbuf,4+n);
			memset(&sendbuf,0,sizeof(sendbuf));
		}
	 }
	 exit(0);
     return 0;
}
