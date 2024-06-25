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
	struct sockaddr_in clt;
        socklen_t clt_socklen=sizeof(clt);
        int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd==-1){
		perror("socket");
	}
	int flag=1;
	if(setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag))==-1){
		perror("setsockopt");
	}
        clt.sin_family=AF_INET;
        clt.sin_port=htons(10000);
        inet_pton(AF_INET,"255.255.255.255",&clt.sin_addr);
	char buff[100];
	strsp(buff,"hello,im broadcast!");
	if(sendto(fd,buff,100,0,(struct sockaddr*)&clt,clt_socklen)==-1){
		perror("sendto");
	}
	close(fd);
}
