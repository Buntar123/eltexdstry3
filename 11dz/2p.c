#include<stdio.h>
#include<pthread.h>
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
void increment(void* arg){
	//int a =*((int*)arg);
	for(int i=0;i<1000000;i++){
                pthread_mutex_lock(&m1);
		(*((int*)arg))++;
		pthread_mutex_unlock(&m1);
	}
}
void increment2(void* arg){
	//int a =*((int*)arg);
	int a=0;
	for(int i=0;i<1000000;i++){
		a++;
		if(a==1000){
			pthread_mutex_lock(&m1);
                	(*((int*)arg))+=1000;
                	pthread_mutex_unlock(&m1);
			a=0;
		}
        }
	/*
	pthread_mutex_lock(&m1);
        (*((int*)arg))+=a;
        pthread_mutex_unlock(&m1);
	*/
}
int main(void){
	int size=0;
	pthread_t T[10];
	for(int i=0;i<10;i++){
		pthread_create(&T[i],NULL,increment2,&size);
	}
	for(int i=0;i<10;i++){
                pthread_join(T[i],NULL);
        }
        printf("%d\n",size);	
}
