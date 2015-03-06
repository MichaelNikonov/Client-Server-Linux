#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "brd.h"

int main(int argv,char *argc){
 int sfd,rfd,i,j;
 struct sockaddr_in s;
 struct sockaddr *adder;
 struct board_ent board[B_ENT_CNT];
 struct msg_ent toSend[B_ENT_CNT];
 struct msg_ent toSend2[B_ENT_CNT];
 struct msg_ent newMsg;
 struct msg_cat catMsg;
 struct msg_date dateMsg;
 struct msg_ok msgOK;
 struct msg_cnt numOfCatMsg,numOfDateMsg;
 int type;
 int globalIndex = -1;

   if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1){//Create server socket.
   perror("sfd");
   exit(1);
}

  s.sin_family=AF_INET;   //Initiale struct socckadder
  s.sin_port=B_PORT_NUM;
  s.sin_addr.s_addr=inet_addr("127.0.0.1");
  if(bind(sfd,(struct sockaddr*)&s,sizeof(s))==-1)
  {
  perror("bind");
  exit(1);
}
msgOK.m_type = B_TYPE_OK;
numOfCatMsg.m_type =B_TYPE_CNT;
numOfCatMsg.m_count=0;
numOfDateMsg.m_count = 0;
numOfDateMsg.m_type = B_TYPE_CNT;
listen (sfd,5);
rfd=accept(sfd,(struct sockaddr*)&adder,sizeof(adder));
while (1)
{
rfd=accept(sfd,NULL,NULL);
if(rfd==-1){
    perror("rfd");
    return EXIT_FAILURE;
}
 else{
        if(recv(rfd,&type,sizeof(type),MSG_PEEK)==-1){
            perror("recieve error\n");
            return EXIT_FAILURE;
        }
        else{
                switch(type)
                {
                    case B_TYPE_ENT:recv(rfd,&newMsg,sizeof(newMsg),MSG_WAITALL);
                                    strcpy(board[(++globalIndex)%B_ENT_CNT].b_category,newMsg.m_entry.b_category);
                                    strcpy(board[globalIndex%B_ENT_CNT].b_text,newMsg.m_entry.b_text);
                                    board[globalIndex%B_ENT_CNT].b_date = newMsg.m_entry.b_date;
                                    printf("%d : %d/%d/%d %s %s \n",globalIndex%B_ENT_CNT,
                                                                    board[globalIndex%B_ENT_CNT].b_date.day,
                                                                    board[globalIndex%B_ENT_CNT].b_date.month,
                                                                    board[globalIndex%B_ENT_CNT].b_date.year,
                                                                    board[globalIndex%B_ENT_CNT].b_category,
                                                                    board[globalIndex%B_ENT_CNT].b_text);
                                    send(rfd,(struct msg_ok *)&msgOK,sizeof(msgOK),MSG_DONTROUTE);
                                    break;
                    case B_TYPE_CAT:recv(rfd,&catMsg,sizeof(catMsg),MSG_WAITALL);
                                    strcat(catMsg.m_cat," \0");
                                    if(fork()==0){
                                       numOfCatMsg.m_count = 0;
                                       for(i=0;i<B_ENT_CNT;i++)
                                         if(!strcmp(board[i].b_category,catMsg.m_cat)){
                                            strcpy(toSend[numOfCatMsg.m_count].m_entry.b_text,board[i].b_text);
                                            strcpy(toSend[numOfCatMsg.m_count].m_entry.b_category,board[i].b_category);
                                            toSend[numOfCatMsg.m_count].m_entry.b_date = board[i].b_date;
                                            toSend[numOfCatMsg.m_count].m_type = B_TYPE_ENT;
                                            numOfCatMsg.m_count++;
                                         }
                                    send(rfd,(struct msg_cnt*)&numOfCatMsg,sizeof(numOfCatMsg),MSG_DONTROUTE);
                                    for(i=0;i<numOfCatMsg.m_count;i++)
                                        send(rfd,(struct msg_ent*)&toSend[i],sizeof(toSend[i]),MSG_DONTROUTE);
                                        return EXIT_SUCCESS;

                                    }
                                    else;break;
                     case B_TYPE_DATE:recv(rfd,&dateMsg,sizeof(dateMsg),MSG_WAITALL);
                                      if(fork()==0){
                                      numOfDateMsg.m_count= 0;
                                        for(j=0;j<B_ENT_CNT;j++)
                                            if(board[j].b_date.year>=dateMsg.m_start.year){
                                                 if(board[j].b_date.year>dateMsg.m_start.year){
                                                    strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_text,board[j].b_text);
                                                    strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_category,board[j].b_category);
                                                    toSend2[numOfDateMsg.m_count].m_entry.b_date = board[j].b_date;
                                                    toSend2[numOfDateMsg.m_count].m_type = B_TYPE_ENT;
                                                    numOfDateMsg.m_count++;
                                                    }
                                                 else if(board[j].b_date.month>=dateMsg.m_start.month){
                                                  if(board[j].b_date.month>dateMsg.m_start.month){
                                                            strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_text,board[j].b_text);
                                                            strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_category,board[j].b_category);
                                                            toSend2[numOfDateMsg.m_count].m_entry.b_date = board[j].b_date;
                                                            toSend2[numOfDateMsg.m_count].m_type = B_TYPE_ENT;
                                                            numOfDateMsg.m_count++;
                                                        }
                                                        else if(board[j].b_date.day>=dateMsg.m_start.day){
                                                                strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_text,board[j].b_text);
                                                                strcpy(toSend2[numOfDateMsg.m_count].m_entry.b_category,board[j].b_category);
                                                                toSend2[numOfDateMsg.m_count].m_entry.b_date = board[j].b_date;
                                                                toSend2[numOfDateMsg.m_count].m_type = B_TYPE_ENT;
                                                                numOfDateMsg.m_count++;
                                                               }//if day
                                                     else ;
                                                 }//if month
                                                  else ;
                                            }//if year
                                        else;



                                        send(rfd,(struct msg_cnt*)&numOfDateMsg,sizeof(numOfDateMsg),MSG_DONTROUTE);
                                        for(j=0;j<numOfDateMsg.m_count;j++)
                                            send(rfd,(struct msg_ent*)&toSend2[j],sizeof(toSend[j]),MSG_DONTROUTE);
                                        return EXIT_SUCCESS;

                                      }// if fork
                                      else ;break;

        }//switch
    }//else

 }

}





}
