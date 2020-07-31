#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{
  int fd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(5555);
  inet_aton("106.54.70.78",&addr.sin_addr);
  int r=connect(fd,(const struct sockaddr*)&addr,sizeof(addr));
  if(r==-1) 
    perror("connect"),exit(-1);
  fd_set fst;
  FD_ZERO(&fst);
  int key=0;
  int max=fd>key?fd:key;
  while(1)
  {
  FD_SET(key,&fst);
  FD_SET(fd,&fst);
  int r=select(max+1,&fst,NULL,NULL,NULL);
  if(r==0)
  {
    continue;
  }
  if(FD_ISSET(fd,&fst))
  {
   char buf[1024]={};
   int r=read(fd,buf,1024);
   if(r==0)
   {
     printf("svr close\n");
     break;
   }
   else 
     printf("==> %s\n",buf);
  }
  if(FD_ISSET(key,&fst))
  {
    char buf[1024]={};
   fgets(buf,1024,stdin);
   write(fd,buf,sizeof buf);
  }
  }
  close(fd);
}

