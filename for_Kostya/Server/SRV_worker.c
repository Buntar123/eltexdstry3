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
#define USERCOUNT 32
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
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
extern struct data_to_value* elements;
extern struct node* head;
int ACK_send(int source_fd,struct sockaddr_in* dest);
int reliable_send(int basic_fd,int ACK_fd,struct message* mess,int size,struct sockaddr_in* dest);
int reliable_send2(int basic_fd,int ACK_fd,char* send_buff,int size,struct sockaddr_in* dest);
int get(char* key,char* value,struct data_to_value* elements);
void add(char* key,char* value,struct data_to_value* elements);
int checklogin(char* key,struct data_to_value* elements);
int find_node(char* login,struct node* head);
struct node* addnode(struct node* head,struct sockaddr_in client,struct sockaddr_in USR_client,char* login);
void reconfig(int ACK_fd,int time);
uint8_t filling(char* send_buff,struct node* head);
int rewrite(char* login,struct node* head,struct sockaddr_in client,struct sockaddr_in USR_client);
void pth_worker(void* arg){
	struct for_worker* data=(struct for_worker*)arg;
        int new_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(new_fd==-1){
                perror("socket");
		free(data);
                pthread_exit(NULL);
        }
        struct sockaddr_in new_srv;
        new_srv.sin_family=AF_INET;
        new_srv.sin_port=htons(0);
        inet_pton(AF_INET,"127.0.0.1",&new_srv.sin_addr);
        if(bind(new_fd,(struct sockaddr *)&new_srv,sizeof(new_srv))==-1){
                perror("bind");
		close(new_fd);
                free(data);
                pthread_exit(NULL);
        }
	struct message* mess=(struct message*)(data->payload);
	ACK_send(new_fd,(struct sockaddr_in*)&(mess->ACK_endpoint));
	if(mess->index!=0 && mess->flag<18){
		if(find_node(mess->login,head)==1){
			close(new_fd);
                	free(data);
                	pthread_exit(NULL);
		}
	}
	//обработка
	char send_buff[500];
        struct message send_mess;
	switch(mess->flag){
		case 16:
			int get_result=get(mess->login,mess->pass,elements);
			if(get_result==-1){
				send_mess.flag=20;
				send_mess.index=0;
                                //send_mess.ACK_endpoint=new_srv;
				memcpy(&send_mess.ACK_endpoint,&new_srv,16);
			}
			if(get_result==-2){
                                send_mess.flag=21;
                                send_mess.index=0;
                                //send_mess.ACK_endpoint=new_srv;
				memcpy(&send_mess.ACK_endpoint,&new_srv,16);
                        }
			if(get_result==1){
                                send_mess.flag=22;
                                send_mess.index=0;
                                //send_mess.ACK_endpoint=new_srv;
				memcpy(&send_mess.ACK_endpoint,&new_srv,16);
				pthread_mutex_lock(&m1);
				if(find_node(mess->login,head)==1){
					rewrite(mess->login,head,data->client,mess->USR_endpoint);
					printf("rewrite\n");
				}
				else{
					head=addnode(head,data->client,mess->USR_endpoint,mess->login);
					printf("add\n");
				}
				pthread_mutex_unlock(&m1);
				the_world(head);
                        }
			reconfig(new_fd,3);
                        reliable_send(new_fd,new_fd,&send_mess,sizeof(send_mess),&data->client);
                        close(new_fd);
                        free(data);
                        pthread_exit(NULL);
			break;
		case 17:
			printf("reg\n");
			if(checklogin(mess->login,elements)==-1){
                                send_mess.flag=18;
                                send_mess.index=0;
                                //send_mess.ACK_endpoint=new_srv;
				memcpy(&send_mess.ACK_endpoint,&new_srv,16);
                        }
			else{
				send_mess.flag=19;
                        	send_mess.index=0;
                        	//send_mess.ACK_endpoint=new_srv;
				memcpy(&send_mess.ACK_endpoint,&new_srv,16);
				pthread_mutex_lock(&m2);
				add(mess->login,mess->pass,elements);
				head=addnode(head,data->client,mess->USR_endpoint,mess->login);
				pthread_mutex_unlock(&m2);
				the_world(head);
			}
			reconfig(new_fd,3);
                        reliable_send(new_fd,new_fd,&send_mess,sizeof(send_mess),&data->client);
                        close(new_fd);
                        free(data);
                        pthread_exit(NULL);
			break;
		case 23:
			printf("запрос списка онлайн пользователей\n");
			send_mess.flag=23;
			send_mess.index=0;
			pthread_mutex_lock(&m2);
			uint8_t count_user=filling(send_buff+76,head);
			pthread_mutex_unlock(&m2);
			send_mess.us_count=count_user;
			memcpy(&send_mess.ACK_endpoint,&new_srv,16);
			memcpy(send_buff,&send_mess,sizeof(struct message));
			reconfig(new_fd,3);
                        reliable_send2(new_fd,new_fd,send_buff,sizeof(send_buff),&data->client);
                        close(new_fd);
                        free(data);
                        pthread_exit(NULL);
			break;
		case 25:

			break;
	}
	close(new_fd);
        free(data);
}


