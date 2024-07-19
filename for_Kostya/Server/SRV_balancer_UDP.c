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
#define USERCOUNT 32
struct message{
        char login[20];
        char pass[20];
        struct sockaddr_in ACK_endpoint;
	struct sockaddr_in USR_endpoint;
        char flag;
        char index;
	char us_count;
};
struct for_worker{
        char payload[200];
        struct sockaddr_in client;
};
struct data_to_value{
        char key[20];
        char value[20];
};
struct data_to_value* elements;
struct node* head;
void init(){
        elements=malloc(sizeof(struct data_to_value)*USERCOUNT);
        if(elements==NULL){printf("ERROR-malloc\n");exit(1);}
}
void pth_worker(void* arg);
int main(void){
        struct sockaddr_in srv;
        int fd=socket(AF_INET,SOCK_DGRAM,0);
        if(fd==-1){
                perror("socket");
        }
        srv.sin_family=AF_INET;
        srv.sin_port=htons(8080);
        inet_pton(AF_INET,"127.0.0.1",&srv.sin_addr);
        socklen_t srv_socklen=sizeof(srv);
        if(bind(fd,(struct sockaddr *)&srv,sizeof(srv))==-1){
                perror("bind");
        }
        pthread_t t_worker;
        init();
        while(1){
                struct sockaddr_in client;
                socklen_t client_socklen=sizeof(client);
                char buff[200];
                if(recvfrom(fd,buff,200,0,(struct sockaddr*)&client,&client_socklen)==-1){
                        perror("recvfrom");
                }
                struct for_worker* data=(struct for_worker*)malloc(sizeof(struct for_worker));
                if(data==NULL){
                        perror("malloc");
                }
                data->client=client;
                memcpy(data->payload,buff,200);
                pthread_create(&t_worker,NULL,pth_worker,data);
        }
}

