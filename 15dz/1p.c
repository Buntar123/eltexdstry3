#include<stdio.h>
#include<signal.h>
void handler(int signum){
	printf("I got sigusr1\n");
}
int main(void){
	struct sigaction sa;
	sa.sa_handler=handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	if(sigaction(SIGUSR1,&sa,NULL)==-1){
		printf("error\n");
	}
	while(1){pause();}
}
