#include<stdio.h>
#include<malloc.h>
#include<pthread.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
int ID1,ID2,ID3;
struct sembuf lock[2]={{0,0,0},{0,1,0}};
struct sembuf unlock={0,-1,0};
void pth_forw(void* arg){
	while(1){
		int notif=semctl(ID3,1,GETVAL);
		if(notif==0){continue;}
		semop(ID3,&lock,2);
		int N_val=semctl(ID3,4,GETVAL);
		if(N_val==0){pthread_exit(NULL);}
		printf("I send to %d, %d\n",N_val,notif);
		semctl(ID3,2,SETVAL,N_val);
		semctl(ID3,1,SETVAL,0);
		semop(ID3,&unlock,1);
	}
}
int main(void){
	key_t key1=ftok("M2",1);
	ID1=shmget(key1,10000,IPC_CREAT | 0666);
	char* messages=(char*)shmat(ID1,NULL,0);
	key_t key2=ftok("M2",2);
        ID2=shmget(key2,10000,IPC_CREAT | 0666);
        char* names=(char*)shmat(ID2,NULL,0);
        key_t key3=ftok("M2",3);
        ID3=semget(key3,5,IPC_CREAT | 0666);
	pthread_t t_forw;
	pthread_create(&t_forw,NULL,pth_forw,NULL);
	pthread_join(t_forw,NULL);
	semctl(ID3,0,IPC_RMID,NULL);
        shmctl(ID2,IPC_RMID,NULL);
        shmctl(ID1,IPC_RMID,NULL);
}
