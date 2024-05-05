#include<stdio.h>
#include<dirent.h>
#include<malloc.h>
#include<sys/stat.h>
#include<time.h>
#define sz_dir_name 256
#define sz_item 66
#define sz_name 25
#define sz_type 3
#define sz_size 11
#define sz_data 24
void find_path2(struct dirent* entry,char* dirname, char* file_path){
        for(int i=0;i<sz_dir_name;i++){
                file_path[i]=dirname[i];
        }
        int i=0;
        char* filename=entry->d_name;
        if(file_path[1]=='\0'){}
        else{
                while(i<sz_dir_name && file_path[i]!='\0'){i++;}
                file_path[i]='/';
        }
        //perviy zvonochek
        i++;
        for(int j=0;filename[j]!='\0' && i<sz_dir_name;j++,i++){
                file_path[i]=filename[j];
        }
        for(;i<sz_dir_name;i++){
                file_path[i]='\0';
        }
}
int create_menu_item(struct dirent* entry,char* dirname,char* item_str){
        char file_path[sz_dir_name];
        char* filename = entry->d_name;
        for(int i=0;i<sz_name;i++){
                if(filename[i]!='\0'){
                        item_str[i]=filename[i];
                }
                else{
                        for(;i<sz_name;i++){
                                item_str[i]=' ';
                        }
                }
        }
        item_str[25]='|';
        switch(entry->d_type){
                case DT_BLK:
                        item_str[26]='B';
                        item_str[27]='L';
                        item_str[28]='K';
                        break;
                case DT_CHR:
                        item_str[26]='C';
                        item_str[27]='H';
                        item_str[28]='R';
                        break;
                case DT_DIR:
                        item_str[26]='D';
                        item_str[27]='I';
                        item_str[28]='R';
                        break;
                case DT_FIFO:
                        item_str[26]='F';
                        item_str[27]='I';
                        item_str[28]='F';
                        break;
                case DT_LNK:
                        item_str[26]='L';
                        item_str[27]='N';
                        item_str[28]='K';
                        break;
                case DT_REG:
                        item_str[26]='R';
                        item_str[27]='E';
                        item_str[28]='G';
                        break;
                case DT_SOCK:
                        item_str[26]='S';
                        item_str[27]='O';
                        item_str[28]='C';
                        break;
                case DT_UNKNOWN:
                        item_str[26]='U';
                        item_str[27]='N';
                        item_str[28]='K';
                        break;
                default:
                        item_str[26]='?';
                        item_str[27]='?';
                        item_str[28]='?';
                        break;
        }
        item_str[29]='|';
        struct stat st_file_inf;
        find_path2(entry,dirname,file_path);
        int st_ret=stat(file_path,&st_file_inf);
        if(st_ret!=0){
                for(int i=30;i<41;i++){
                        item_str[i]='-';
                }
		item_str[41]='|';
		for(int i=42;i<66;i++){
                        item_str[i]='-';
                }
		return 0;
        }
	else{
                int sz=st_file_inf.st_size;
                char s_size[100];//
		sprintf(s_size,"%d",sz);
		int p=0;
		for(;p<100 && s_size[p]!='\0';p++){}
		if(p>11){
			item_str[30]='*';
			item_str[31]='*';
			item_str[32]='*';
			item_str[33]='>';
			item_str[34]='9';
			item_str[35]='3';
			item_str[36]='G';
			item_str[37]='B';
			item_str[38]='*';
			item_str[39]='*';
			item_str[40]='*';
			item_str[41]='|';
		}
		else{
			for(int i=30;i<41;i++){
                                if(s_size[i-30]!='\0'){
                                        item_str[i]=s_size[i-30];
                                }
                                else{
                                        for(;i<41;i++){
                                                item_str[i]=' ';
                                        }
                                }
			}
			item_str[41]='|';
                }
		for(int i=42;i<66;i++){
                        item_str[i]=ctime(&st_file_inf.st_mtime)[i-42];
                }
                return 1;
	}
}
/*
int main(void){
        DIR* dir;
        struct dirent* entry;
        char dirname[256];
        dirname[0]='/';
        //dirname[1]='r';
        //dirname[2]='o';
        //dirname[3]='o';
        //dirname[4]='t';
        dirname[1]='e';
        dirname[2]='t';
        dirname[3]='c';
        dir=opendir(dirname);
        if(dir==NULL){printf("opendir\n");exit(1);}
        while((entry=readdir(dir))!=NULL){
                char item_str[67];
                int a=create_menu_item(entry,dirname,item_str);
                printf("| %s |\n ",item_str);
		item_str[66]='\0';
//		if(item_str[66]=='\0'){
//                      printf("| %s | %c\n",item_str,'y');
//		}
//		else{
//			printf("| %s |\n",item_str);
//		}
        }

}
*/
