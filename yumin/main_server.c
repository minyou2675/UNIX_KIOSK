#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include  "db2.h"

int main(){
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
close(fd2);
puts("read fd2");

if(mkfifo("menu_pipe",0666) == -1){
        perror("mkfifo");
        exit(1);
    }

if((fd3 = open("menu_pipe",O_RDWR)) == -1){
        perror("open2");
        exit(1);
    }
puts("open menu_pipe");
if((write(fd3,buf,strlen(buf))) < 0 ){
        perror("read");
        exit(1);
    } 

printf("exit\n");
sleep(10000);    
close(fd3);
return 0;
}
