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
        
        int fd1, fd2;
        char* err_msg = 0;
	    sqlite3* db;
        
	    // scanf("%s",menu_name);
	    int rc = sqlite3_open("test.db", &db);
	 

	    if (rc != SQLITE_OK)
	    {
		perror("DB");
		sqlite3_close(db);
		exit(1);
		}
        if(mkfifo("menu_pipe",0666) < 0){
            perror("mkfifo");
            exit(1);
        }
        rc = sqlite3_exec(db,sql,Read,0,&err_msg);

        if((fd1 = open(menu.txt,O_RDONLY)) < 0){
            perror("open1");
            exit(1);
        }
        if((read(fd,buf,sizeof(buf))) < 0){
            perror("read");
            exit(1);
        }
    
        if((fd2 = open(menu_pipe,O_WRONLY|O_CREAT|O_TRUNC)) < 0){
            perror("open2");
            exit(1);
        }
        if((write(fd2,buf,strlen(buf))) < 0 ){
            perror("read");
            exit(1);
        } 
        puts(buf);
        close(fd);

}