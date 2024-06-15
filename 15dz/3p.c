#include<stdio.h>
#include<signal.h>
#include<unistd.h>
int main(void){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask,SIGUSR1);
        if(sigprocmask(SIG_BLOCK,&mask,NULL)==-1){
                printf("sigprocmask_error\n");
        }
	int sig;
        while(1){
		sigwait(&mask,&sig);
		printf("I got SIGUSR1\n");
	}
}

