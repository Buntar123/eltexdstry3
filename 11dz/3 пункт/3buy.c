#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
struct store{
        int* prdct;
        int* check;
};
extern int y;
extern pthread_mutex_t m1;
void buy(void* arg){
        srand(time(NULL));
        struct store* st=(struct store*)arg;
        int need = rand()%(120000-80000+1)+80000;
        for(int i=0;i<5;i++){
                if(st->check[i]==0){
                        printf("|Thread %ld|Store %d|Went inside|\n",pthread_self(),i);
                        pthread_mutex_lock(&m1);
                        if(st->check[i]==0){
                                st->check[i]=1;
                                printf("|Thread %ld|Store %d|REALLY went inside|\n",pthread_self(),i);
                                pthread_mutex_unlock(&m1);
                                if(st->prdct[i]!=0){
                                        if(need>=st->prdct[i]){
                                                printf("|Thread %ld|Store %d|Give %d|Sleep|\n",pthread_self(),i,st->prdct[i]);
                                                need-=st->prdct[i];
                                                st->prdct[i]=0;
                                        }
                                        else{
                                                printf("|Thread %ld|Store %d|Give %d|Sleep|\n",pthread_self(),i,need);
                                                st->prdct[i]-=need;
                                                need=0;
                                        }
                                        st->check[i]=0;
                                        //sleep(2);
                                        sleep(3);
                                }
                                else{
                                        printf("|Thread %ld|Store %d|Store is empty|go next\n",pthread_self(),i);
                                        st->check[i]=0;
                                        sleep(1);
                                }
                        }
                        else{
                                pthread_mutex_unlock(&m1);
                                printf("|Thread %ld|Store %d|STORE IS CLOSE|GO AWAY|\n",pthread_self(),i);
				sleep(1);
                        }
                }
                else{
                        printf("|Thread %ld|Store %d|Store is closed|\n",pthread_self(),i);
                        sleep(1);
                }
                if(need==0){
                        printf("|Thread %ld|EXIT|\n",pthread_self());
                        break;
                }
                if(i==4){i=-1;}
        }
}

