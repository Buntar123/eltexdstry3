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
        int fd=socket(AF_LOCAL,SOCK_STREAM,0);
        srv.sun_family=AF_LOCAL;
        strsp(srv.sun_path,"bingo");
        if(connect(fd,(struct sockaddr*)&srv,sizeof(srv))==-1){
                perror("connect");
        }
        char buff[100];
        recv(fd,buff,100,0);
        printf("%s\n",buff);
        close(fd);
}

