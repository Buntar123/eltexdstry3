#include<mqueue.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(void){
	mqd_t mq;
	char message[4];
	message[0]='H';
	message[1]='i';
	message[2]='!';
	message[3]='\0';
	struct mq_attr attr;
        attr.mq_flags=0;
        attr.mq_maxmsg=10;
        attr.mq_msgsize=100;
        attr.mq_curmsgs=0;
	mq=mq_open("/mq1",O_CREAT |O_RDWR, 0666,&attr);
	mq_send(mq,message,100,NULL);
	sleep(2);
	char buff[100];
	mq_receive(mq,buff,100,NULL);
	printf("Client - %s\n",buff);
	mq_close(mq);
}
