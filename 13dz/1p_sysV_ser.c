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
	msg.mtext[0]='H';
	msg.mtext[1]='i';
	msg.mtext[2]='!';
	msg.mtext[3]='\0';
	msg.mtype=2;
	msgsnd(q_id1,&msg,sizeof(msg.mtext),0);
	msgrcv(q_id1,&msg,sizeof(msg.mtext),1,0);
	printf("Client - %s\n",msg.mtext);
	msgctl(q_id1,IPC_RMID,NULL);
}
