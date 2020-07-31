#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<signal.h>
#include<errno.h>
int create_tcp()
{
   int lfd=socket(AF_INET,SOCK_STREAM,0);
   int op=1;
   setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(int));
   struct sockaddr_in addr;
   addr.sin_family=AF_INET;
   addr.sin_port=htons(5555);
   addr.sin_addr.s_addr=0;
   bind(lfd,(const struct sockaddr*)&addr,sizeof addr);
   listen(lfd,SOMAXCONN); 
   return lfd;
}
int main()
{
   int lfd=create_tcp();
   int epo=epoll_create(1);
   struct epoll_event even,evens[1024];
   even.events=EPOLLIN;
   even.data.fd=lfd;
   epoll_ctl(epo,EPOLL_CTL_ADD,lfd,&even);
   int i=0;
   for(;;)
   {
     int r=epoll_wait(epo,evens,1024,-1);
     if(r==-1) continue;
     for(i=0;i<r;i++)
     {
       int cfd=evens[i].data.fd;
       if(cfd==lfd)
       {
         int fd=accept(lfd,NULL,NULL);
         even.data.fd=fd;
         even.events=EPOLLIN;
         epoll_ctl(epo,EPOLL_CTL_ADD,fd,&even);
       }
       else 
       {
         char buf[1024]={};
         int r=read(cfd,buf,sizeof(buf));
         if(r<=0)
         {
           printf("cli close\n");
           epoll_ctl(epo,EPOLL_CTL_DEL,cfd,NULL);
           close(cfd);
         }
         else 
         write(cfd,buf,strlen(buf));
       }
     }
   }
}
