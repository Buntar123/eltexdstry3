#include<mqueue.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(void){
	mqd_t mq;
        char message[100];
        message[0]='H';
        message[1]='e';
        message[2]='l';
        message[3]='l';
	message[4]='o';
	message[5]='\0';
        mq=mq_open("/mq1",O_RDWR);
        char buff[100];
        mq_receive(mq,buff,100,NULL);
        printf("Server - %s\n",buff);
	mq_send(mq,message,100,NULL);
        mq_close(mq);
}
