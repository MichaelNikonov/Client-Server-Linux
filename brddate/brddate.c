#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "brd.h"


   int main(int argv,char **argc){
   int sfd,i,test;
   char *token;
   struct sockaddr_in s;
   struct msg_date date;
   struct msg_ent ent;
   struct msg_cnt cnt;

   if(argv>2){
   printf("Too much arguments\n");
   exit(1);
}

   if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1){//Create client socket.
   perror("sfd");
   exit(1);
}

   s.sin_family=AF_INET;
   s.sin_port=B_PORT_NUM;
   s.sin_addr.s_addr=inet_addr("127.0.0.1");
   if(connect(sfd,(struct sockaddr*)&s,sizeof(s))==-1){//Connect to server.
   perror("connect");
   exit(1);
}
//Initial date and send to server
  date.m_type=B_TYPE_DATE;
  token=strtok(argc[1],"/");
  date.m_start.day=atoi(token);
  token=strtok(NULL,"/");
  date.m_start.month=atoi(token); 
  token=strtok(NULL,"/");
  date.m_start.year=atoi(token);
  send(sfd,&date,sizeof(date),0);

  if((test=recv(sfd,&cnt,sizeof(cnt),0))>0){//Recieve number of adequate advertismentsand print then one by one.
  for(i=0;i<cnt.m_count-1;i++){//Recieve number ofadequate advertisments and print then one by one.
  if((test=recv(sfd,&ent,sizeof(ent),0))>0){
  if(ent.m_entry.b_date.day<10 &&ent.m_entry.b_date.month>=10)
  printf("0%d/%d/%d",ent.m_entry.b_date.day,ent.m_entry.b_date.month,ent.m_entry.b_date.year);
  if(ent.m_entry.b_date.month<10 &&ent.m_entry.b_date.day>=10)
  printf("%d/0%d/%d",ent.m_entry.b_date.day,ent.m_entry.b_date.month,ent.m_entry.b_date.year);
  if(ent.m_entry.b_date.day<10 &&ent.m_entry.b_date.month<10)
  printf("0%d/0%d/%d",ent.m_entry.b_date.day,ent.m_entry.b_date.month,ent.m_entry.b_date.year);
  else if(ent.m_entry.b_date.day>=10 &&ent.m_entry.b_date.month>=10)
  printf("%d/%d/%d",ent.m_entry.b_date.day,ent.m_entry.b_date.month,
  ent.m_entry.b_date.year);
  printf("%s\n",ent.m_entry.b_text);
}
}
}
close(sfd);//Close socket.
return 0;
} 
