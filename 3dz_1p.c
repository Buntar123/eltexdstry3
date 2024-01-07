#include <stdio.h> 
int main(void){
	int vvod;
	scanf("%d",&a);
	int change;
	scanf("%d",&b);
	char *ptr;
	ptr = &vvod;
	ptr+=2;
	printf("3th byte before %d\n",*ptr);
	*ptr=change;
	printf("3th byte after %d\n",*ptr);
	printf("a after %d",vvod);
}