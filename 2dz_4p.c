#include <stdio.h>
#define N 10
int main(void){
	int A[N][N];
	int t=1;
	int s=1;
	int i=0,j=0,y=1;
	while(s<=(N*N)){
		switch(t){
			case 1:{
				A[j][i]=s;
				s++;
				if((N-y)==i){
					t=2;
					j++;
				}
				else{
					i++;
				}
				break;
			}
			case 2:{
				A[j][i]=s;
				s++;
				if((N-y)==j){
					t=3;
					i--;
				}
				else{
					j++;
				}
				break;
			}
			case 3:{
				A[j][i]=s;
				s++;
				if((y-1)==i){
					t=4;
					j--;
					y++;
				}
				else{
					i--;
				}
				break;
			}
			case 4:{
				A[j][i]=s;
				s++;
				if((y-1)==j){
					t=1;
					i++;
				}
				else{
					j--;
				}
				break;
			}
		}
	}
	for(int g=0;g<N;g++){
		for(int h=0;h<N;h++){
			printf("%d",A[g][h]);
			printf(" ");
	    }
	    printf("\n");
	}
}