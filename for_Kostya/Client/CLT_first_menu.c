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
	char us_index;
};
void clean_stdin_buff(){
        int c;
        while((c=getchar())!='\n' && c!=EOF);
}
void data_input(char* login,char* pass){
        while(1){
                int end;
                printf("Введите логин\n");
                printf("P.S. Не менее 5, но не более 20\n");
                fgets(login,20,stdin);
                for(int i=0;i<20;i++){
                        if(login[i]=='\n'){
                                end=i;
                                memset(login+i,'\0',20-i);
                                break;
                        }
                }
                if(end<5){
                        printf("Не менее 5 символов!\n");
                        continue;
                }
                printf("Введите пароль\n");
                printf("P.S. Не менее 5 но не более 20\n");
                fgets(pass,20,stdin);
                for(int i=0;i<20;i++){
                        if(pass[i]=='\n'){
                                end=i;
                                memset(pass+i,'\0',20-i);
                                break;
                        }
                }
                if(end<5){
                        printf("Не менее 5 символов!\n");
                        continue;
                }
                break;
        }
}
extern char my_login[20];
void first_menu(struct message* send_mess,struct sockaddr_in* ACK_client,struct sockaddr_in* USR_client){
        printf("********************\n");
        printf("*     ChatApp!     *\n");
        printf("********************\n");
        printf("* 1.Авторизация    *\n");
        printf("* 2.Регистрация    *\n");
        printf("* 3.Аннигиляция    *\n");
        printf("********************\n");
        char flag;
	while(1){
		flag=getchar();
        	clean_stdin_buff();
		if(flag=='1' || flag=='2' || flag=='3'){
			break;
		}
		printf("Неверный символ!\n");
	}
        if(flag=='3'){
                exit(1);
        }
        char pass[20];
        char login[20];
        data_input(login,pass);
        memcpy(my_login,login,20);
        memcpy(send_mess->login,login,20);
        memcpy(send_mess->pass,pass,20);
        switch(flag){
                case '1':
                        send_mess->flag=16;
                        send_mess->index=0;
                        memcpy(&send_mess->ACK_endpoint,ACK_client,16);
			memcpy(&send_mess->USR_endpoint,USR_client,16);
                        break;
                case '2':
                        send_mess->flag=17;
                        send_mess->index=0;
                        memcpy(&send_mess->ACK_endpoint,ACK_client,16);
			memcpy(&send_mess->USR_endpoint,USR_client,16);
                        break;
        }
        //данные готовы к отправке
}

