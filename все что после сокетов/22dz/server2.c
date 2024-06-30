#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <string.h>
void strsp(char* where,char* what){
        int j;
        for(j=0;what[j]!='\0';j++){
                where[j]=what[j];
        }
        where[j]='\0';
}
int main(void){
        struct sockaddr_in srv;
        struct sockaddr_ll clt;
        socklen_t clt_socklen=sizeof(clt);
        socklen_t srv_socklen=sizeof(srv);
        int fd=socket(AF_INET,SOCK_DGRAM,0);
        srv.sin_family=AF_INET;
        srv.sin_port=htons(8080);
        inet_pton(AF_INET,"10.0.2.9",&srv.sin_addr);
        if(bind(fd,(struct sockaddr*)&srv,sizeof(srv))==-1){
                perror("bind");
        }
        char buff[100];
        if(recvfrom(fd,buff,100,0,(struct sockaddr*)&clt,&clt_socklen)==-1){
                perror("recvfrom");
        }
        printf("%s\n",buff);
        strsp(buff,"Hi,RAW!");
        struct sockaddr_in clt2;
        if(sendto(fd,buff,100,0,(struct sockaddr*)&clt,clt_socklen)==-1){
                perror("sendto");
        }
        close(fd);
}

