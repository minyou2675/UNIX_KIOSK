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
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("menu.db", &db);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    char *sql = "DROP TABLE IF EXISTS MENU;" 
                "CREATE TABLE MENU(Id INT, Name char(20), Price INT);" 
                "INSERT INTO MENU VALUES(1, 'pizza', 52642);" 
                "INSERT INTO MENU VALUES(2, 'pasta', 57127);" 
                "INSERT INTO MENU VALUES(3, 'hamburger', 9000);" 
                "INSERT INTO MENU VALUES(4, 'cake', 29000);" 
                "INSERT INTO MENU VALUES(5, 'salad', 350000);" 
                "INSERT INTO MENU VALUES(6, 'chicken', 21000);" 
                "INSERT INTO MENU VALUES(7, 'noodle', 41400);" 
                "INSERT INTO MENU VALUES(8, 'juice', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    }

   sql = "select name,price from menu;"; 

    rc = sqlite3_exec(db,sql,Read,0,&err_msg);

    if(rc != SQLITE_OK)
    {
	    perror("rc");
	    sqlite3_free(err_msg);
	    sqlite3_close(db);
	    return 1;
    }
    
    sqlite3_close(db);
    
    return 0;
}
//search all data

int Read(void* NotUsed, int argc, char** argv, char** azColName)
{	

	NotUsed = 0;
    
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
	
}




