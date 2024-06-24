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
void ptr_worker(void* arg){
        int fd=*(int*)arg;
        char* buff;
        time_t now = time(NULL);
        buff=ctime(&now);
        send(fd,buff,100,0);
        close(fd);
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
        pthread_t t_worker;
        while(1){
                int new_fd=accept(fd,NULL,NULL);
                pthread_create(&t_worker,NULL,ptr_worker,&new_fd);
        }
}
