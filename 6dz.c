#include <stdio.h>
#include <malloc.h>
#define B 10
//в задании про функции вы говорили что функция аналогичная mass_filling можно использовать для переполнения буфера из-за считывания
//в массив ограниченной длины потока ввода scanf, а как можно избежать подобного, не вынося вывод из функции в main и сохранить scanf?
struct abonent {
char name[B];
char second_name[B];
char tel[B];
};
int struc_equal(struct abonent Fir,struct abonent Sec){
	int count_equal=1;
	for(int i=0;i<B;i++){
		if(Fir.name[i]!=Sec.name[i]){count_equal=0;break;}
		if(Fir.second_name[i]!=Sec.second_name[i]){count_equal=0;break;}
		if(Fir.tel[i]!=Sec.tel[i]){count_equal=0;break;}
	}
	if(count_equal==0){
		return 0;
	}
	else{return 1;}
}
void mass_filling(char A[B]){
	for(int i=0;i<B;i++){
		scanf(" %c",&A[i]);
		if(A[i]=='0'){
			for(;i<B;i++){
				A[i]='0';
			}
			break;
		}
	}
}
void stuct_show(struct abonent A){
	printf("-----\n");
	for(int j=0;j<B;j++){
		 printf("%c",A.name[j]);
	}
	printf("\n");
	for(int j=0;j<B;j++){
		printf("%c",A.second_name[j]);
	}
	printf("\n");
	for(int j=0;j<B;j++){
		printf("%c",A.tel[j]);
	}
	printf("\n-----\n");
}
int main(void){
	struct abonent* A = NULL;
	struct abonent* C;
	int n=0, count_empty_after_del = 0;
	int count_cycle = 0;
	while(count_cycle!=5){
		printf("command?\n");
		if(count_empty_after_del==1){
			count_cycle=1;
			count_empty_after_del = 0;
			printf("add new item, massive is empty\n");
		}
		else{scanf("%d",&count_cycle);}
		switch(count_cycle){
			case 1:
			    printf("add\n");
				n++;
				if(n==1){
					printf("use_malloc\n");
					C=(struct abonent*)malloc(sizeof(struct abonent));
					if(C==NULL){printf("error_1");count_cycle=5;break;}
					A=C;
				}
				else{
					printf("use_realloc\n");
					if(A==NULL){printf("error_3");count_cycle=5;break;}
					C=(struct abonent*)realloc(A,n*sizeof(struct abonent));
					if(C==NULL){printf("error_1");count_cycle=5;break;}
					if(C!=A && A!=NULL){printf("error_2\n");}
					A=C;
				}
				printf("name?\n");
				mass_filling(A[n-1].name);
				printf("second_name?\n");
				mass_filling(A[n-1].second_name);
				printf("tel?\n");
				mass_filling(A[n-1].tel);
				break;
			case 2:
			    printf("delete\n");
				int del_num=-1;
				struct abonent *D=NULL;
				D =(struct abonent*)malloc(sizeof(struct abonent));
				if(D==NULL){printf("error_1");count_cycle=5;break;}
				printf("name?\n");
				mass_filling(D[0].name);
				printf("second_name?\n");
				mass_filling(D[0].second_name);
				printf("tel?\n");
				mass_filling(D[0].tel);
				for(int i=0;i<n;i++){
					if(struc_equal(A[i],D[0])==1){
						del_num=i;
						break;
					}
				}
				if(del_num==-1){printf("error_not_found\n");free(D);D=NULL;break;}
				free(D);
				D=NULL;
				A[del_num]=A[n-1];
				C =(struct abonent*)realloc(A,(n-1)*sizeof(struct abonent));
				if(C==NULL && n==1){A=C;n--;count_empty_after_del=1;break;}
				if(C==NULL){printf("error_1");count_cycle=5;break;}
				if(C!=A && A!=NULL){printf("error_2");}
				A=C;
				n--;
				break;
			case 3:
			    printf("find\n");
				int del_find;
				scanf("%d",&del_find);
				char *E = NULL;
				E=(char*)malloc(B*sizeof(char));
				if(E==NULL){printf("error_1");count_cycle=5;break;}
				printf("name?\n");
				mass_filling(E);
				for(int i=0;i<n;i++){
					int count_delfin=1;
					for(int j=0;j<B;j++){
						if(A[i].name[j]!=E[j]){
							count_delfin=0;
							break;
						}
					}
					if(count_delfin==1){
						stuct_show(A[i]);
					}
				}
				free(E);
				E=NULL;
				break;
			case 4:
			    printf("show all\n");
				for(int i=0;i<n;i++){
					stuct_show(A[i]);
				}
				break;
			case 5:
			    printf("exit\n");
				free(A);
				count_cycle=5;
				break;
		}
	}
}