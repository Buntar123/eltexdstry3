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
struct message{
        char login[20];
        char pass[20];
        struct sockaddr_in ACK_endpoint;
	struct sockaddr_in USR_endpoint;
        char flag;
        char index;
	char us_count;
};
int reliable_send(int basic_fd,int ACK_fd,struct message* mess,int size,struct sockaddr_in* dest){
	int count=0;
	while(count!=3){
		mess->index=count;
		char ACK[4];
		if(sendto(basic_fd,mess,size,0,(struct sockaddr*)dest,sizeof(*dest))==-1){
                        perror("sendto\n");
                        return -1;
                }
		if(recvfrom(ACK_fd,ACK,4,0,NULL,NULL)==-1){
                        count++;
                        continue;
                }
		return 1;
	}
	return -2;
}
int reliable_send2(int basic_fd,int ACK_fd,char* send_buff,int size,struct sockaddr_in* dest){
        int count=0;
        while(count!=3){
                send_buff[73]=count;
                char ACK[4];
                if(sendto(basic_fd,send_buff,size,0,(struct sockaddr*)dest,sizeof(*dest))==-1){
                        perror("sendto\n");
                        return -1;
                }
                if(recvfrom(ACK_fd,ACK,4,0,NULL,NULL)==-1){
                        count++;
                        continue;
                }
                return 1;
        }
        return -2;
}
int ACK_send(int source_fd,struct sockaddr_in* dest){
	char ACK[4];
        ACK[0]=15;
        ACK[1]='A';
        ACK[2]='C';
        ACK[3]='K';
	if(sendto(source_fd,ACK,4,0,(struct sockaddr*)dest,16)==-1){
                perror("ACK_sendto");
                return -1;
        }
        return 1;
}
