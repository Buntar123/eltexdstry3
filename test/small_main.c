#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <malloc.h>
#include<pthread.h>
#include<ncurses.h>
#include<menu.h>
char* str_rework(char* string,int size);
void free_rot_str(char** strings);
void str_anti_rework(char* statlc,char* dynamic,int size);
int str_check(char* string1,char* string2);
void stitching(char* dynamic,char* statlc,int size);
struct msgbuf{
        long mtype;
	long pid;
        char mtext[100];
};
struct for_readc {
	char** names;
	char** messages;
	WINDOW* down;
	WINDOW* left;
	WINDOW* right;
};
int q_id1,q_id2;
char Ex[]="=#EXIT#=";
char PROB[118];
void pth_readc(void* arg){
	struct for_readc* args=(struct for_readc*)arg;
	struct msgbuf msg;
	while(1){
		pthread_testcancel();
		msgrcv(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),getpid(),0);
		int j,del_num;
                for(j=0;args->names[j]!=NULL;j++){
			if(str_check(args->names[j],msg.mtext)==1){
				del_num=j;
			}
		}
		if(msg.pid==1){
			args->names=realloc(args->names,(j+2)*sizeof(char*));
        	        args->names[j]=str_rework(msg.mtext,100);
	                args->names[j+1]=NULL;
		}
		else if(msg.pid==-1){
			free(args->names[del_num]);
			for(int i=del_num;args->names[i]!=NULL;i++){
				args->names[i]=args->names[i+1];
			}
			args->names=realloc(args->names,j*sizeof(char*));
		}
		/*
		printf("-----\n");
                for(int i=0;args->names[i]!=NULL;i++){
                        printf("%s\n",args->names[i]);
                }
                printf("-----\n");*/
	}
}
void pth_user_readc(void* arg){
	struct for_readc* args=(struct for_readc*)arg;
        struct msgbuf msg;
	while(1){
		pthread_testcancel();
		msgrcv(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),getpid(),0);
		int j;
                for(j=0;args->messages[j]!=NULL;j++){}
		args->messages=realloc(args->messages,(j+2)*sizeof(char*));
		args->messages[j]=str_rework(msg.mtext,100);
                args->messages[j+1]=NULL;
		/*printf("ser==\n");
                for(int i=0;args->messages[i]!=NULL;i++){
                        printf("%s\n",args->messages[i]);
                }
                printf("=====\n");*/
	}
}
void REG(struct for_readc* args){
	struct msgbuf msg;
	msg.mtype=1;
        msg.pid=getpid();
	fgets(msg.mtext,100,stdin);
        for(int i=0;i<100;i++){
                if(msg.mtext[i]=='\n'){
                        msg.mtext[i]='\0';
                        break;
                }
        }
	int j;
        for(j=0;args->names[j]!=NULL;j++){}
	args->names=realloc(args->names,(j+2)*sizeof(char*));
        args->names[j]=str_rework(msg.mtext,100);
        args->names[j+1]=NULL;
	msgsnd(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
}
void pth_user_send(void* arg){
	struct for_readc* args=(struct for_readc*)arg;
	struct msgbuf msg;
	msg.mtype=1;
        msg.pid=getpid();
	while(1){
		char buff[100];
                for(int i=0;i<100;i++){buff[i]='\0';}
                mvwgetstr(args->down,1,1,buff);
                mvwprintw(args->down,1,1,PROB);
		wrefresh(args->left);
                wrefresh(args->down);
		for(int i=0;i<100;i++){
                        msg.mtext[i]=buff[i];
                }
		//fgets(msg.mtext,100,stdin);
		for(int i=0;i<100;i++){
                        if(msg.mtext[i]=='\n'){
                                msg.mtext[i]='\0';
                                break;
                         }
                }
		if(str_check(Ex,msg.mtext)==1){
                        str_anti_rework(msg.mtext,args->names[0],100);
                        msgsnd(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
			endwin();
			pthread_exit(NULL);
                }
		else{
			stitching(args->names[0],msg.mtext,100);
                        int k;
                        for(k=0;args->messages[k]!=NULL;k++){}
                        args->messages=realloc(args->messages,(k+2)*sizeof(char*));
                        args->messages[k]=str_rework(msg.mtext,100);
                        args->messages[k+1]=NULL;
                        msgsnd(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
                        /*printf("sam==\n");
                        for(int i=0;args->messages[i]!=NULL;i++){
                                printf("%s\n",args->messages[i]);
                        }
                        printf("=====\n");*/
                }
	}
}
int main(void){
        for(int i=0;i<118;i++){PROB[i]=' ';}
	key_t key = ftok("Mark",1);
        q_id1 = msgget(key,0);
	key_t key_user = ftok("Mark",2);
	q_id2 = msgget(key_user,0);
	char** names,messages;
	//srt
	initscr();
	cbreak();
	int he_2=32,wi_2=80,sty_2=1,stx_2=1;
        int he_3=32,wi_3=40,sty_3=1,stx_3=81;
        int he_4=3,wi_4=120,sty_4=33,stx_4=1;
	WINDOW* win2_left=newwin(he_2,wi_2,sty_2,stx_2);
        WINDOW* win3_right=newwin(he_3,wi_3,sty_3,stx_3);
        WINDOW* win1_down=newwin(he_4,wi_4,sty_4,stx_4);
	box(win2_left,0,0);
        box(win3_right,0,0);
        box(win1_down,0,0);
        mvwprintw(win2_left,0,38,"CHAT");
        mvwprintw(win3_right,0,14,"PARTICIPANTS");
        mvwprintw(win1_down,0,51,"ENTER YOUR MESSAGE");
        refresh();
        wrefresh(win2_left);
        wrefresh(win3_right);
        wrefresh(win1_down);
	//end
	struct for_readc rdrc_args={.names=names,.messages=messages,.down=win1_down,.left=win2_left,.right=win3_right};
	rdrc_args.names=malloc(sizeof(char*));
	rdrc_args.names[0]=NULL;
	rdrc_args.messages=malloc(sizeof(char*));
	rdrc_args.messages[0]=NULL;
	//srt
	//end
	pthread_t t_readc, t_user_readc,t_user_send;
        pthread_create(&t_readc,NULL,pth_readc,&rdrc_args);
	pthread_create(&t_user_readc,NULL,pth_user_readc,&rdrc_args);
	pthread_create(&t_user_send,NULL,pth_user_send,&rdrc_args);
	REG(&rdrc_args);
	pthread_join(t_user_send,NULL);
	pthread_cancel(t_readc);
	pthread_cancel(t_user_readc);
	pthread_join(t_readc,NULL);
        pthread_join(t_user_readc,NULL);
	endwin();
        free_rot_str(rdrc_args.names);
	free_rot_str(rdrc_args.messages);
}
