#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<malloc.h>
#include<pthread.h>
struct msgbuf{
        long mtype;
        char mtext[100];
};
int main(void){
	key_t key = ftok("1p_serv",1);
        int q_id1 = msgget(key,0666 | IPC_CREAT);
        struct msgbuf msg;
	msgrcv(q_id1,&msg,sizeof(msg.mtext),2,0);
	printf("Server - %s",msg.mtext);
	msg.mtype=1;
	msg.mtext[0]='H';
	msg.mtext[1]='e';
	msg.mtext[2]='l';
	msg.mtext[3]='l';
	msg.mtext[4]='o';
	msg.mtext[5]='!';
	msg.mtext[6]='\0';
	msgsnd(q_id1,&msg,sizeof(msg.mtext),0);
}
