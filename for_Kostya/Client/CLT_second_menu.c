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
void dest_login_input(char* login){
	while(1){
		int end;
                printf("Введите логин возможного собеседника\n");
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
		break;
	}
}
