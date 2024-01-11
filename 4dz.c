#include <stdio.h>
#define A 100
#define B 10
//при вводе данных если ввести 0 то остальные ячейки массива например имени заполнятся нулями то есть после например ввода имени нужно ввести 0
//с функциями это было бы проще написать, но я их специально не использовал, надеюсь не зря
struct abonent {
char name[B];
char second_name[B];
char tel[B];
};
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
						for(int j=0;j<B;j++){
							scanf(" %c",&Handbook[i].name[j]);
							if(Handbook[i].name[j]=='0'){
								for(;j<B;j++){
									Handbook[i].name[j]='0';
								}
								break;
							}
						}
						printf("second_name?\n");
						for(int j=0;j<B;j++){
							scanf(" %c",&Handbook[i].second_name[j]);
							if(Handbook[i].second_name[j]=='0'){
								for(;j<B;j++){
									Handbook[i].second_name[j]='0';
								}
								break;
							}
						}
						printf("tel?\n");
						for(int j=0;j<B;j++){
							scanf(" %c",&Handbook[i].tel[j]);
							if(Handbook[i].tel[j]=='0'){
								for(;j<B;j++){
									Handbook[i].tel[j]='0';
								}
								break;
							}
						}
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
			    for(int j=0;j<B;j++){
					scanf(" %c",&delete_abonent.name[j]);
					if(delete_abonent.name[j]=='0'){
						for(;j<B;j++){
							delete_abonent.name[j]='0';
						}
						break;
					}
				}
				printf("second_name?\n");
				for(int j=0;j<B;j++){
					scanf(" %c",&delete_abonent.second_name[j]);
					if(delete_abonent.second_name[j]=='0'){
						for(;j<B;j++){
							delete_abonent.second_name[j]='0';
						}
						break;
					}
				}
				printf("tel?\n");
				for(int j=0;j<B;j++){
					scanf(" %c",&delete_abonent.tel[j]);
					if(delete_abonent.tel[j]=='0'){
						for(;j<B;j++){
							delete_abonent.tel[j]='0';
						}
						break;
					}
				}
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
			    for(int j=0;j<B;j++){
					scanf(" %c",&find_name[j]);
					if(find_name[j]=='0'){
						for(;j<B;j++){
							find_name[j]='0';
						}
						break;
					}
				}
			    for(int i=0;i<A;i++){
					if(Handbook[i].name[0]!='0'){
						int countfind=1;
						for(int j=0;j<B;j++){
							if(Handbook[i].name[j]!=find_name[j]){countfind=0;break;}
						}
						if(countfind==1){
							for(int j=0;j<B;j++){
							    printf("%c",Handbook[i].name[j]);
						    }
						    printf("\n");
						    for(int j=0;j<B;j++){
							    printf("%c",Handbook[i].second_name[j]);
						    }
						    printf("\n");
						    for(int j=0;j<B;j++){
							    printf("%c",Handbook[i].tel[j]);
						    }
						    printf("\n---\n");
						}
					}
				}
			    break;
			case 4:
			    for(int i=0;i<A;i++){
		            if(Handbook[i].name[0]!='0'){
						for(int j=0;j<B;j++){
							printf("%c",Handbook[i].name[j]);
						}
						printf("\n");
						for(int j=0;j<B;j++){
							printf("%c",Handbook[i].second_name[j]);
						}
						printf("\n");
						for(int j=0;j<B;j++){
							printf("%c",Handbook[i].tel[j]);
						}
						printf("\n---\n");
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