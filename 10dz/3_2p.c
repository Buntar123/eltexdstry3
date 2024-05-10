#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
void free_rot_str(char** strings){
	int j=0;
	while(strings[j]!=NULL){
		free(strings[j]);
		j++;
	}
	free(strings[j]);
	free(strings);
}
char** rotting_string(char* big_string,int big_size){
	int pr_count=0;
	for(int i=0;i<big_size;i++){
                if(big_string[i]=='\0'){
                        break;
                }
                if(big_string[i]==' '){
                        pr_count++;
                }
        }
        char** strings=(char**)malloc((pr_count+2)*sizeof(char*));
        if(strings==NULL){return NULL;}
        int down_elm=0;
        int s_count=0;
        for(int i=0;i<big_size;i++){
                int end_count=0;
                if(big_string[i]=='\0'){
                        end_count=1;
                }
                if(big_string[i]==' ' || big_string[i]=='\0'){
                        int sz=i-down_elm;
                        strings[s_count]=(char*)malloc(sz*sizeof(char));
                        for(int j=0;j<sz;j++){
                                strings[s_count][j]=big_string[down_elm+j];
                        }
                        down_elm=i+1;
                        s_count++;
                }
                if(end_count==1){break;}
	}
        strings[pr_count+1]=NULL;
	return strings;
}
int main(void){
	while(1){
		char name[256];
                fgets(name,256,stdin);
		if(name[0]=='e' && name[1]=='x' && name[2]=='i' && name[3]=='t' && name[4]=='\n'){exit(1);}
		for(int i=0;i<256;i++){
			if(name[i]=='\n'){
                                name[i]='\0';
                                break;
                        }
		}
                long f1=fork();
                if(f1==0){
			char** args=rotting_string(name,256);
			if(args==NULL){printf("rotting_string\n");exit(1);}
                        execvp(args[0],args);
			free_rot_str(args);
                        exit(1);
                }
		if(f1<0){printf("err_fork\n");break;}
                else{waitpid(f1,NULL,0);}
	}
}
