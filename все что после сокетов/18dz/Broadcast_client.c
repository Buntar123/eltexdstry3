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
        inet_pton(AF_INET,"255.255.255.255",&clt.sin_addr);
	if(bind(fd,(struct sockaddr *)&clt,sizeof(clt))==-1){
                perror("bind");
        }
	char buff[100];
	recvfrom(fd,buff,100,0,NULL,NULL);
	printf("%s\n",buff);
	close(fd);
}
