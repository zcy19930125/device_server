#include "stdio.h"
#include "LibSocket.h"
#include "LibEpoll.h"
#include "LibLoger.h"
#include "LibMsgQueue.h"

#define MSGQUEUE_KEY 718
#define MAX_BUFF_LEN 1024
//int buff[MAX_BUFF_LEN];


struct msg_st {
  long int msg_type;
  char buff[MAX_BUFF_LEN];
} msg_buff;


int msgId;

int main(void)
{
	int listenFd;
	EpollSet_t* epollSet;
	int fds;
	int i;
	
	info("starting a listen 7222\n");
	if((listenFd = StartupSocket(7222)) == -1)
		return -1;
	
	info("listen : fd = %d\n", listenFd);
	
	if((epollSet = CreatEpollSet(500)) == NULL)
		return -1;
	
	info("creat epoll ok: fd = %d\n", listenFd);
	
	if(EpollSetAddFd(epollSet,listenFd) == -1)
		return -1;
	
	if((msgId = CreatMessageQueue(MSGQUEUE_KEY)) == -1)
		return -1;
	
	
	while(1)
	{
		fds = EpollSetWait(epollSet);
		if(fds < 0)
		{
			perror("wait error:");
			return -1;
		}
		for(i=0; i<fds; i++)
		{
			if(epollSet->events[i].data.fd == listenFd)
			{
				int sock_fd;
				sock_fd = accept(listenFd, (struct sockaddr*)NULL, NULL);
				if(sock_fd < 0)
				{
					perror("accept error:");
					continue;
				}
			    printf("accept sock:%d\n",sock_fd);	
				EpollSetAddFd(epollSet,sock_fd);
				continue;
			}
			else
			{	
				int bytes;
				bytes = recv(epollSet->events[i].data.fd, msg_buff.buff, MAX_BUFF_LEN, 0);
				if(bytes <= 0)
				{
					EpollSetDeleteFd(epollSet, epollSet->events[i].data.fd);
					close(epollSet->events[i].data.fd);
					continue;
				}
				else
				{
					printf("%s\n",(char*)msg_buff.buff);
					msg_buff.msg_type = 1;
					printf("send %s, type = %ld, len = %ld\n", msg_buff.buff,msg_buff.msg_type,sizeof(struct msg_st));
					MsgQueueSend(msgId,&msg_buff,sizeof(struct msg_st));
				}
			}
			
		}
	}
	
	
	
}


