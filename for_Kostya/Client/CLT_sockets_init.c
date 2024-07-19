#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
struct socks_data{
        struct sockaddr_in basic_client;
        struct sockaddr_in ACK_client;
        struct sockaddr_in balancer;
        struct sockaddr_in USR_client;
        int basic_fd;
        int ACK_fd;
        int USR_fd;
};
void socks_init(struct socks_data* data){
	struct sockaddr_in basic_client,ACK_client,srv,USR_client;
        srv.sin_family=AF_INET;
        srv.sin_port=htons(8080);
        inet_pton(AF_INET,"127.0.0.1",&srv.sin_addr);
        int basic_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(basic_fd==-1){
                perror("socket");
                exit(1);
        }
        int ACK_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(ACK_fd==-1){
                perror("socket");
                exit(1);
        }
        basic_client.sin_family=AF_INET;
        basic_client.sin_port=htons(10000);
	//basic_client.sin_port=htons(0);
        inet_pton(AF_INET,"127.0.0.1",&basic_client.sin_addr);
        socklen_t bc_socklen=sizeof(basic_client);
        if(bind(basic_fd,(struct sockaddr *)&basic_client,bc_socklen)==-1){
                perror("bind");
                exit(1);
        }
        ACK_client.sin_family=AF_INET;
        ACK_client.sin_port=htons(9000);
	//ACK_client.sin_port=htons(0);
        inet_pton(AF_INET,"127.0.0.1",&ACK_client.sin_addr);
        socklen_t Ac_socklen=sizeof(ACK_client);
        if(bind(ACK_fd,(struct sockaddr *)&ACK_client,Ac_socklen)==-1){
                perror("bind");
                exit(1);
        }
        struct timeval tv;
        tv.tv_sec=3;
        tv.tv_usec=0;
        if(setsockopt(ACK_fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv))==-1){
                perror("setsockopt");
                exit(1);
        }
	int USR_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(USR_fd==-1){
                perror("socket");
                exit(1);
        }
        USR_client.sin_family=AF_INET;
        USR_client.sin_port=htons(11000);
	//USR_client.sin_port=htons(0);
        inet_pton(AF_INET,"127.0.0.1",&USR_client.sin_addr);
        socklen_t USR_socklen=sizeof(USR_client);
	if(bind(USR_fd,(struct sockaddr *)&USR_client,USR_socklen)==-1){
                perror("bind");
                exit(1);
        }
	memcpy(&data->balancer,&srv,16);
	memcpy(&data->basic_client,&basic_client,16);
	memcpy(&data->ACK_client,&ACK_client,16);
	memcpy(&data->USR_client,&USR_client,16);
	data->basic_fd=basic_fd;
	data->ACK_fd=ACK_fd;
	data->USR_fd=USR_fd;
}
