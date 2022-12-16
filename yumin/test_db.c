#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <fcntl.h>

int Receipt(int num,char *text){
    sqlite3 *menu_db;
    sqlite3 *db;
    char *err_msg = 0; 
    sqlite3_stmt *res;
    char *sql = "DROP TABLE IF EXISTS Receipt;" 
                "CREATE TABLE MENU(Id INT, Name char(20), Num INT, Cost INT);";
    int rc2 = sqlite3_open("menu.db",&menu_db);
    int rc = sqlite3_open("receipt.db", &db); 
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
  

     sql = "SELECT PRICE FROM MENU WHERE MENU.Name = ?";
    
 
      if (rc2 != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }

    if((rc = sqlite3_prepare_v2(db,sql,-1,&res,0)) == -1){
       perror("sqlite3_prepare");
       exit(1); 

    }
    rc = sqlite3_bind_text(res,1,text,-1,SQLITE_STATIC);
    if( rc != SQLITE_OK){
        perror("sqlite3_bind");
        exit(1);
    }

    rc = sqlite3_step(res);
    int price = sqlite3_column_int(res,0);
    int cost = num * price
    if (rc != SQLITE_DONE){
        perror("sqlite3_step");
        exit(1);
    }
    sqlite3_finalize(res);

    sql = "INSERT INTO Receipt(name,cost) values(?,?);"
    rc = sqlite3_prepare_v2(db,sql,-1,&res,NULL);
    if(rc != SQLITE_OK){
        perror("sqlite3_prepare");
        exit(1);
    }
    rc = sqlite3_bind_text(res, 1,text);
    if(rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_bind_int(res,2,cost,-1,SQLITE_STATIC);
    if( rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_step(res);
    if(rc != SQLITE_DONE){
        return 1;
    }
    sqlite3_finalize(res);

    sqlite3_close(db);


    

   
    return 0;

}



