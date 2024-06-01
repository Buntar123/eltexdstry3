#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<malloc.h>
#include<pthread.h>
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
struct msgbuf{
        long mtype;
        long pid;
        char mtext[100];
};
struct for_read{
        char** names;
        long* prior;
	char** messages;
};
int q_id1,q_id2;
char* str_rework(char* string,int size);
void free_rot_str(char** strings);
void free_rot_long(long* mass){
	int j;
	for(j=0;mass[j]!=0;j++){
		free(mass[j]);
	}
	free(mass[j]);
	free(mass);
};
void str_anti_rework(char* statlc,char* dynamic,int size);
int str_check(char* string1,char* string2);
void pth_read(void* arg){
	struct for_read* args=(struct for_read*)arg;
        struct msgbuf msg;
	while(1){
		msgrcv(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),1,0);
		int j,del_or_add=0,del_num;
                for(j=0;args->prior[j]!=0;j++){
			if(args->prior[j]==msg.pid){
				del_or_add=1;
				del_num=j;
			}
		}
		if(del_or_add==1){
			printf("I know him\n");
			free(args->names[del_num]);
			for(int i=del_num;args->prior[i]!=0;i++){
				args->prior[i]=args->prior[i+1];
				args->names[i]=args->names[i+1];
			}
			args->names=realloc(args->names,j*sizeof(char*));
                        args->prior=realloc(args->prior,j*sizeof(long));
			if(args->names[0]==NULL){
				printf("EX1\n");
				while(1){
					int ch1=msgrcv(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),1,IPC_NOWAIT);
					int ch2=msgrcv(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),1,IPC_NOWAIT);
					if(ch1==-1 && ch2==-1){break;}
				}
				printf("EX2\n");
				msgctl(q_id1,IPC_RMID,NULL);
				msgctl(q_id2,IPC_RMID,NULL);
				printf("EX3\n");
				pthread_exit(NULL);
			}
			for(int i=0;args->prior[i]!=0;i++){
                                msg.mtype=args->prior[i];
                                msg.pid=-1;
                                msgsnd(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
                        }
		}
		else{
			printf("I dont know him\n");
			struct msgbuf msg2;
			pthread_mutex_lock(&m1);//lock
			for(int i=0;args->messages[i]!=NULL;i++){
				str_anti_rework(msg2.mtext,args->messages[i],100);
				msg2.mtype=msg.pid;
				msg2.pid=0;
				msgsnd(q_id2,&msg2,sizeof(msg.mtext)+sizeof(msg.pid),0);
			}
			args->names=realloc(args->names,(j+2)*sizeof(char*));
	                args->prior=realloc(args->prior,(j+2)*sizeof(long));
			args->names[j]=str_rework(msg.mtext,100);
	                args->prior[j]=msg.pid;
			args->names[j+1]=NULL;
	                args->prior[j+1]=0;
			pthread_mutex_unlock(&m1);//unlock
			for(int i=0;i!=j;i++){
				msg.mtype=args->prior[i];
				msg.pid=1;
				msgsnd(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
			}
			for(int i=0;i!=j;i++){
				msg.mtype=args->prior[j];
				str_anti_rework(msg.mtext,args->names[i],100);
                                msg.pid=1;
				msgsnd(q_id1,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
			}
		}
		printf("-----\n");
		for(int i=0;args->names[i]!=NULL;i++){
			printf("%s\n",args->names[i]);
		}
		printf("-----\n");
	}
}
void pth_user_read(void* arg){
	struct for_read* args=(struct for_read*)arg;
        struct msgbuf msg;
	while(1){
		msgrcv(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),1,0);
		int j;
		for(j=0;args->messages[j]!=NULL;j++){}
		pthread_mutex_lock(&m1);//lock
		args->messages=realloc(args->messages,(j+2)*sizeof(char*));
		args->messages[j]=str_rework(msg.mtext,100);
		args->messages[j+1]=NULL;
		for(int i=0;args->prior[i]!=0;i++){
			if(args->prior[i]!=msg.pid){
				msg.mtype=args->prior[i];
                       		msgsnd(q_id2,&msg,sizeof(msg.mtext)+sizeof(msg.pid),0);
			}
		}
		pthread_mutex_unlock(&m1);//unlock
		printf("=====\n");
                for(int i=0;args->messages[i]!=NULL;i++){
                        printf("%s\n",args->messages[i]);
                }
                printf("=====\n");
		pthread_testcancel();
	}
}
int main(void){
	key_t key = ftok("Mark",1);
	key_t key_user = ftok("Mark",2);
        q_id1 = msgget(key,0666 | IPC_CREAT);
	q_id2 = msgget(key_user,0666 | IPC_CREAT);
	char** messages,names;
	long* prior;
	struct for_read rdr_args={.names=names,.prior=prior, .messages=messages};
	rdr_args.messages=malloc(sizeof(char*));
        rdr_args.names=malloc(sizeof(char*));
        rdr_args.prior=malloc(sizeof(long));
	rdr_args.messages[0]=NULL;
	rdr_args.names[0]=NULL;
	rdr_args.prior[0]=0;
	pthread_t t_read, t_u_read;
        pthread_create(&t_read,NULL,pth_read,&rdr_args);
	pthread_create(&t_u_read,NULL,pth_user_read,&rdr_args);
        pthread_join(t_read,NULL);
	pthread_cancel(t_u_read);
	msgctl(q_id1,IPC_RMID,NULL);
        msgctl(q_id2,IPC_RMID,NULL);
	pthread_join(t_u_read,NULL);
	free_rot_str(rdr_args.names);
        free_rot_str(rdr_args.messages);
        free_rot_long(rdr_args.prior);
}
