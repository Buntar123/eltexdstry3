#include <stdio.h>
#define A 6
#define B 1
//я наполнял массивы числами для удобства
int main(void)
{
 int STRING[A];
 int subSTRING[B];
 for(int i=0;i<A;i++){
	 scanf("%d", &STRING[i]);
 }
 printf("---\n");
 for(int i=0;i<B;i++){
	 scanf("%d", &subSTRING[i]);
 }
 int *ptrSTR = &STRING;
 int *ptrsub = &subSTRING;
 int b = NULL;
 int *ptrres = &b;
 int count = 0,j;
 for(int i=0;i<A;i++){
	 j=0;
	 for(;j<B;j++){
		 if(*(ptrSTR+i)==*(ptrsub+j)){
			 i++;
		 }
		 else{j=-1;break;}
	 }
	 if(j==(B)){ptrres=(ptrSTR+i-1-B+1);break;}
 }
 printf("---\n");
 printf("%d\n",ptrres);
 printf("%d",*ptrres);
}