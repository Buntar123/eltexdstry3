#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
void strsp(char* where, char* what){
        int j;
        for(j=0;what[j]!='\0';j++){
                where[j]=what[j];
        }
        where[j]='\0';
}
int main(void){
        struct sockaddr_un srv;
        srv.sun_family=AF_LOCAL;
        strsp(srv.sun_path,"bingo");
        int count=1000;
        while(count!=0){
                int f1=fork();
                if(f1==0){
                        int fd=socket(AF_LOCAL,SOCK_STREAM,0);
                        if(connect(fd,(struct sockaddr*)&srv,sizeof(srv))==-1){
                                perror("connect");
                                exit(1);
                        }
                        char buff[100];
                        recv(fd,buff,100,0);
                        printf("%s",buff);
                        exit(1);
                }
                count--;
        }
}

