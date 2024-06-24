#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
void strsp(char* where, char* what){
        int j;
        for(j=0;what[j]!='\0';j++){
                where[j]=what[j];
        }
        where[j]='\0';
}
pthread_mutex_t M[20];
void pth_worker(void* arg){
	int* Applic=(int*)arg;
	for(int i=0;i<20;i++){
		if(Applic[i]!=-1){
			if(pthread_mutex_trylock(&M[i])!=-1){
				char* buff;
				time_t now = time(NULL);
                		buff=ctime(&now);
                		if(send(Applic[i],buff,100,0)==-1){
					perror("send");
				}
                		close(Applic[i]);
				Applic[i]=-1;
				pthread_mutex_unlock(&M[i]);
			}
		}
		if(i==19){
			i=-1;
		}
	}
}
int main(void){
	struct sockaddr_un srv;
        int fd=socket(AF_LOCAL,SOCK_STREAM,0);
        srv.sun_family=AF_LOCAL;
        strsp(srv.sun_path,"bingo");
        unlink(srv.sun_path);
        if(bind(fd,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("bind");
        }
        if(listen(fd,20)==-1){
                perror("listen");
        }
	int Applic[20];
	for(int i=0;i<20;i++){
		pthread_mutex_init(&M[i],NULL);
		Applic[i]=-1;
	}
	pthread_t Workers[5];
	for(int i=0;i<5;i++){
		pthread_create(&Workers[i],NULL,pth_worker,Applic);
	}
	while(1){
		int new_fd=accept(fd,NULL,NULL);
		if(new_fd==-1){perror("accept");continue;}
		for(int i=0;i<20;i++){
			if(Applic[i]==-1){
				if(pthread_mutex_trylock(&M[i])!=-1){
					Applic[i]=new_fd;
					pthread_mutex_unlock(&M[i]);
					break;
				}
			}
		}
	}
}
