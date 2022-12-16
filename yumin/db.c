#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int Read(void*, int, char**, char** );


//gcc -o db db.c -lsqlite3 -std=c99
int main(void){
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
	//CREATE TABLE
	char *sql = "DROP TABLE IF EXISTS menu;"
				"CREATE TABLE menu(id int primary key, name varchar(2),price int,available int, img text,);"
				"insert into menu(name,price,img) values ('pizza','25000',1,'aaa');";
	rc = sqlite3_exec(db,sql,0,0,&err_msg);

	sql = "select * from menu;";

	rc = sqlite3_exec(db,sql,Read,0,&err_msg)
;	//read

	//close
	free(err_msg);
	sqlite3_close(db);
	return 0;

}
//search all data

int Read(void* NotUsed, int argc, char** argv, char** azColName)
{	
	int fd;
	if((fd = open("test.db",O_WRONLY | O_CREAT | O_EXCL, 0644) < -1)){
		perror("fd");
		exit(1);
	}

	NotUsed = 0;
	char buf[1024];
	for(int i = 0; i < 2; i++){
		printf("columns %s \n",azColName[i]);
		strcat(buf,azColName[i]);
	}
	strcat(buf,"\n");
	
	for (int i = 0; i < argc; i++)
	{
	
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : NULL);
		strcat(buf, argv[i]);
		

	}
	puts(buf);
	write(fd, buf, sizeof(buf));
	printf("\n"); 
	close(fd);

	return 0;
	
}

// int Recp(void* NotUsed, int argc, char** argv, char** azColname){


// 	return 0;
// }

