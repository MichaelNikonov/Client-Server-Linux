#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "brd.h"



   int main(){
   int fd,j,sfd,status,count=0;
   size_t nread;
   struct sockaddr_in s;
   struct msg_ent ent;
   struct msg_ok ok;
   char date[B_DATE_LEN],*token,category[B_CAT_LEN+1],ch;


   if((fd=open("classified.txt",0666))==-1){//Open text file.
      perror(fd);
      exit(1);
}

   while(1){
   if((nread=read(fd,date,sizeof(date)))>0){//Get the date from the text file.
   j=0;
   token=strtok(date,"/");
   ent.m_entry.b_date.day=atoi(token);
   token=strtok(NULL,"/");
   ent.m_entry.b_date.month=atoi(token);
   token=strtok(NULL,"/");
   ent.m_entry.b_date.year=atoi(token);

   do{//Get the free text from the text file.
   read(fd,&ch,sizeof(ch));
   ent.m_entry.b_category[j]=ch;
   j++;
   }while(ch!=' ' && j<19);

   ent.m_entry.b_category[j]=NULL;
   j=0;
   do{//Get the free text from the text file.
   read(fd,&ch,sizeof(ch));
   ent.m_entry.b_text[j]=ch;
   j++;
   }while(ch!='\n' && j<200);

   ent.m_entry.b_text[j]=NULL;
   ent.m_type=B_TYPE_ENT;
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

  send(sfd,(struct msg_ent *) &ent,sizeof(ent),MSG_DONTROUTE);
                     //Send server one entry.
  if((recv(sfd,&status,sizeof(int),0))>0){//Recieve ok message from server.
  if(status==B_TYPE_OK)//if flag==B_TYPE_OK.
  close(sfd);//Close socket.
}
  sleep(1);
}
else
  lseek(fd,0,0);
}
return 0;
}
