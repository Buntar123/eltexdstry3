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
	socklen_t srvlen=sizeof(srv);
	socklen_t cltlen=sizeof(clt);
        int fd=socket(AF_LOCAL,SOCK_DGRAM,0);
        srv.sun_family=AF_LOCAL;
        strsp(srv.sun_path,"bingo");
	clt.sun_family=AF_LOCAL;
	strsp(clt.sun_path,"bingo2");
	unlink(clt.sun_path);
        if(bind(fd,(struct sockaddr *)&clt,sizeof(clt))==-1){
                perror("bind");
        }
	char buff[100];
        strsp(buff,"Hello!");
	if(connect(fd,(struct sockaddr*)&srv,srvlen)==-1){
		perror("connect");
	}
	if(send(fd,buff,100,0)==-1){
		perror("send");
	}
	if(recv(fd,buff,100,0)==-1){
		perror("recv");
	}
        printf("%s\n",buff);
	sleep(1);
	close(fd);
	unlink(clt.sun_path);
}
