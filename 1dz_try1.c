#include <stdio.h>
int main(void){
	int a;
	scanf("%d",&a);
	for(int i=32-1;i>=0;i--){
		printf("%d",(a>>i)&1);
	}
	return 0; 
}
