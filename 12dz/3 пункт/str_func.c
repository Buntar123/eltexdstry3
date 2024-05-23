#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
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
                        strings[s_count]=(char*)malloc((sz+1)*sizeof(char));
                        for(int j=0;j<sz;j++){
                                strings[s_count][j]=big_string[down_elm+j];
                        }
                        strings[s_count][sz]='\0';
                        down_elm=i+1;
                        s_count++;
                }
                if(end_count==1){break;}
        }
        strings[pr_count+1]=NULL;
        return strings;
}
void free_rot_str(char** strings){
        int j=0;
        while(strings[j]!=NULL){
                free(strings[j]);
                j++;
        }
        free(strings[j]);
        free(strings);
}
char** parce(char** strings,int first, int last){
        char** result=malloc((last-first+1+1)*sizeof(char*));
        result[last-first+1]=NULL;
        for(int i=0;i<last-first+1;i++){
                result[i]=strings[first+i];
        }
        return result;
}

