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
        int ttl=1;
        if(setsockopt(fd,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl))==-1){
		perorr("setsockopt");
	}
        clt.sin_family=AF_INET;
        clt.sin_port=htons(10000);
        inet_pton(AF_INET,"224.0.0.1",&clt.sin_addr);
        char buff[100];
        strsp(buff,"hello,im multicast!");
        if(sendto(fd,buff,100,0,(struct sockaddr*)&clt,clt_socklen)==-1){
		perror("sendto");
	}
        //close(fd);
}
