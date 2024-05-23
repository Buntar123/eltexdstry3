#include<stdio.h>
//#include<sys/types.h>
//#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc, char* argv[]){
	sleep(1);
	printf("clt\n");
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1){
                printf("open_c\n");
                exit(1);
        }
	char buffer[50];
	int sz=read(fd,buffer,50);
	if(sz==-1){
		printf("Read\n");
	}
	buffer[sz]='\0';
	printf("%s\n",buffer);
	close(fd);
	unlink(argv[1]);
}
