#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <malloc.h>
#include<pthread.h>
#include<ncurses.h>
char* str_rework(char* string,int size);
void free_rot_str(char** strings);
void str_anti_rework(char* statlc,char* dynamic,int size);
int str_check(char* string1,char* string2);
void stitching(char* dynamic,char* statlc,int size);
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
struct msgbuf{
        long mtype;
        long pid;
        char mtext[100];
};
struct for_readc{ 
        char** names;
        char** messages;
	WINDOW* down;
        WINDOW* left;
        WINDOW* right;

};
int q_id1,q_id2;
int ch_stat=0,us_stat=0;
char PROB[118];
char PROB2[38];
char PROB3[78];
char Ex[]="=#EXIT#=";
char Comm1[]="=#CH_UP#=";
char Comm2[]="=#CH_DN#=";
char Comm3[]="=#US_UP#=";
char Comm4[]="=#US_DN#=";
void output(struct for_readc* args,int mark){
	int j;
	if(mark==1){
		for(j=0;j<30 && args->names[us_stat*30+j]!=NULL;j++){
			mvwprintw(args->right,1+j,1,args->names[us_stat*30+j]);
		}
		for(;j<30;j++){
			mvwprintw(args->right,1+j,1,PROB2);
		}
		wrefresh(args->right);
	}
	else if(mark==2){
		for(j=0;j<30 && args->messages[ch_stat*30+j]!=NULL;j++){
                        mvwprintw(args->left,1+j,1,args->messages[ch_stat*30+j]);
                }
                for(;j<30;j++){
                        mvwprintw(args->left,1+j,1,PROB3);
                }
                wrefresh(args->left);
	}
}
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
		//
		pthread_mutex_lock(&m1);
		output(args,1);
		pthread_mutex_unlock(&m1);
		//
		/*printf("-----\n");
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
		//
		pthread_mutex_lock(&m1);
                output(args,2);
                pthread_mutex_unlock(&m1);
		//
		/*printf("=ser=\n");
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
	char buff[100];
        for(int i=0;i<100;i++){buff[i]='\0';}
        mvwgetstr(args->down,1,1,buff);
        mvwprintw(args->down,1,1,PROB);
        wrefresh(args->down);
        for(int i=0;i<100;i++){msg.mtext[i]=buff[i];}
        //fgets(msg.mtext,100,stdin);
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
	//
	pthread_mutex_lock(&m1);
        output(args,1);
        pthread_mutex_unlock(&m1);
	//
	/*printf("-----\n");
        for(int i=0;args->names[i]!=NULL;i++){
                printf("%s\n",args->names[i]);
        }
        printf("-----\n");*/
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
        	//wrefresh(args->down);
        	for(int i=0;i<100;i++){msg.mtext[i]=buff[i];}
                //fgets(msg.mtext,100,stdin);
                for(int i=0;i<100;i++){
                        if(msg.mtext[i]=='\n'){
                                msg.mtext[i]='\0';
                                break;
                         }
                }
		int j1;
	        for(j1=0;args->messages[j1]!=NULL;j1++){}
		pthread_mutex_lock(&m1);
		if(str_check(Comm1,msg.mtext)==1){
			if(j1>30*(ch_stat+1)){
				ch_stat++;
				output(args,2);
			}
			pthread_mutex_unlock(&m1);
			continue;
		}
                if(str_check(Comm2,msg.mtext)==1){
			if(ch_stat!=0){
				ch_stat--;
				output(args,2);
			}
			pthread_mutex_unlock(&m1);
                        continue;
		}
		for(j1=0;args->names[j1]!=NULL;j1++){}
                if(str_check(Comm3,msg.mtext)==1){
			if(j1>30*(us_stat+1)){
				us_stat++;
				output(args,1);
			}
			pthread_mutex_unlock(&m1);
			continue;
		}
                if(str_check(Comm4,msg.mtext)==1){
			if(us_stat!=0){
				us_stat--;
                        	output(args,1);
			}
			pthread_mutex_unlock(&m1);
			continue;
		}
		pthread_mutex_unlock(&m1);
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
			//
			pthread_mutex_lock(&m1);
                        output(args,2);
                        pthread_mutex_unlock(&m1);
			//
			/*printf("=sam=\n");
                	for(int i=0;args->messages[i]!=NULL;i++){
                        	printf("%s\n",args->messages[i]);
                	}
                	printf("=====\n");*/
                        msgsnd(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
                }
        }
}
int main(void){
	//
	for(int i=0;i<38;i++){
		if(i==37){PROB2[i]='\0';}
		else{PROB2[i]=' ';}
	}
	for(int i=0;i<78;i++){
		if(i==77){PROB3[i]='\0';}
		else{PROB3[i]=' ';}
	}
	for(int i=0;i<118;i++){
                if(i==117){PROB[i]='\0';}
                else{PROB[i]=' ';}
        }
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
	//
        key_t key = ftok("Mark",1);
        q_id1 = msgget(key,0);
        key_t key_user = ftok("Mark",2);
        q_id2 = msgget(key_user,0);
        char** names,messages;
        struct for_readc rdrc_args={.names=names,.messages=messages,.down=win1_down,.left=win2_left,.right=win3_right};
        rdrc_args.names=malloc(sizeof(char*));
        rdrc_args.names[0]=NULL;
        rdrc_args.messages=malloc(sizeof(char*));
        rdrc_args.messages[0]=NULL;
        pthread_t t_readc, t_user_readc,t_user_send;
        pthread_create(&t_readc,NULL,pth_readc,&rdrc_args);
        pthread_create(&t_user_readc,NULL,pth_user_readc,&rdrc_args);
        REG(&rdrc_args);
        pthread_create(&t_user_send,NULL,pth_user_send,&rdrc_args);
        pthread_join(t_user_send,NULL);
        pthread_cancel(t_readc);
        pthread_cancel(t_user_readc);
        pthread_join(t_readc,NULL);
        pthread_join(t_user_readc,NULL);
        free_rot_str(rdrc_args.names);
        free_rot_str(rdrc_args.messages);
}
