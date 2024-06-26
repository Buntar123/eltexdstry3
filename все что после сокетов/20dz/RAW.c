#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
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
        struct sockaddr_in srv,srv2;
        socklen_t srv_len=sizeof(srv);
	socklen_t srv2_len=sizeof(srv2);
	srv.sin_family=AF_INET;
        srv.sin_port=htons(8080);
        inet_pton(AF_INET,"127.0.0.1",&srv.sin_addr);
	char payload[]="Im RAW!";
	size_t payload_len=strlen(payload);
	uint16_t source=htons(10000);
	uint16_t dest=htons(8080);
	uint16_t len=htons(8+payload_len);
	uint16_t check_sum=0;
	char buff[100];
	memset(buff,0,100);
	memcpy(buff,&source,2);
	memcpy(buff+2,&dest,2);
	memcpy(buff+4,&len,2);
	memcpy(buff+6,&check_sum,2);
	memcpy(buff+8,&payload,payload_len);
	if(sendto(fd,buff,100,0,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("sendto");
        }
	int on=1;
        if(setsockopt(fd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))==-1){
                perror("setsockopt");
        }
	while(1){
		char buff2[100];
		memset(buff2,0,100);
		if(recvfrom(fd,buff2,100,0,NULL,NULL)==-1){
			perror("recvfrom");
		}
		uint16_t* source=(uint16_t*)(buff2+20);
		uint16_t port = ntohs(*source);
		if(port==10000){
			printf("%s\n",(buff2+28));
			break;
		}
		else{continue;}
	}
	close(fd);
}
