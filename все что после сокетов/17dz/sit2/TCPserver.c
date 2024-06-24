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
void pth_worker(void* arg){
	int fd=*(int*)arg;
	while(1){
		int new_fd=accept(fd,NULL,NULL);
        	char* buff;
        	time_t now = time(NULL);
        	buff=ctime(&now);
        	send(new_fd,buff,100,0);
        	close(new_fd);
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
        if(listen(fd,10)==-1){
                perror("listen");
        }
	pthread_t WORKERS[10];
	for(int i=0;i<10;i++){
		pthread_create(&WORKERS[i],NULL,pth_worker,&fd);
	}
	pthread_join(WORKERS[0],NULL);
}
