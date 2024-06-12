#include<stdio.h>
#include<pthread.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
struct sembuf dec={0,-1,0};
struct sembuf add={1,1,0};
int main(void){
        key_t key1=ftok("M2",1);
        int ID1=shmget(key1,10000,0);
        char* messages=(char*)shmat(ID1,NULL,0);
        key_t key3=ftok("M2",3);
        int ID2=semget(key3,2,0);
	semop(ID2,&dec,1);
	char buff[20];
	int j;
        for(j=0;messages[j]!='\0';j++){
                buff[j]=messages[j];
        }
	buff[j]=messages[j];
	printf("%s\n",buff);
	messages[0]='H';
	messages[1]='e';
	messages[2]='l';
	messages[3]='l';
	messages[4]='o';
	messages[5]='!';
	messages[6]='\0';
	semop(ID2,&add,1);
}

