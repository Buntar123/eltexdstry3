#include<stdio.h>
#include<pthread.h>
void try(void* arg){
	pthread_t* Thr=(pthread_t*)arg;
	for(int i=0;i<5;i++){
		if(Thr[i]==pthread_self()){
			printf("Thread - %d\n",i);
			break;
		}
	}
	/*
	int* B = (int*)arg;
	printf("Thread - %d\n",*B);
	*/
}
int main(void){
	pthread_t pthreads[5];
	//int count[5];
	for(int i=0;i<5;i++){
		//count[i]=i;
		//pthread_create(&pthreads[i],NULL,try,(void*)&count[i]);
		pthread_create(&pthreads[i],NULL,try,(void*)pthreads);
	}
	for(int i=0;i<5;i++){
		pthread_join(pthreads[i],NULL);
	}
}
