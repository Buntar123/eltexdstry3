#include <stdio.h>
#define A 100
#define B 10
//при вводе данных если ввести 0 то остальные ячейки массива например имени заполнятся нулями то есть после например ввода имени нужно ввести 0
struct abonent {
char name[B];
char second_name[B];
char tel[B];
};
void Input(char MassIn[B]){
	for(int j=0;j<B;j++){
		scanf(" %c",&MassIn[j]);
		if(MassIn[j]=='0'){
			for(;j<B;j++){
				MassIn[j]='0';
			}
			break;
		}
	}
}
void OutputSTRUCT(char MassIn1[B], char MassIn2[B], char MassIn3[B]){
	for(int j=0;j<B;j++){
		printf("%c",MassIn1[j]);
	}
	printf("\n");
	for(int j=0;j<B;j++){
		printf("%c",MassIn2[j]);
	}
	printf("\n");
	for(int j=0;j<B;j++){
		printf("%c",MassIn3[j]);
	}
	printf("\n---\n");
}
int main(void)
{
	struct abonent Handbook[100];
	for(int i=0;i<A;i++){
		Handbook[i].name[0]='0';
	}
	int countWH=1,countSWT;
	while(countWH==1){
		printf("command?\n");
		scanf("%d",&countSWT);
		switch(countSWT){
			case 1:
			    int countexam=1;
			    for(int i=0;i<A;i++){
					if(Handbook[i].name[0]=='0'){
						printf("name?\n");
						Input(Handbook[i].name);
						printf("second_name?\n");
						Input(Handbook[i].second_name);
						printf("tel?\n");
						Input(Handbook[i].tel);
						countexam=0;
						break;
					}
				}
				if(countexam==1){
					printf("Handbook is overflowing\n");
				}
			    break;
			case 2:
			    struct abonent delete_abonent;
			    printf("name?\n");
				Input(delete_abonent.name);
				printf("second_name?\n");
				Input(delete_abonent.second_name);
				printf("tel?\n");
				Input(delete_abonent.tel);
				for(int i=0;i<A;i++){
					if(Handbook[i].name[0]!='0'){
						int countfind=1;
						for(int j=0;j<B;j++){
							if(Handbook[i].name[j]!=delete_abonent.name[j]){countfind=0;break;}
							if(Handbook[i].second_name[j]!=delete_abonent.second_name[j]){countfind=0;break;}
							if(Handbook[i].tel[j]!=delete_abonent.tel[j]){countfind=0;break;}
						}
						if(countfind==1){
							for(int j=0;j<B;j++){
							    Handbook[i].name[j]='0';
								Handbook[i].second_name[j]='0';
								Handbook[i].tel[j]='0';
						    }
							break;
						}
					}
				}
			    break;
			case 3:
			    char find_name[10];
				printf("name?\n");
				Input(find_name);
			    for(int i=0;i<A;i++){
					if(Handbook[i].name[0]!='0'){
						int countfind=1;
						for(int j=0;j<B;j++){
							if(Handbook[i].name[j]!=find_name[j]){countfind=0;break;}
						}
						if(countfind==1){
							OutputSTRUCT(Handbook[i].name, Handbook[i].second_name, Handbook[i].tel);
						}
					}
				}
			    break;
			case 4:
			    for(int i=0;i<A;i++){
		            if(Handbook[i].name[0]!='0'){
						OutputSTRUCT(Handbook[i].name, Handbook[i].second_name, Handbook[i].tel);
					}
	            }
			    break;
			case 5:
			    countWH=0;
			    break;
			default:
			    printf("unknown\n");
		}
	}
}