#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
uint16_t ipcheck_sum1(const void* buff,size_t hdr_len){
        const uint16_t* ptr=(const uint16_t*)buff;
        uint32_t sum=0;
        for(int i=0;i<10;i++){
                sum+=*ptr++;
        }
        while(sum>>16){
                sum=(sum & 0xFFFF) + (sum>>16);
        }
        return (uint16_t)~sum;
}
int main(){
	int fd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
        if(fd==-1){
                perror("socket");
        }
	struct sockaddr_ll srv;
	memset(&srv,0,sizeof(srv));
	socklen_t srv_len=sizeof(srv);
	srv.sll_family=AF_PACKET;
	srv.sll_ifindex=if_nametoindex("enp0s8");
	srv.sll_halen=6;
	unsigned char mac[]={0x08,0x00,0x27,0x7a,0x71,0x3b};
	memcpy(srv.sll_addr,mac,6);
	char payload[]="Im RAW!";
        size_t payload_len=strlen(payload);
	char buff[100];
	memset(buff,0,100);
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
        in_addr_t sourceIP = inet_addr("10.0.2.10");
        in_addr_t destIP = inet_addr("10.0.2.9");
	//ethernet
	buff[0]=0x08;
	buff[1]=0x00;
	buff[2]=0x27;
	buff[3]=0x7a;
	buff[4]=0x71;
	buff[5]=0x3b;
	buff[6]=0x08;
	buff[7]=0x00;
	buff[8]=0x27;
	buff[9]=0xc2;
	buff[10]=0x91;
	buff[11]=0xdd;
	buff[12]=0x08;
	buff[13]=0x00;
	memcpy(buff+14,&Ver_IHL,1);
        memcpy(buff+15,&DS,1);
        memcpy(buff+16,&len_ip,2);
        memcpy(buff+18,&id,2);
        memcpy(buff+20,&fl_off,2);
        memcpy(buff+22,&TTL,1);
        memcpy(buff+23,&tr_prot,1);
        memcpy(buff+24,&check_sum_ip,2);
        memcpy(buff+26,&sourceIP,4);
        memcpy(buff+30,&destIP,4);
        memcpy(buff+34,&source,2);
        memcpy(buff+36,&dest,2);
        memcpy(buff+38,&len,2);
        memcpy(buff+40,&check_sum,2);
        memcpy(buff+42,&payload,payload_len);
	uint16_t sum=ipcheck_sum1(buff+14,20);
	memcpy(buff+24,&sum,2);
	if(sendto(fd,buff,100,0,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("sendto");
        }
	while(1){
                char buff2[100];
                memset(buff2,0,100);
                if(recvfrom(fd,buff2,100,0,NULL,NULL)==-1){
                        perror("recvfrom");
                }
                uint16_t* source=(uint16_t*)(buff2+36);
                uint16_t port = ntohs(*source);
                if(port==10000){
                        printf("%s\n",(buff2+42));
                        break;
                }
                else{continue;}
        }
	close(fd);
}
