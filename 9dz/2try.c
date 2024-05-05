#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
void creatE(){
	char buff_name[25];
        fgets(buff_name,25,stdin);
        int i=0;
        while((buff_name[i])!='\n'){
                i++;
        }
        buff_name[i]='\0';
        int c=creat(buff_name,S_IRWXU);
        if(c==-1){printf("1\n");exit(1);}
        char buff[100];
        fgets(buff,100,stdin);
        FILE* file=fopen(buff_name,"w");
        fprintf(file,buff);
        fclose(file);
}
void creatE_2(char* name, char* content){
	int c=creat(name,S_IRWXU);
        if(c==-1){printf("1\n");exit(1);}
        FILE* file=fopen(name,"w");
        fprintf(file,content);
        fclose(file);
}
int main(void){
	char name[]="output.txt";
	char content[]="String from file";
	creatE_2(name,content);
	FILE* file=fopen(name,"r");
	fseek(file,0,SEEK_END);
	long pos = ftell(file);
	char ch;
	while(pos>=0){
		fseek(file,pos,SEEK_SET);
		ch=fgetc(file);
		if(ch!=EOF){
			printf("%c",ch);
		}
		pos--;
	}
	fclose(file);
}
