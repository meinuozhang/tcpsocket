#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include<signal.h>
#define  _BACK_LOG  5   
int startup(const char *ip,unsigned int port)
{
  int listen_sock=socket(AF_INET,SOCK_STREAM,0);
  if(listen_sock==-1)
  {
      printf("%d: %s\n",errno,strerror(errno));
      exit(1);
  }
 // printf("%d:\n",listen_sock);
 // close(tcp_socket);
  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(port);
  inet_aton(ip,&local.sin_addr);
  if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))==-1)
  {
     printf("%d :%s\n",errno,strerror(errno));
     exit(1);
  }
  if(listen(listen_sock,_BACK_LOG)==-1)
  {
      printf("%d :%s\n",errno,strerror(errno));
      exit(1);
  }
      return listen_sock;
}  
static void wait_child(int signo)
{
	 pid_t exit_id;
	 while((exit_id=waitpid(-1,NULL,WNOHANG))>0)
	 {
		  printf("child exit ,pid is:%d\n",exit_id);
	 }
}
void *thread_run(void *arg)
{
	printf("create a new thread,thread_id is :%u\n",pthread_self());
	int new_client=(int) arg;
	char  buf[1024];
	while(1)
	{
       memset(buf,'\0',sizeof(buf));
	   ssize_t _size=read(new_client,buf,sizeof(buf)-1);
	   if(_size<0)
	   {
		   perror("read\n");
		   break;
	   }
	   else if(_size==0)
       {
		   printf("client release!\n");
		   break;
	   }
	   else 
	   {
		   buf[_size]='\0';
		   printf("client :-> %s",buf);
	   }
	}
	close(new_client);
}
void usage(char *proc)
{
	printf("Usage:%s [IP][PORT]\n",proc);
}


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
		exit(1);
	}
    signal(SIGCHLD,wait_child);
	char *ip=argv[1];
	int port=atoi(argv[2]);
    struct  sockaddr_in client_info;
    socklen_t client_len=0;
	int listen_sock=startup(ip,port);
	while(1)
	{
		int new_client=accept(listen_sock,(struct sockaddr*)&client_info,&client_len);
		if(new_client<0)
		{
			perror("accept");
            continue;
        }
		printf("get a new connect...\n");
#ifdef _FUN1_
	   char buf[1024];
	   printf("usage fun1...\n");	
	while(1)
    {
       memset(buf,'\0',sizeof(buf));
	   ssize_t _size=read(new_client,buf,sizeof(buf)-1);
	   if(_size<0)
	   {
		   perror("read\n");
		   break;
	   }
	   else if(_size==0)
       {
		   printf("client release!\n");
		   break;
	   }
	   else 
	   {
		   buf[_size]='\0';
           printf("client:-> %s",buf);
	   }
      }
#endif
#ifdef _fun2_  
	   printf("many procs,use fun2...\n");
	   pid_t id=fork();
	   if(id<0)
		{
			perror("fork");
		}
		else if(id==0)
	{
			close(listen_sock);
			char buf[1024];
    	while(1)
      {
            memset(buf,'\0',sizeof(buf));
	       ssize_t _size=read(new_client,buf,sizeof(buf)-1);
	       if(_size<0)
	      {
		   perror("read\n");
		   break;
	      }
		   else if(_size==0)
		   {
			  printf("client release!\n");
			  break;
		   }
		   else
		   {
			   buf[_size]='\0';
			   printf("client :-> %s",buf);
		   }
	  }
		close(new_client);
		exit(1);
	}
		close(new_client);
#endif
#ifdef _FUN3_
		printf("thread,use fun3...\n");
		pthread_t t_id;
		pthread_create(&t_id,NULL,thread_run,(void *)new_client);
		pthread_detach(t_id);
#endif
	return 0;
}
}
