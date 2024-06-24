#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>
void strsp(char* where, char* what){
        int j;
        for(j=0;what[j]!='\0';j++){
                where[j]=what[j];
        }
        where[j]='\0';
}
int main(void){
	struct sockaddr_un srvTCP;
        int fdTCP=socket(AF_LOCAL,SOCK_STREAM,0);
        srvTCP.sun_family=AF_LOCAL;
        strsp(srvTCP.sun_path,"bingo");
        unlink(srvTCP.sun_path);
        if(bind(fdTCP,(struct sockaddr *)&srvTCP,sizeof(srvTCP))==-1){
                perror("bind");
        }
        if(listen(fdTCP,5)==-1){
                perror("listen");
        }
	//
	struct sockaddr_un srvUDP;
	socklen_t srvlen=sizeof(srvUDP);
        int fdUDP=socket(AF_LOCAL,SOCK_DGRAM,0);
        srvUDP.sun_family=AF_LOCAL;
        strsp(srvUDP.sun_path,"bingo2");
        unlink(srvUDP.sun_path);
        if(bind(fdUDP,(struct sockaddr *)&srvUDP,sizeof(srvUDP))==-1){
                perror("bind");
        }
	//
	int epfd=epoll_create(2);
	struct epoll_event ev, events[2];
	ev.events=EPOLLIN;
	ev.data.fd=fdTCP;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fdTCP,&ev);
	ev.data.fd=fdUDP;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fdUDP,&ev);
	while(1){
		int ep=epoll_wait(epfd,events,2,-1);
		for(int i=0;i<ep;i++){
			if(events[i].data.fd==fdTCP){
				printf("TCP\n");
				int fd=accept(fdTCP,NULL,NULL);
				char* buff;
        			time_t now = time(NULL);
        			buff=ctime(&now);
        			if(send(fd,buff,100,0)==-1){
					perror("send");
				}
        			close(fd);
			}
			else if(events[i].data.fd==fdUDP){
				struct sockaddr_un cltUDP;
        			socklen_t cltlen=sizeof(cltUDP);
				printf("UDP\n");
				char buff[100];
				if(recvfrom(fdUDP,buff,100,0,(struct sockaddr*)&cltUDP,&cltlen)==-1){
                			perror("recvfrom");
        			}
				time_t now = time(NULL);
        			char* buff2=ctime(&now);
				if(sendto(fdUDP,buff2,100,0,(struct sockaddr*)&cltUDP,cltlen)==-1){
                			perror("sendto");
        			}
			}
		}
	}
}
