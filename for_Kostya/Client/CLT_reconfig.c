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
void reconfig(int ACK_fd,int time){
        struct timeval tv;
        tv.tv_sec=time;
        tv.tv_usec=0;
        if(setsockopt(ACK_fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv))==-1){
                return -1;
        }
}
