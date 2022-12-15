#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORTNUM 9000

int main(){
        char buf[256];
        struct DB dbs;
        struct sockaddr_in sin, cli;
        int sd, ns, clientlen = sizeof(cli);

	dbs.rc = sqlite3_open(test.db,dbs->db);
        if (dbs.rc != SQLITE_OK)
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
        sin.sin_addr.s_addr = inet_addr("10.0.2.255");

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
            	sqlite3_exec(dbs.db,sql,Read,0,&err_msg);
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
        }
        //buf에 있는 내용을 DB에 추가 + PRINTF 로 내역 띄우기

        //table 형태는? 
        //sql = create table receipt(name varchar(20),num int,income int,delivery int,FOREIGN KEY REFERENCES ON 
        //  MENU(NAME) ON UPDATE CASCADE);
        

        close(ns);
        close(sd);

	


}
