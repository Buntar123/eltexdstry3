#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
struct store{
        int* prdct;
        int* check;
};
int y=0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
extern void buy(void* arg);
extern void load(void* arg);
int main(void){
        srand(time(NULL));
        int store[5];
        int close[5];
        for(int i=0;i<5;i++){
                store[i]=rand()%(12000-8000+1)+(8000);
                close[i]=0;
        }
	/*
        int total=0;
        for(int i=0;i<5;i++){
                printf("%d\n",store[i]);
                total+=store[i];
        }
        printf("Total - %d\n",total);
        printf("-----\n");*/
        struct store st={.prdct=store, .check=close};
        pthread_t t1, t2,t3,t4,t5;
        pthread_create(&t2,NULL,buy,&st);
        pthread_create(&t1,NULL,buy,&st);
        pthread_create(&t3,NULL,buy,&st);
        //pthread_create(&t4,NULL,buy,&st);
        pthread_create(&t5,NULL,load,&st);
        printf("1 - %ld 2 - %ld 3 - %ld 4 - %ld 5 - %ld\n ",t1,t2,t3,t4,t5);
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);
        pthread_join(t3,NULL);
        //pthread_join(t4,NULL);
        y=1;
        pthread_join(t5,NULL);
}

