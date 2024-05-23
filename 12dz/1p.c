#include<stdio.h>
int main(void){
	int fd[2];
	if(pipe(fd)==-1){
		printf("Pipe\n");
		exit(1);
	}
	long f1=fork();
	if(f1<0){
		printf("Fork\n");
                exit(1);
	}
	if(f1==0){
		char buffer[50];
		close(fd[1]);
		read(fd[0],buffer,3);
		printf("%s\n",buffer);
		printf("Child:Exit\n");
		close(fd[0]);
		exit(1);
	}
	close(fd[0]);
	write(fd[1],"Hi!",3);
	wait();
	printf("Parent:Child exit\n");
	close(fd[1]);
}
