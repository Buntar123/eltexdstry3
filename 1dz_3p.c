#include <stdio.h>
int main(void){
	int a,b;
	scanf("%d",&a);
	for(int i=32-1;i>=0;i--){

		if(1==((a>>i)&1)){
			b++;
		}
	}
	printf("%d",b);
	return 0; 
}