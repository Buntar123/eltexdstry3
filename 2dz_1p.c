#include <stdio.h>
#define N 3
int main(void){
	int A[N][N];
	int m=1;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
		A[i][j]=m;
		m++;
	        }
	}
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			printf("%d",A[i][j]);
			printf(" ");
	        }
	printf("\n");
	}
	return 0; 
}
