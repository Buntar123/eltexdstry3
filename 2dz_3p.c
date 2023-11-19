#include <stdio.h>
#define N 4
int main(void){
	int A[N][N];
    for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
		if(i<(N-1-j)){
			A[j][i]=0;
		}
		else{
			A[j][i]=1;
		}
	    }
	}
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			printf("%d",A[i][j]);
			printf(" ");
	    }
	printf("\n");
	}
}
