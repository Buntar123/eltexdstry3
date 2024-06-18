#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
	struct sockaddr_in srv;
        socklen_t srv_socklen=sizeof(srv);
        int fd=socket(AF_INET,SOCK_STREAM,0);
	srv.sin_family=AF_INET;
        srv.sin_port=htons(8080);//!!!
        inet_pton(AF_INET,"127.0.0.1",&srv.sin_addr);//!!!
	if(connect(fd,(struct sockaddr*)&srv,sizeof(srv))==-1){
                perror("connect");
        }
	char buff[100];
        recv(fd,buff,100,0);
        printf("%s\n",buff);
        buff[0]='H';
        buff[1]='i';
        buff[2]='!';
        buff[3]='\0';
        send(fd,buff,100,0);
        sleep(2);
        close(fd);
}
