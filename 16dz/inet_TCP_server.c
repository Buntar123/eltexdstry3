#include <sys/types.h>
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
	struct sockaddr_in srv,clt;
	socklen_t clt_socklen=sizeof(clt);
	socklen_t srv_socklen=sizeof(srv);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	srv.sin_family=AF_INET;
	srv.sin_port=htons(8080);//!!!
	inet_pton(AF_INET,"127.0.0.1",&srv.sin_addr);//!!!
	if(bind(fd,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("bind");
        }
	if(listen(fd,1)==-1){
                perror("listen");
        }
	int new_fd=accept(fd,(struct sockaddr *)&clt,&clt_socklen);
        if(new_fd==-1){
                perror("accept");
        }
	char buff[100];
        strsp(buff,"Hello!");
        send(new_fd,buff,100,0);
        recv(new_fd,buff,100,0);
        printf("%s\n",buff);
        close(new_fd);
        close(fd);
}
