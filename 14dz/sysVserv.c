#include<stdio.h>
#include<pthread.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
struct sembuf add={0,1,0};
struct sembuf dec={1,-1,0};
int main(void){
	key_t key1=ftok("M2",1);
        int ID1=shmget(key1,10000,IPC_CREAT | 0666);
        char* messages=(char*)shmat(ID1,NULL,0);
        key_t key3=ftok("M2",3);
        int ID2=semget(key3,2,IPC_CREAT | 0666);
	messages[0]='H';
	messages[1]='i';
	messages[2]='!';
	messages[3]='\0';
	semop(ID2,&add,1);
	semop(ID2,&dec,1);
	char buff[20];
        int j;	
	for(j=0;messages[j]!='\0';j++){
		buff[j]=messages[j];
	}
	buff[j]=messages[j];
	printf("%s\n",buff);
	shmctl(ID1,IPC_RMID,NULL);
	semctl(ID2,0,IPC_RMID,NULL);
}

