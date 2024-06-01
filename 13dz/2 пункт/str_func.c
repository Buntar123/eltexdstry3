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
char* str_rework(char* string,int size){
        int j;
        for(j=0;string[j]!='\0' || j<size;j++){}
        char* rew_str=malloc((j+1)*sizeof(char));
	if(rew_str==NULL){printf("malloc\n");exit(1);}
        for(int i=0;i<=j;i++){
                rew_str[i]=string[i];
        }
        return rew_str;
}
void str_anti_rework(char* statlc,char* dynamic,int size){
	int j;
	for(j=0;dynamic[j]!='\0' && j<size;j++){
		statlc[j]=dynamic[j];
	}
	statlc[j]='\0';
}
int str_check(char* string1,char* string2){
        int i;
        for(i=0;(string1[i]!='\0')||(string2[i]!='\0');i++){
                if(string1[i]!=string2[i]){
                        return 0;
                }
        }
        if(string1[i]!=string2[i]){
                return 0;
        }
        return 1;
}
void stitching(char* dynamic,char* statlc,int size){
        char buff[size];
        for(int i=0;i<size;i++){
                buff[i]=statlc[i];
        }
        int j;
        for(j=0;dynamic[j]!='\0';j++){
                statlc[j]=dynamic[j];
        }
        statlc[j]=':';
        j++;
        for(int i=0;buff[i]!='\0';j++,i++){
                statlc[j]=buff[i];
        }
        statlc[j]='\0';
}

