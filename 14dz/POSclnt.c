#include<stdio.h>
#include<sys/mman.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
	int fd_shm=shm_open("/shm",O_RDWR,0666);
        char* addr_shm=(char*)mmap(NULL,1024,PROT_READ | PROT_WRITE,MAP_SHARED,fd_shm,0);
        sem_t *sem1=sem_open("/sem1",O_RDWR,0666);
        sem_t *sem2=sem_open("/sem2",O_RDWR,0666);
	sem_wait(sem1);
	char buff[20];
        int j;
        for(j=0;addr_shm[j]!='\0';j++){
                buff[j]=addr_shm[j];
        }
        buff[j]=addr_shm[j];
        printf("%s\n",buff);
        addr_shm[0]='H';
        addr_shm[1]='e';
        addr_shm[2]='l';
        addr_shm[3]='l';
        addr_shm[4]='o';
        addr_shm[5]='!';
        addr_shm[6]='\0';
        sem_post(sem2);
        sem_unlink(sem1);
        sem_unlink(sem2);
        shm_unlink("/shm");
}
