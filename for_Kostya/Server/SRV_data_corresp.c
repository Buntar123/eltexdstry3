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
struct data_currsp{
	char first_clt[20];
	char second_clt[20];
	void* history;
};
struct pers_corrsp{
	int number_msg;
	char msg[100];
};
struct pers_corrsp* pers_list;
void init_pers_list(struct pers_corrsp* pers_list){

}
