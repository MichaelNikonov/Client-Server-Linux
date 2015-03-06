#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "brd.h"


   int main(int argv,char **argc){
   int sfd,i;
   struct sockaddr_in s;
   struct msg_cat cat;
   struct msg_ent ent;
   struct msg_cnt cnt;
   if(argv>2){               //Cheak number of argument
   printf("Too much arguments\n");
   exit(1);
}

  if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1){     //Create client socket.
  perror("sfd");
  exit(1);
}

  s.sin_family=AF_INET;   //Initiale struct socckadder
  s.sin_port=B_PORT_NUM;
  s.sin_addr.s_addr=inet_addr("127.0.0.1");
  if(connect(sfd,(struct sockaddr*)&s,sizeof(s))==-1)
  {                                       //Connect to server.
  perror("connect");
  exit(1);
}

//Initial category and send to server
  cat.m_type=B_TYPE_CAT;
  strcpy(cat.m_cat,argc[1]);//copy the category
  cat.m_cat[strlen(argc[1])]=NULL; //Set NULL in the end of the string
  send(sfd,&cat,sizeof(cat),0);
  if(recv(sfd,&cnt,sizeof(cnt),0)>0){//Recieve number of adequat advertisments and print then one by one.
  for(i=0;i<cnt.m_count-1;i++){
  if((recv(sfd,&ent,sizeof(ent),MSG_WAITALL))>0)     //Recive one advertisment every iteration
  printf("%s %s\n",ent.m_entry.b_category,ent.m_entry.b_text);
}
}
close(sfd);//Close sccocket.
return 0;
}
