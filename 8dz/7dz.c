#include<stdio.h>
#include"7_2dz.h"
int main(void){
	int count_cycle=0;
	int first,second;
	while(count_cycle!=5){
		printf("command?\n");
		scanf("%d",&count_cycle);
		switch(count_cycle){
			case 1:
				scanf("%d",&first);
				scanf("%d",&second);
				printf("%d\n",add(first,second));
				break;
			case 2:
				scanf("%d",&first);
                                scanf("%d",&second);
                                printf("%d\n",sub(first,second));
                                break;
			case 3:
			        scanf("%d",&first);
                                scanf("%d",&second);
                                printf("%d\n",mul(first,second));
                                break;
			case 4:
				scanf("%d",&first);
                                scanf("%d",&second);
                                printf("%d\n",div2(first,second));
                                break;
			case 5:
				count_cycle = 5;
				break;
		}
	}
}
