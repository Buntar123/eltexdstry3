#include<stdio.h>
#include<pthread.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<ncurses.h>
char my_Name[38];
int ch_stat=0,us_stat=0;
char Ex[]="=#EXIT#=";
char Comm1[]="=#CH_UP#=";
char Comm2[]="=#CH_DN#=";
char Comm3[]="=#US_UP#=";
char Comm4[]="=#US_DN#=";
int ID1,ID2,ID3;
char PROB[118];
char PROB2[38];
char PROB3[78];
pthread_t t_write, t_react;
struct sembuf lock[2]={{0,0,0},{0,1,0}};
struct sembuf v4unlock[3]={{4,1,0},{1,1,0},{0,-1,0}};
struct sembuf v3unlock={1,1,0};
struct sembuf qwe[2]={{3,1,0},{1,1,0}};
struct sembuf v2lock[3]={{0,0,0},{0,1,0},{2,-1,0}};
struct sembuf v22lock[3]={{2,-1,0},{0,0,0},{0,1,0}};
struct sembuf unlock={0,-1,0};
struct sembuf dec4={4,-1,0};
struct mass{
        char* messages;
        char* names;
	WINDOW* down;
        WINDOW* left;
        WINDOW* right;
};
void output(struct mass* args,int Nnum,int mnum){
        int j=0;
        for(int i=0+38*30*us_stat;j<30 && i!=Nnum*38;j++,i+=38){
                char buff[38];
                for(int ib=i,ic=0;ic<38;ic++,ib++){buff[ic]=args->names[ib];}
                mvwprintw(args->right,1+j,1,buff);
        }
        for(;j<30;j++){
                mvwprintw(args->right,1+j,1,PROB2);
        }
        j=0;
        for(int i=0+78*30*ch_stat;j<30 && i!=mnum*78;j++,i+=78){
                char buff[78];
                for(int ib=i,ic=0;ic<78;ic++,ib++){buff[ic]=args->messages[ib];}
                mvwprintw(args->left,1+j,1,buff);
        }
        for(;j<30;j++){
                mvwprintw(args->left,1+j,1,PROB3);
        }
        wrefresh(args->right);
        wrefresh(args->left);
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
void delete(char* names,int Nnum){
        for(int i=0;i!=Nnum*38;i+=38){
                char buff[38];
                for(int ib=i,ic=0;ic<38;ib++,ic++){buff[ic]=names[ib];}
                if(str_check(buff,my_Name)==0){continue;}
                for(int im=i,ic=0;ic<38;im++,ic++){names[im]=names[im+38];}
        }
}
void pth_write(void* arg){
        struct mass* args=(struct mass*)arg;
        while(1){
                char buff[78];
                for(int i=0;i<78;i++){buff[i]='\0';}
		char buffw[118];
		for(int i=0;i<118;i++){buffw[i]='\0';}
		mvwgetstr(args->down,1,1,buffw);
		mvwprintw(args->down,1,1,PROB);
		wrefresh(args->down);
		for(int i=0;i<78;i++){buff[i]=buffw[i];}
                semop(ID3,&lock,2);
		int mnum=semctl(ID3,3,GETVAL);
                int Nnum=semctl(ID3,4,GETVAL);
                if(str_check(Ex,buff)==1){
                        semop(ID3,&dec4,1);
                        delete(args->names,Nnum);
                        pthread_exit(NULL);
                }
		if(str_check(Comm1,buff)==1){
			if(mnum>(30+30*ch_stat)){
				ch_stat++;
                        	output(args,Nnum,mnum);
			}
			semop(ID3,&unlock,1);
			continue;
		}
		if(str_check(Comm2,buff)==1){
			if(ch_stat!=0){
				ch_stat--;
                        	output(args,Nnum,mnum);
			}
                        semop(ID3,&unlock,1);
                        continue;
		}
		if(str_check(Comm3,buff)==1){
                        if(Nnum>(30+30*us_stat)){
				us_stat++;
	                        output(args,Nnum,mnum);
			}
                        semop(ID3,&unlock,1);
                        continue;
                }
		if(str_check(Comm4,buff)==1){
			if(us_stat!=0){
				us_stat--;
                        	output(args,Nnum,mnum);
			}
                        semop(ID3,&unlock,1);
                        continue;
		}
                char buff2[78];
                for(int i=0;i<78;i++){buff2[i]='\0';}
                int ib;
                for(ib=0;my_Name[ib]!='\0';ib++){buff2[ib]=my_Name[ib];}
                buff2[ib]=':';
                ib++;
                for(int i=0;ib<78;ib++,i++){buff2[ib]=buff[i];}
                for(int i=78*mnum,i2=0;i!=78*mnum+78;i++,i2++){
                        if(buff2[i2]!='\0'){args->messages[i]=buff2[i2];}
                        else{
                                for(;i!=78*mnum+77;i++){
                                        args->messages[i]='-';
                                }
                        }
                        if(i==(78*mnum+77)){
                                args->messages[i]='\0';
                                break;
                        }
                }
                semop(ID3,&qwe,2);
                semop(ID3,&unlock,1);
        }
}
void REG(struct mass* args){
        for(int i=0;i<38;i++){my_Name[i]='\0';}
	char buffw[38];
        for(int i=0;i<38;i++){buffw[i]='\0';}
        mvwgetstr(args->down,1,1,buffw);
        mvwprintw(args->down,1,1,PROB);
        wrefresh(args->down);
	for(int i=0;i<38;i++){my_Name[i]=buffw[i];}
        semop(ID3,&lock,2);
        int Nnum=semctl(ID3,4,GETVAL,0);
	if(Nnum==-1){exit(1);}
        for(int i=38*Nnum,i2=0;i!=38*Nnum+38;i++,i2++){
                if(my_Name[i2]!='\0'){args->names[i]=my_Name[i2];}
                else{
                        for(;i!=38*Nnum+37;i++){
                                args->names[i]='\0';
                        }
                }
                if(i==(38*Nnum+37)){
                        args->names[i]='\0';
                        break;
                }
        }
        semop(ID3,&v4unlock,3);
}
void pth_react(void* arg){
        struct mass* args=(struct mass*)arg;
        while(1){
                semop(ID3,&v2lock,3);
                int Nnum=semctl(ID3,4,GETVAL);
                int Mnum=semctl(ID3,3,GETVAL);
		output(args,Nnum,Mnum);
                semop(ID3,&unlock,1);
        }
}
int main(void){
        key_t key1=ftok("M2",1);
        ID1=shmget(key1,10000,0);
        char* messages=(char*)shmat(ID1,NULL,0);
        key_t key2=ftok("M2",2);
        ID2=shmget(key2,10000,0);
        char* names=(char*)shmat(ID2,NULL,0);
        key_t key3=ftok("M2",3);
        ID3=semget(key3,5,0);
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
	struct mass ar={.messages=messages,.names=names,.down=win1_down,.left=win2_left,.right=win3_right};
	//
        //struct mass ar={.messages=messages,.names=names};
        REG(&ar);
        pthread_create(&t_react,NULL,pth_react,&ar);
        pthread_create(&t_write,NULL,pth_write,&ar);
        pthread_join(t_write,NULL);
        pthread_cancel(t_react);
        pthread_join(t_react,NULL);
        semop(ID3,&v3unlock,1);
        semctl(ID3,0,SETVAL,0);
	endwin();
}

