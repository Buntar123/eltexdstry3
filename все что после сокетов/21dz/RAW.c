#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
int main(void){
        int fd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
	if(fd==-1){
                perror("socket");
        }
	int on=1;
        if(setsockopt(fd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))==-1){
                perror("setsockopt");
        }
        struct sockaddr_in srv;
        socklen_t srv_len=sizeof(srv);
        srv.sin_family=AF_INET;
        char payload[]="Im RAW!";
        size_t payload_len=strlen(payload);
	//udp
        uint16_t source=htons(10000);
        uint16_t dest=htons(8080);
        uint16_t len=htons(8+payload_len);
        uint16_t check_sum=0;
	//ip
	uint8_t Ver_IHL=69;
	uint8_t DS=0;
	uint16_t len_ip=htons(28+payload_len);
	uint16_t id=0;
	uint16_t fl_off=0;
	uint8_t TTL=25;
	uint8_t tr_prot=17;
	uint16_t check_sum_ip=0;
	in_addr_t sourceIP = inet_addr("127.0.0.1");
	in_addr_t destIP = inet_addr("127.0.0.1");
        char buff[100];
	memset(buff,0,100);
	memcpy(buff,&Ver_IHL,1);
	memcpy(buff+1,&DS,1);
	memcpy(buff+2,&len_ip,2);
	memcpy(buff+4,&id,2);
	memcpy(buff+6,&fl_off,2);
	memcpy(buff+8,&TTL,1);
	memcpy(buff+9,&tr_prot,1);
	memcpy(buff+10,&check_sum_ip,2);
	memcpy(buff+12,&sourceIP,4);
	memcpy(buff+16,&destIP,4);
	memcpy(buff+20,&source,2);
	memcpy(buff+22,&dest,2);
	memcpy(buff+24,&len,2);
	memcpy(buff+26,&check_sum,2);
	memcpy(buff+26,&check_sum,2);
	memcpy(buff+28,&payload,payload_len);
	if(sendto(fd,buff,100,0,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("sendto");
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
