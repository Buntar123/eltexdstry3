#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
struct node{
        char login[20];
        struct sockaddr_in client;
	struct sockaddr_in USR_client;
        struct node* next;
};
struct node* create_node(struct sockaddr_in client,struct sockaddr_in USR_client,char* login){
        struct node* new_node=(struct node*)malloc(sizeof(struct node));
        //strcpy(new_node->login,login);
	memcpy(new_node->login,login,20);
	memcpy(&new_node->client,&client,16);
	memcpy(&new_node->USR_client,&USR_client,16);
        //new_node->client=client;
	//new_node->USR_client=USR_client;
        new_node->next=NULL;
        return new_node;
}
struct node* addnode(struct node* head,struct sockaddr_in client,struct sockaddr_in USR_client,char* login){
        struct node* new_node=create_node(client,USR_client,login);
        if(head==NULL){
                head=new_node;
                head->next=head;
                return head;
        }
        struct node* tmp=head;
        while(tmp->next!=head){
                tmp=tmp->next;
        }
        tmp->next=new_node;
        new_node->next=head;
        return head;
}
int find_node(char* login,struct node* head){
        struct node* tmp=head;
        if(head==NULL){return 0;}
        if(tmp->next==head){
                if(strcmp(tmp->login,login)==0){
                        return 1;
                }
                return 0;
        }
        while(tmp->next!=head){
                if(strcmp(tmp->login,login)==0){
                        return 1;
                }
                tmp=tmp->next;
        }
        if(strcmp(tmp->login,login)==0){
                return 1;
        }
        return 0;
}
uint8_t rewrite(char* login,struct node* head,struct sockaddr_in client,struct sockaddr_in USR_client){
	struct node* tmp=head;
	if(tmp->next==head){
                memcpy(&tmp->client,&client,16);
        	memcpy(&tmp->USR_client,&USR_client,16);
		return 1;
        }
	while(tmp->next!=head){
                if(strcmp(tmp->login,login)==0){
			memcpy(&tmp->client,&client,16);
                	memcpy(&tmp->USR_client,&USR_client,16);
			return 1;
		}
                tmp=tmp->next;
        }
	if(strcmp(tmp->login,login)==0){
                memcpy(&tmp->client,&client,16);
                memcpy(&tmp->USR_client,&USR_client,16);
		return 1;
        }
	return 0;
}
void the_world(struct node* head){
        struct node* tmp=head;
        if(head==NULL){return 0;}
        if(tmp->next==head){printf("%s\n",tmp->login);return 0;}
        while(tmp->next!=head){
                printf("%s\n",tmp->login);
                tmp=tmp->next;
        }
        printf("%s\n",tmp->login);
}
uint8_t count_node(struct node* head){
	struct node* tmp=head;
	uint8_t count=0;
        if(head==NULL){return 0;}
        if(tmp->next==head){return 1;}
        while(tmp->next!=head){
                count++;
                tmp=tmp->next;
        }
        count++;
	return count;
}
uint8_t filling(char* send_buff,struct node* head){
	//незабывать про размеры,обработки переполнения пока нет
	uint8_t count=0;
	struct node* tmp=head;
	if(head==NULL){return 0;}
	if(tmp->next==head){
		memcpy(send_buff,tmp->login,20);
		printf("%s\n",(send_buff+(count*20)));
		return 1;
	}
	while(tmp->next!=head){
                memcpy((send_buff+(count*20)),tmp->login,20);
		printf("%s\n",(send_buff+(count*20)));
		count++;
                tmp=tmp->next;
        }
	memcpy(send_buff+count*20,tmp->login,20);
	printf("%s\n",(send_buff+(count*20)));
	count++;
	return count;
}
/*int main(void){
	char Login[20];
        sprintf(Login,"Billy Bones");
        struct sockaddr_in clt;
        clt.sin_family=AF_INET;
        clt.sin_port=htons(8080);
        inet_pton(AF_INET,"127.0.0.1",&clt.sin_addr);
        struct node* head;
        head=addnode(head,clt,clt,Login);
	head=addnode(head,clt,clt,"Jimm");
        head=addnode(head,clt,clt,"Albert");
	printf("%d\n",count_node(head));
	char send_buff[500];
	send_buff[0]=0;
	send_buff[1]=1;
	printf("%d\n",filling(send_buff+2,head));
	printf("%s\n",send_buff+2);
	printf("%s\n",send_buff+22);
	printf("%s\n",send_buff+42);
}*/
