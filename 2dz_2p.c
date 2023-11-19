#include <stdio.h>
#define N 5
int main(void){
	int A[N];
	int m=1;
	int t;
	for(int i=0;i<N;i++){
		A[i]=m;
		m++;
	}
	for(int i=0;i<N;i++){
		if(i<=((N-1)/2)){
			t=A[i];
		    A[i]=A[N-1-i];
		    A[N-1-i]=t;
		}
		printf("%d",A[i]);
		printf(" ");
	}
	return 0; 
}