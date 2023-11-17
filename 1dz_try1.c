#include <stdio.h>
int main(void){
	int a;
	scanf("%d",&a);
    
	//printf("%d\n",sizeof(a));
	//for(int i=0;i<9;i++){
		//printf("%d",(a&1));
		//a=a>>1;
	//
	for(int i=32-1;i>=0;i--){
		printf("%d",(a>>i)&1);
	}
	return 0; 
}