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
struct message{
        char login[20];
        char pass[20];
        struct sockaddr_in ACK_endpoint;
	struct sockaddr_in USR_endpoint;
        char flag;
        char index;
	char us_count;
};
char my_login[20];
struct socks_data* ptr_s_data;
void socks_init(struct socks_data* data);
int reliable_send(int basic_fd,int ACK_fd,struct message* mess,int size,struct sockaddr_in* dest);
int ACK_send(int source_fd,struct sockaddr_in* dest);
void first_menu(struct message* send_mess,struct sockaddr_in* ACK_client);
void reconfig(int ACK_fd,int time);//нужна обработка ошибки
void dest_login_input(char* login);
int main(void){
	//инициализация сокетов
	struct socks_data s_data;
	socks_init(&s_data);
	ptr_s_data=&s_data;
	//алгоритм отправки и получения сообщений первого меню
	while(1){
		struct message send_mess;
		first_menu(&send_mess,&ptr_s_data->ACK_client);
		int res_reliable_send=reliable_send(ptr_s_data->basic_fd,ptr_s_data->ACK_fd,&send_mess,sizeof(send_mess),&ptr_s_data->balancer);
		if(res_reliable_send==-1){
			printf("отправка не удалась\n");
			continue;
		}
		if(res_reliable_send==-2){
                        printf("надежная отправка не удалась\n");
                }
		if(res_reliable_send==1){
                        printf("надежная отправка удалась\n");
                }
		reconfig(ptr_s_data->basic_fd,10);
		struct message rec_mess;
		struct sockaddr_in new_srv;
		socklen_t new_srv_len=sizeof(new_srv);
		if(recvfrom(ptr_s_data->basic_fd,&rec_mess,200,0,(struct sockaddr*)&new_srv,&new_srv_len)==-1){
			if(res_reliable_send==-2){
				printf("либо сервер не получил сообщение,либо ответ не пришел\n");
				printf("рекомендуется повторить попытку\n");
				//очистка буфера сокета
				char trash[2];
        			recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
				continue;
			}
			if(res_reliable_send==1){
                                printf("сервер получил сообщение,но его ответ не пришел\n");
				printf("рекомендуется повторить попытку\n");
				//очистка буфера сокета
				char trash[2];
        			recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
                                continue;
                        }
		}
		else{
			//обработка
			if(ACK_send(ptr_s_data->basic_fd,&new_srv)==-1){
				printf("ACK_sendto\n");
			}
			int proc_result;
			switch(rec_mess.flag){
				case 18:
					printf("Ник не уникален\n");
					proc_result=0;
					break;
				case 19:
					printf("Регистрация прошла успешно\n");
					proc_result=1;
					break;
				case 20:
					printf("Пароль не совпал\n");
					proc_result=0;
					break;
				case 21:
					printf("Такого логина нет\n");
					proc_result=0;
					break;
				case 22:
					printf("Авторизация прошла успешно\n");
					proc_result=1;
					break;
			}
			if(proc_result==0){
				continue;
			}
			if(proc_result==1){
				reconfig(ptr_s_data->basic_fd,0);
                                break;
                        }
		}
		
	}
	//Второе меню
	while(1){
		printf("******************************\n");
        	printf("*         ChatApp!!          *\n");
        	printf("******************************\n");
        	printf("* 1.Кому я могу написать?    *\n");
		printf("* 2.Создать личную переписку *\n");
		printf("* 3.Показать мои чаты        *\n");
        	printf("******************************\n");
		char flag_ch=getchar();
        	clean_stdin_buff();
        	struct message send_mess;
		if(flag_ch=='1'){
			send_mess.flag=23;
                	send_mess.index=0;
                	memcpy(&send_mess.ACK_endpoint,&ptr_s_data->ACK_client,16);
			int res_reliable_send=reliable_send(ptr_s_data->basic_fd,ptr_s_data->ACK_fd,&send_mess,sizeof(send_mess),&ptr_s_data->balancer);
			if(res_reliable_send==-1){
                        	printf("отправка не удалась\n");
                        	continue;
                	}
                	if(res_reliable_send==-2){
                        	printf("надежная отправка не удалась\n");
                	}
                	if(res_reliable_send==1){
                        	printf("надежная отправка удалась\n");
                	}
			reconfig(ptr_s_data->basic_fd,10);
        		struct message* rec_mess;
        		struct sockaddr_in new_srv;
        		char rec_buff[500];
        		socklen_t new_srv_len=sizeof(new_srv);
			if(recvfrom(ptr_s_data->basic_fd,rec_buff,500,0,(struct sockaddr*)&new_srv,&new_srv_len)==-1){
                        	if(res_reliable_send==-2){
                                	printf("либо сервер не получил сообщение,либо ответ не пришел\n");
                                	printf("рекомендуется повторить попытку\n");
                                	//очистка буфера сокета
                                	char trash[2];
                                	recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
                                	continue;
                                	}
                        	if(res_reliable_send==1){
                                	printf("сервер получил сообщение,но его ответ не пришел\n");
                                	printf("рекомендуется повторить попытку\n");
                                	//очистка буфера сокета
                                	char trash[2];
                                	recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
                                	continue;
                        	}
                	}
			else{
                        	//обработка
                        	if(ACK_send(ptr_s_data->basic_fd,&new_srv)==-1){
                                	printf("ACK_sendto\n");
                        	}
                        	rec_mess=(struct message*)(rec_buff);
                        	int count=rec_mess->us_count;
                        	int count2=0;
                        	while(count2!=count){
                                	char qwe[20];
                                	memcpy(qwe,rec_buff+76+count2*20,20);
                                	printf("%s\n",qwe);
                                	count2++;
                        	}
        		}
		}
		if(flag_ch=='2'){
			send_mess.flag=23;
                        send_mess.index=0;
                        memcpy(&send_mess.ACK_endpoint,&ptr_s_data->ACK_client,16);
			char login_enemy[20];
			dest_login_input(login_enemy);
			memcpy(send_mess.login,login_enemy,20);
			memcpy(send_mess.pass,my_login,20);
			int res_reliable_send=reliable_send(ptr_s_data->basic_fd,ptr_s_data->ACK_fd,&send_mess,sizeof(send_mess),&ptr_s_data->balancer);
			if(res_reliable_send==-1){
                                printf("отправка не удалась\n");
                                continue;
                        }
                        if(res_reliable_send==-2){
                                printf("надежная отправка не удалась\n");
                        }
                        if(res_reliable_send==1){
                                printf("надежная отправка удалась\n");
                        }
		}
	}
}
	/*
	printf("***************************\n");
        printf("*        ChatApp!!        *\n");
        printf("***************************\n");
        printf("* 1.Кому я могу написать? *\n");
        printf("***************************\n");
	char flag_ch=getchar();
        clean_stdin_buff();
	struct message send_mess;
	int res_reliable_send;
	if(flag_ch=='1'){
		send_mess.flag=23;
		send_mess.index=0;
		memcpy(&send_mess.ACK_endpoint,&ptr_s_data->ACK_client,16);
		res_reliable_send=reliable_send(ptr_s_data->basic_fd,ptr_s_data->ACK_fd,&send_mess,sizeof(send_mess),&ptr_s_data->balancer);
		if(res_reliable_send==-1){
                        printf("отправка не удалась\n");
                        //continue;
                }
                if(res_reliable_send==-2){
                        printf("надежная отправка не удалась\n");
                }
                if(res_reliable_send==1){
                        printf("надежная отправка удалась\n");
                }
	}
	reconfig(ptr_s_data->basic_fd,10);
        struct message* rec_mess;
        struct sockaddr_in new_srv;
	char rec_buff[500];
        socklen_t new_srv_len=sizeof(new_srv);
	if(recvfrom(ptr_s_data->basic_fd,rec_buff,500,0,(struct sockaddr*)&new_srv,&new_srv_len)==-1){
                if(res_reliable_send==-2){
                        printf("либо сервер не получил сообщение,либо ответ не пришел\n");
                        printf("рекомендуется повторить попытку\n");
                        //очистка буфера сокета
                        char trash[2];
                        recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
                        //continue;
			exit(1);
                }
                if(res_reliable_send==1){
                        printf("сервер получил сообщение,но его ответ не пришел\n");
                        printf("рекомендуется повторить попытку\n");
                        //очистка буфера сокета
                        char trash[2];
                        recvfrom(ptr_s_data->basic_fd,trash,2,MSG_DONTWAIT,NULL,NULL);
                        //continue;
			exit(1);
                }
        }
	else{
                        //обработка
                        if(ACK_send(ptr_s_data->basic_fd,&new_srv)==-1){
                                printf("ACK_sendto\n");
                        }
			rec_mess=(struct message*)(rec_buff);
			int count=rec_mess->us_count;
			int count2=0;
			while(count2!=count){
				char qwe[20];
				memcpy(qwe,rec_buff+76+count2*20,20);
				printf("%s\n",qwe);
				count2++;
			}
	}
}*/
