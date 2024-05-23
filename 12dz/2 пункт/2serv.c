#include<stdio.h>
//#include<sys/types.h>
//#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc, char* argv[]){
	printf("srv\n");
	mode_t mode=0666;
	mkfifo(argv[1],mode);
	int fd=open(argv[1],O_WRONLY);
	if(fd==-1){
		printf("open\n");
		exit(1);
	}
	char message[]="Hi!";
	write(fd,message,sizeof(message));
	close(fd);
	//printf("srv_end\n");
}
