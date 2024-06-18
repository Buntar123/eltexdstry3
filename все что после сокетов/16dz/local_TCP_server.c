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
	struct sockaddr_un srv,clt;
	int fd=socket(AF_LOCAL,SOCK_STREAM,0);
	srv.sun_family=AF_LOCAL;
	strsp(srv.sun_path,"bingo");
	unlink(srv.sun_path);
	if(bind(fd,(struct sockaddr *)&srv,sizeof(srv))==-1){
		perror("bind");
	}
	if(listen(fd,1)==-1){
                perror("listen");
        }
	socklen_t clt_socklen=sizeof(clt);
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
	unlink(srv.sun_path);
}
