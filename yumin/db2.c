#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int Read(void*, int, char**, char** );

void read_sig_handler(sqlite3* db){
	char* err_msg = 0;
	char* sql = "select * from menu where available = 1;";
	sqlite3_exec(db,sql,Read,0,&err_msg);

}

// void send_sig_hanlder(){
// 	char* err_msg = 0;
//	sqlite3_exec(db,sql,Recep,0,&err_msg);

// }

typedef struct {

   
    sqlite3* db;
    int rc;
    sqlite3_stmt* res;
    
} DB;

//search all data

int Read(void* NotUsed, int argc, char** argv, char** azColName)
{	
	NotUsed = 0;
	char buf[1024];
	
	for (int i = 0; i < argc; i++)
	{
	
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : NULL);
		strcat(buf, argv[i]);
		

	}
	puts(buf);
	printf("\n"); 

	return 0;

	
}

// int Recp(void* NotUsed, int argc, char** argv, char** azColname){


// 	return 0;
// }

