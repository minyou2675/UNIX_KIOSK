#include <sys/socket.h>
#include <sqlite3.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include  "db.h"

#define PORTNUM 9000

int main(){
        char buf[256];
        
        struct sockaddr_in sin, cli;
        int sd, ns, clientlen = sizeof(cli);

        char* err_msg = 0;
	    sqlite3* db;
	    // scanf("%s",menu_name);
	    int rc = sqlite3_open("test.db", &db);
	    sqlite3_stmt* res;

	    if (rc != SQLITE_OK)
	    {
		perror("DB");
		sqlite3_close(db);
		exit(1);
		}
	    rc = sqlite3_prepare_v2(db,"SELECT SQLITE_VERSION()",-1,&res,0);
	    if (rc != SQLITE_OK)
	    {
		perror("DB");
		sqlite3_close(db);
		exit(1);
        }

        if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket");
            exit(1);
        }
        memset((char *)&sin, '\0', sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORTNUM);
        sin.sin_addr.s_addr = inet_addr("54.180.1.233");

        if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
            perror("bind");
            exit(1);
        }

        if(listen(sd ,5)){
            perror("listen");
            exit(1);
        }
        int client_addr_size;
        client_addr_size = sizeof(cli);
        
        while(1){
        if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1){
            perror("accept");
            exit(1);
        }
        else{
               char* sql = "select * from menu where available = 1;";
            	sqlite3_exec(db,sql,Read,0,&err_msg);
        //     //accept 할 시 read data 실행
         
        }
        // //영수증 받기
        if((recv(ns,*buf,sizeof(buf),0))== -1){
            perror("receive");
            exit(1);
        };
        
        sprintf(buf, "Your IP address is %s", inet_ntoa(cli.sin_addr));
        if(send(ns, buf, strlen(buf) + 1, 0) == -1){
            perror("send");
            exit(1);
        }
        else{
            break;
        }
        }
        //buf에 있는 내용을 DB에 추가 + PRINTF 로 내역 띄우기

        //table 형태는? 
        //sql = create table receipt(name varchar(20),num int,income int,delivery int,FOREIGN KEY REFERENCES ON 
        //  MENU(NAME) ON UPDATE CASCADE);
        
        free(err_msg);
	sqlite3_close(db);
        close(ns);
        close(sd);


}
