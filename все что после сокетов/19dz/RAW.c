#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
void strsp(char* where, char* what){
        int j;
        for(j=0;what[j]!='\0';j++){
                where[j]=what[j];
        }
        where[j]='\0';
}
int main(void){
	int fd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
	if(fd==-1){
		perror("socket");
	}
	int on=1;
	if(setsockopt(fd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))==-1){
		perror("setsockopt");
	}
	char buff[100];
	struct sockaddr_in srv;
	socklen_t srv_len=sizeof(srv);
	while(1){
		int b_read=recvfrom(fd,buff,100,0,(struct sockaddr*)&srv,&srv_len);
		if(b_read==-1){
			perror("recvfrom");
			continue;
		}
		printf("%d\n",b_read);
		//printf("%s\n",buff+28);
		for(int i=28;i<100;i++){
			printf("%c",buff[i]);
		}
	}
}
