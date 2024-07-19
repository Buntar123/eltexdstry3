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
struct data_to_value{
        char key[20];
        char value[20];
};
long hash(char* key){
	long sum=0;
	for(int i=0;i<strlen(key);i++){
		sum+=key[i]*(i+1);
	}
	return sum;
}
int checklogin(char* key,struct data_to_value* elements){
	for(int i=0;i<USERCOUNT;i++){
		if(strcmp(elements[i].key,key)==0){
			return -1;
		}
	}
	return 1;
}
void add(char* key,char* value,struct data_to_value* elements){
	FILE *fd=fopen("database.txt","a");
	long hashed=hashed % USERCOUNT;
	if(strcmp(elements[hashed].key,"")==0){
		long temp=hashed;
		strncpy((elements+temp)->key,key,20);
		strncpy((elements+temp)->value,value,20);
	}
	else{
		strncpy((elements+hashed)->key,key,20);
		strncpy((elements+hashed)->value,value,20);
	}
	fputs(key,fd);
	fputs((elements+hashed)->value,fd);
	fclose(fd);
}
int get(char* key,char* value,struct data_to_value* elements){
	for(int i=0;i<USERCOUNT;i++){
		if(strcmp(elements[i].key,key)==0){
			if(strcmp(elements[i].value,value)==0){
				return 1;
			}
			else{
				return -1;
			}
		}
	}
	return -2;
}
