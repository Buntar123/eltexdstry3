#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
char** rotting_string(char* big_string,int big_size);
void free_rot_str(char** strings);
char** parce(char** strings,int first, int last);
void first_command(char** result,int* fd);
void last_command(char** result,int* fd);
void middle_command(char** result,int*fd_old,int* fd_new);
void entire_command(char** args);
/*char** parce(char** strings,int first, int last){
	char** result=malloc((last-first+1+1)*sizeof(char*));
	result[last-first+1]=NULL;
	for(int i=0;i<last-first+1;i++){
		result[i]=strings[first+i];
	}
	return result;
}
void free_parce(char** results){
	free(results);
}
void first_command(char** result,int* fd){
	if(pipe(fd)==-1){
                printf("Pipe\n");
                exit(1);
        }
	pid_t f1=fork();
	if(f1<0){
                printf("Fork\n");
                exit(1);
        }
	if(f1==0){
                close(fd[0]);
		dup2(fd[1],1);
		execvp(result[0],result);
        }
	free(result);
	waitpid(f1,NULL,0);

}
void last_command(char** result,int* fd){
	pid_t f1=fork();
	if(f1<0){
                printf("Fork\n");
                exit(1);
        }
	if(f1==0){
                close(fd[1]);
                dup2(fd[0],0);
                execvp(result[0],result);
        }
	free(result);
	close(fd[0]);
	close(fd[1]);
	waitpid(f1,NULL,0);
}
void middle_command(char** result,int*fd_old,int* fd_new){
	if(pipe(fd_new)==-1){
                printf("Pipe\n");
                exit(1);
        }
	pid_t f1=fork();
	if(f1<0){
                printf("Fork\n");
                exit(1);
        }
        if(f1==0){
                close(fd_old[1]);
		dup2(fd_old[0],0);
		close(fd_new[0]);
		dup2(fd_new[1],1);
                execvp(result[0],result);
        }
	free(result);
	close(fd_old[0]);
	close(fd_old[1]);
	waitpid(f1,NULL,0);
}
void entire_command(char** args){
	long f1=fork();
	if(f1<0){printf("err_fork\n");}
	if(f1==0){
                execvp(args[0],args);
        }
        waitpid(f1,NULL,0);
}*/
int main(void){
	while(1){
		char name[256];
        	fgets(name,256,stdin);
        	if(name[0]=='e' && name[1]=='x' && name[2]=='i' && name[3]=='t'/* && name[4]=='\n'*/){;break;}
        	for(int i=0;i<256;i++){
                	if(name[i]=='\n'){
                        	name[i]='\0';
                        	break;
                	}
        	}
        	char** args=rotting_string(name,256);
		int comm_count=1;
		int first=0,last;
		int fd_main[2];
		int fd_buff[2];
		for(int i=0;args[i]!=NULL;i++){
			if(args[i][0]=='|' && args[i][1]=='\0'){
				last=i-1;
				char** result=parce(args,first,last);
				if(comm_count==1){
					first_command(result,fd_main);
					comm_count++;
				}
				else{
					middle_command(result,fd_main,fd_buff);
					fd_main[0]=fd_buff[0];
	                        	fd_main[1]=fd_buff[1];
				}
				first=i+1;
				continue;
			}
			if(args[i+1]==NULL && comm_count==1){
                                entire_command(args);
                                break;
                        }

			if(args[i+1]==NULL){
				last=i;
				char** result=parce(args,first,last);
				last_command(result,fd_main);
				break;
			}
		}
		free_rot_str(args);
	}
}
