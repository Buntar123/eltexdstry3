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
        clt.sin_family=AF_INET;
        clt.sin_port=htons(10000);
        /*if(inet_pton(AF_INET,INADDR_ANY,&clt.sin_addr)==-1){
		perror("inet_pton");
	}*/
        if(bind(fd,(struct sockaddr *)&clt,sizeof(clt))==-1){
                perror("bind");
        }
	struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr=inet_addr("224.0.0.1");
        mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))==-1){
		perror("setsockopt");
	}
        char buff[100];
        if(recvfrom(fd,buff,100,0,NULL,NULL)==-1){
		perror("recvfrom");
	}
        printf("%s\n",buff);
        close(fd);
}

