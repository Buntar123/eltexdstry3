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
	unlink(srv.sun_path);
	if(bind(fd,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("bind");
        }
	char buff[100];
        if(recvfrom(fd,buff,100,0,(struct sockaddr*)&clt,&cltlen)==-1){
                perror("recvfrom");
        }
        printf("%s\n",buff);
	strsp(buff,"Hi!");
	if(sendto(fd,buff,100,0,(struct sockaddr*)&clt,cltlen)==-1){
                perror("sendto");
        }
	sleep(1);
	close(fd);
	unlink(srv.sun_path);
}
