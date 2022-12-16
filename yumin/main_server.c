#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include  "order.h"
#include <sys/msg.h>
struct message{
    long type;
    char buf[BUFSIZ];

};


int message_queue_id = msgget(12345, 0666 | IPC_CREAT);
if(message_queue_id == -1){
    perror("message queue id");
    exit(1);
}


int main(){
    struct message msg;
    msg.type = 1;

	char buf[1024];
    int fd1[2];
    char* sql = "select name,price from menu;";
    char* err_msg = 0;
    sqlite3* db;
    

    
    // scanf("%s",menu_name);
    int rc = sqlite3_open("menu.db", &db);


    if (rc != SQLITE_OK)
    {
        perror("DB");
        sqlite3_close(db);
        exit(1);
        }
    rc = sqlite3_exec(db,sql,Read,0,&err_msg);
	puts("sql");

if(pipe(fd1) == -1){
        perror("pipe");
        exit(1);
    }
int fd2 = dup(fd1[0]);
int fd3 = dup(fd1[1]);

if((fd2  = open("menu.txt",O_RDONLY)) == -1){
        perror("open1");
        exit(1);
    }
puts("menu.txt open");
puts("read fd1");
if((read(fd2,buf,sizeof(buf))) < 0){
        perror("read");
        exit(1);
    }

strcpy(msg.buf,buf);
if(msgsnd(message_queue_id, &msg, sizeof(msg), 0) == -1){
    perror("msgsnd");
    exit(1);
}
close(fd2);


printf("exit\n");
sleep(10000);    
close(fd3);
return 0;
}