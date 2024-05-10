#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
int main(void){
        int status;
        long f1=fork();
        if(f1==0){
                printf("Process1: pid - %d ppid - %d\n",getpid(),getppid());
                long f3=fork();
                if(f3==0){printf("Process3: pid - %d ppid - %d\n",getpid(),getppid());exit(1);}
                long f4=fork();
                if(f4==0){printf("Process4: pid - %d ppid - %d\n",getpid(),getppid());exit(1);}
		if(f3<0){printf("f3\n");}
		else{waitpid(f3,&status,0);printf("Exit pid - %ld\n",f3);}
		if(f4<0){printf("f4\n");}
		else{waitpid(f4,&status,0);printf("Exit pid - %ld\n",f4);}
                exit(1);
        }
        long f2=fork();
        if(f2==0){
                printf("Process2: pid - %d ppid - %d\n",getpid(),getppid());
                long f5=fork();
                if(f5==0){printf("Process5: pid - %d ppid - %d\n",getpid(),getppid());exit(1);}
		if(f5<0){printf("f5\n");}
		else{waitpid(f5,&status,0);printf("Exit pid - %ld\n",f5);}
                exit(1);
        }
	if(f1<0){printf("f1\n");}
	else{waitpid(f1,&status,0);printf("Exit pid - %ld\n",f1);}
	if(f2<0){printf("f2\n");}
	else{waitpid(f2,&status,0);printf("Exit pid - %ld\n",f2);}
}

