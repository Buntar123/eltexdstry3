#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<menu.h>
#include<malloc.h>
#include<dirent.h>
#include<sys/stat.h>
struct file_inf{
        int count;
        int max_block;
        int block;
};
struct interface{
        WINDOW* win;
        MENU* menu;
        char** dynmass;
        ITEM** items;
};
int count_file(char* dirname,struct file_inf* file){
        file->count=0;
        DIR* dir2;
        struct dirent* ent;
        dir2=opendir(dirname);
        if(dir2==NULL){return 0;}
        while((ent=readdir(dir2))!=NULL){file->count++;}
        closedir(dir2);
        file->max_block=(file->count/32)+(file->count%32!=0);
        file->max_block--;
        file->block=0;
        return 1;
}
void find_path2(struct dirent* entry,char* dirname, char* file_path);
int create_menu_item(struct dirent* entry,char* dirname,char* item_str);
int main(void){
        DIR* dir;
        char Empty[67];
        for(int i=0;i<66;i++){
                if(i==25 || i==29 || i==41){Empty[i]='|';}
                else{Empty[i]=' ';}
        }
        Empty[66]='\0';
        struct interface right3;
        struct interface left2;
        struct dirent* entry;
        char dirname[256];
        char dirname2[256];
        dirname[0]='/';
	dirname[1]='\0';
        dirname2[0]='/';
	dirname2[1]='\0';
        initscr();
        cbreak();
        noecho();
        keypad(stdscr,TRUE);
        //1-window
        int he_1=10,wi_1=139,sty_1=0,stx_1=1;
        int he_2=34, wi_2=69, sty_2=10, stx_2=1;
        int he_3=34, wi_3=69, sty_3=10, stx_3=71;
        WINDOW* win1=newwin(he_1,wi_1,sty_1,stx_1);
        left2.win=newwin(he_2,wi_2,sty_2,stx_2);
        right3.win=newwin(he_3,wi_3,sty_3,stx_3);
        box(win1,0,0);
        box(left2.win,0,0);
        box(right3.win,0,0);
        refresh();
        wrefresh(win1);
        wrefresh(left2.win);
        wrefresh(right3.win);
        //2-file_count+
        struct file_inf file;
        struct file_inf file2;
        int b=count_file(dirname,&file);
        if(b==0){endwin();printf("count_file_1\n");exit(1);}
        b=count_file(dirname2,&file2);
        if(b==0){endwin();printf("count_file_2\n");exit(1);}
        //3-dyn mass
        left2.dynmass=malloc((file.count)*sizeof(char*));
        if(left2.dynmass==NULL){endwin();printf("malloc_1\n");exit(1);}
        right3.dynmass=malloc((file.count)*sizeof(char*));
        if(right3.dynmass==NULL){endwin();printf("malloc_1\n");exit(1);}
        for(int i=0;i<file.count;i++){
                left2.dynmass[i]=malloc(67*sizeof(char));
                if(left2.dynmass[i]==NULL){endwin();printf("malloc_2\n");exit(1);}
                right3.dynmass[i]=malloc(67*sizeof(char));
                if(right3.dynmass[i]==NULL){endwin();printf("malloc_2\n");exit(1);}
        }
        dir=opendir(dirname);
        if(dir==NULL){endwin();printf("opendir_1\n");exit(1);}
        for(int i=0;((entry=readdir(dir))!=NULL);i++){
                char itemstr[67];
                int a=create_menu_item(entry,dirname,itemstr);
                itemstr[66]='\0';
                for(int j=0;j<67;j++){
                        left2.dynmass[i][j]=itemstr[j];
                }
                for(int j=0;j<67;j++){
                        right3.dynmass[i][j]=itemstr[j];
                }
        }
        closedir(dir);
        //4-menu
        left2.items=(ITEM**)malloc(33*sizeof(ITEM*));
        if(left2.items==NULL){endwin();printf("malloc_3\n");exit(1);}
        right3.items=(ITEM**)malloc(33*sizeof(ITEM*));
        if(right3.items==NULL){endwin();printf("malloc_3\n");exit(1);}
        int menu_count=0;
        for(int i=0;i<32 && i<file.count;i++,menu_count++){
                left2.items[i]=new_item(left2.dynmass[i],NULL);
                if(left2.items[i]==NULL){endwin();printf("new_item_1\n");exit(1);}
                right3.items[i]=new_item(right3.dynmass[i],NULL);
                if(right3.items[i]==NULL){endwin();printf("new_item_1\n");exit(1);}
        }
        if(menu_count<32){
                for(;menu_count<32;menu_count++){
                        left2.items[menu_count]=new_item(Empty,NULL);
                        if(left2.items[menu_count]==NULL){endwin();printf("new_item_1\n");exit(1);}
                        right3.items[menu_count]=new_item(Empty,NULL);
                        if(right3.items[menu_count]==NULL){endwin();printf("new_item_1\n");exit(1);}
                }
        }
        left2.items[menu_count]=(ITEM*)NULL;
        left2.menu=new_menu((ITEM**)left2.items);
        set_menu_win(left2.menu,left2.win);
        set_menu_sub(left2.menu,derwin(left2.win,he_2-2,wi_2-2,1,1));
        set_menu_format(left2.menu,32,1);
        post_menu(left2.menu);
        right3.items[menu_count]=(ITEM*)NULL;
        right3.menu=new_menu((ITEM**)right3.items);
        set_menu_win(right3.menu,right3.win);
        set_menu_sub(right3.menu,derwin(right3.win,he_2-2,wi_2-2,1,1));
        set_menu_format(right3.menu,32,1);
        post_menu(right3.menu);
        refresh();
        wrefresh(left2.win);
        wrefresh(right3.win);
        refresh();
        //5-switch
        int c;
        char* cur_dirname;
        cur_dirname=dirname;
        struct interface* curr;
        curr=&left2;
        struct file_inf* cur_file;
        cur_file=&file;
        int win_count=2;
        while((c=getch())!=KEY_F(1)){
                int cur_index;
                switch(c){
                        case (char)9:
                                if(win_count==2){
                                        win_count=3;
                                        curr=&right3;
                                        cur_file=&file2;
                                        cur_dirname=dirname2;
                                }
                                else{
                                        win_count=2;
                                        curr=&left2;
                                        cur_file=&file;
                                        cur_dirname=dirname;
                                }
                                break;
                        case KEY_UP:
                                cur_index=item_index(current_item(curr->menu));
                                if(cur_file->block==0 && cur_index==0){break;}
                                if(cur_index>0){
                                        menu_driver(curr->menu,REQ_UP_ITEM);
                                        break;
                                }
                                cur_file->block--;
                                for(int i=0;i<33;i++){
                                        free_item(curr->items[i]);
                                }
                                free_menu(curr->menu);
                                unpost_menu(curr->menu);
                                menu_count=0;
                                for(int i=32*cur_file->block;i<32*cur_file->block+32 && i<cur_file->count;i++,menu_count++){}
                                for(int i=0;i<menu_count;i++){
                                          curr->items[i]=new_item(curr->dynmass[i+32*cur_file->block],NULL);
                                          if(curr->items[i]==NULL){endwin();printf("malloc_m_s %d\n",i);exit(1);}
                                }
                                if(menu_count<32){
                                        for(;menu_count<cur_file->count;menu_count++){
                                                curr->items[menu_count]=new_item(Empty,NULL);
                                                if(curr->items[menu_count]==NULL){endwin();printf("new_item_1\n");exit(1);}
                                        }
                                }
                                curr->items[menu_count]=(ITEM*)NULL;
                                curr->menu = new_menu((ITEM**)curr->items);
                                set_menu_win(curr->menu,curr->win);
                                set_menu_sub(curr->menu,derwin(curr->win,he_2-2,wi_2-2,1,1));
                                set_menu_format(curr->menu,32,1);
                                post_menu(curr->menu);
                                menu_driver(curr->menu,REQ_LAST_ITEM);
                                refresh();
                                break;
                        case KEY_DOWN:
                                cur_index=item_index(current_item(curr->menu));
                                if(cur_file->block==cur_file->max_block && cur_index==31){break;}
                                if(cur_index<31){
                                        menu_driver(curr->menu,REQ_DOWN_ITEM);
                                        break;
                                }
                                cur_file->block++;
                                for(int i=0;i<33;i++){
                                        free_item(curr->items[i]);
                                }
                                free_menu(curr->menu);
                                unpost_menu(curr->menu);
                                menu_count=0;
                                for(int i=32*cur_file->block;i<32*cur_file->block+32 && i<cur_file->count;i++,menu_count++){}
                                for(int i=0;i<menu_count;i++){
                                          curr->items[i]=new_item(curr->dynmass[i+32*cur_file->block],NULL);
                                          if(curr->items[i]==NULL){endwin();printf("malloc_m_s %d\n",i);exit(1);}
                                }
                                if(menu_count<32){
                                        for(;menu_count<cur_file->count;menu_count++){
                                                curr->items[menu_count]=new_item(Empty,NULL);
                                                if(curr->items[menu_count]==NULL){endwin();printf("new_item_1\n");exit(1);}
                                        }
                                }
                                curr->items[menu_count]=(ITEM*)NULL;
                                curr->menu = new_menu((ITEM**)curr->items);
                                set_menu_win(curr->menu,curr->win);
                                set_menu_sub(curr->menu,derwin(curr->win,he_2-2,wi_2-2,1,1));
                                set_menu_format(curr->menu,32,1);
                                post_menu(curr->menu);
                                menu_driver(curr->menu,REQ_FIRST_ITEM);
                                refresh();
                                break;
                        case '\n':
                                cur_index=item_index(current_item(curr->menu));
                                char D,I,R;
                                D=curr->dynmass[cur_index][26];
                                I=curr->dynmass[cur_index][27];
                                R=curr->dynmass[cur_index][28];
                                if(D!='D' || I!='I' || R!='R'){break;}
                                if(curr->dynmass[cur_index][0]=='.' && curr->dynmass[cur_index][1]==' '){
                                        menu_driver(curr->menu,REQ_FIRST_ITEM);
                                        break;
                                }
                                char file_path[256];
                                if(curr->dynmass[cur_index][0]=='.' && curr->dynmass[cur_index][1]=='.' && curr->dynmass[cur_index][2]==' '){
                                        if(cur_dirname[1]=='\0'){
                                                menu_driver(curr->menu,REQ_FIRST_ITEM);
                                                break;
                                        }
                                        int p_count=0;
                                        for(int i=0;i<256;i++){file_path[i]=cur_dirname[i];}
                                        for(;file_path[p_count]!='\0' && p_count<256;p_count++){}
                                        for(;file_path[p_count]!='/';p_count--){
                                                file_path[p_count]='\0';
                                        }
                                        if(p_count==0){}
                                        else{
                                                file_path[p_count]='\0';
                                        }
                                }
                                else{
                                        dir=opendir(cur_dirname);
                                        if(dir==NULL){printf("opendir\n");exit(1);}
                                        for(int i=0;(entry=readdir(dir))!=NULL;i++){
                                                if(i==cur_index){
                                                        find_path2(entry,cur_dirname,file_path);
                                                }
                                        }
                                        closedir(dir);
				}
                                dir=opendir(file_path);
                                if(dir==NULL){break;}
                                closedir(dir);
                                for(int i=0;i<256;i++){
                                        cur_dirname[i]=file_path[i];
                                }
                                for(int i=0;i<33;i++){
                                        free_item(curr->items[i]);
                                }
                                free_menu(curr->menu);
                                unpost_menu(curr->menu);
                                free(curr->items);
                                for(int i=0;i<cur_file->count;i++){
                                        free(curr->dynmass[i]);
                                }
                                free(curr->dynmass);
                                b=count_file(cur_dirname,cur_file);
                                if(b==0){endwin();printf("count_file_3\n");exit(1);}
                                curr->dynmass=malloc((cur_file->count)*sizeof(char*));
                                if(curr->dynmass==NULL){endwin();printf("malloc_1\n");exit(1);}
                                for(int i=0;i<cur_file->count;i++){
                                        curr->dynmass[i]=malloc(67*sizeof(char));
                                        if(curr->dynmass[i]==NULL){endwin();printf("malloc_2\n");exit(1);}
                                }
                                dir=opendir(cur_dirname);
                                if(dir==NULL){endwin();printf("opendir_1\n");exit(1);}
                                for(int i=0;((entry=readdir(dir))!=NULL);i++){
                                        char itemstr[67];
                                        int a=create_menu_item(entry,cur_dirname,itemstr);
                                        itemstr[66]='\0';
                                        for(int j=0;j<67;j++){
                                                curr->dynmass[i][j]=itemstr[j];
                                        }
                                }
                                closedir(dir);
                                curr->items=(ITEM**)malloc(33*sizeof(ITEM*));
                                if(curr->items==NULL){endwin();printf("malloc_3\n");exit(1);}
                                int menu_count=0;
                                for(int i=0;i<32 && i<cur_file->count;i++,menu_count++){
                                        curr->items[i]=new_item(curr->dynmass[i],NULL);
                                        if(curr->items[i]==NULL){endwin();printf("new_item_1\n");exit(1);}
                                }
                                if(menu_count<32){
                                        for(;menu_count<32;menu_count++){
                                                curr->items[menu_count]=new_item(Empty,NULL);
                                                if(curr->items[menu_count]==NULL){endwin();printf("new_item_1\n");exit(1);}
                                        }
                                }
                                curr->items[menu_count]=(ITEM*)NULL;
                                curr->menu=new_menu((ITEM**)curr->items);
                                set_menu_win(curr->menu,curr->win);
                                set_menu_sub(curr->menu,derwin(curr->win,he_2-2,wi_2-2,1,1));
                                set_menu_format(curr->menu,32,1);
                                post_menu(curr->menu);
                                refresh();
                                break;
                }
                wrefresh(curr->win);
        }
        for(int i=0;i<33;i++){
                free_item(left2.items[i]);
                free_item(right3.items[i]);
        }
        unpost_menu(left2.menu);
        unpost_menu(right3.menu);
        free_menu(left2.menu);
        free_menu(right3.menu);
        free(left2.items);
        free(right3.items);
        for(int i=0;i<file.count;i++){
                free(left2.dynmass[i]);
        }
        free(left2.dynmass);
        for(int i=0;i<file2.count;i++){
                free(right3.dynmass[i]);
        }
        free(right3.dynmass);
        delwin(left2.win);
        delwin(right3.win);
        delwin(win1);
        endwin();
}
