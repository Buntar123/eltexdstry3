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
void load(void* arg){
        srand(time(NULL));
        struct store* st=(struct store*)arg;
        for(int i=0;i<5;i++){
                if(st->check[i]==0){
                        pthread_mutex_lock(&m1);
                        if(st->check[i]==0){
                                st->check[i]=1;
                                pthread_mutex_unlock(&m1);
                                st->prdct[i]+=10000;
                                printf("|Loader %ld|Store %d|I load 10000|\n",pthread_self(),i);
                                st->check[i]=0;
                                //sleep(1);
                                sleep(2);
                        }
                        else{
                                pthread_mutex_unlock(&m1);
                                printf("|Loader %ld|Store %d|STORE IS CLOSE|\n",pthread_self(),i);
                        }
                }
                else{
                        printf("|Loader %ld|Store %d|Store is closed|\n",pthread_self(),i);
                }
                if(y==1){
                        printf("|Loader %ld|EXIT|\n",pthread_self());
                        break;
                }
                if(i==4){i=-1;}
        }
}

