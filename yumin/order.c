#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sqlite3.h>

int Read(void* NotUsed, int argc, char** argv, char** azColName)
{	FILE *fd;

	NotUsed = 0;
	fd = fopen("menu.txt","a");

	for (int i = 0; i < argc; i++)
	{
	
		fprintf(fd, argv[i]);
		fprintf(fd," ");
		printf("%s\n",argv[i]);
		

	}
	fprintf(fd,"\n");
	fclose(fd);

	return 0;

	
}



int Receipt(sqlite3 *db,int num,char *text){

    char *err_msg = 0; 
    sqlite3_stmt *res;
    int rc = sqlite3_open("menu.db",&db); 
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
        return 1;

    }
    rc = sqlite3_bind_text(res,1,text,-1,SQLITE_STATIC);
    if( rc != SQLITE_OK){
        return 1;
    }

    rc = sqlite3_step(res);
    int price = sqlite3_column_int(res,0);
    int cost = num * price
    if (rc != SQLITE_DONE){

        return 1;
    }
    sqlite3_finalize(res);

    sql = "INSERT INTO RECEIPT(name,num,cost) values(?,?,?);"
    rc = sqlite3_prepare_v2(db,sql,-1,&res,NULL);
    if(rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_bind_text(res, 1,text);
    if(rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_bind_int(res,2,num,-1,SQLITE_STATIC);
    if( rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_bind_int(res,3,cost,-1,SQLITE_STATIC);
    if( rc != SQLITE_OK){
        return 1;
    }
    rc = sqlite3_step(res);
    if(rc != SQLITE_DONE){
        return 1;
    }
    sqlite3_finalize(res);

    FILE* fd = fopen("receipt.txt","a");
    
    fprintf("%s %d %d",text,num,cost);

    fclose(fd);
    sqlite3_close(db);

    return 0;

}

