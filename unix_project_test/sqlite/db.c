#include <sqlite3.h>
#include <stdio.h>



//gcc -o db db.c -lsqlite3 -std=c99
int main(void){
	char* menu_name;
	char* err_msg = 0;
	int  menu_num;
	int price;
	sqlite3* db;
	scanf("%s",menu_name);
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
	printf("SELECT NUMBER\n");
	printf("1\n2\n3\n4\n5");
	int number;
	scanf("%d",&number);
	//select
	switch(number){
		case 1 : 
			printf("DATA INSERT");
			break;
		case 2 :
			printf("DATA DELETE");
			break;
		case 3 :
			printf("DATA MODIFY");
			break;
		case 4 :
			printf("Look up");
			break;
		case 5 :
			break; 
	
	}	
	//Looking up
	char* look = ("select * from menu;");
	rc = sqlite3_exec(db,look,callback,0&err_msg);
	//menu name,num,
	//
	//insert query execute
	char* query = ("INSERT INTO MENU VALUES(%s,%d,%d);",menu_name,menu_num,
		price);
	rc = sqlite3_exec(db, query, 0, 0, &err_msg);
	free(err_msg)
	sqlite3_close(db);
	return 0;

}
//search all data
int look(sqlite3* db,char* err_msg,int rc){

	char* query = ("select * form menu");
	rc = sqlite3_exec(db,query,callback,0,&err_msg);
	return 0;
}
//insert date
int insert_data(sqlite3_stmt* res, sqlite3* db, char* err_msg, int rc){
	char* query = "insert into menu values(?,?,?,?)";
	if(rc != SQLITE_OK)
	{
		perror(sqlite);
		sqlite3_close(db);
		return 1;
	}
	rc = sqlite3_prepare_v2(db, query, -1, &res, 0);
	if ( rc == SQLITE_OK) 
		


}
//modify data



//delete data
int delete_data(int argc, char** argv){

	char* query = "delete 


}
// 

int callback(void* NotUsed, int argc, char** argv, char** azColName)
{	
	NotUsed = 0;
	
	for (int i = 0; i < argc; i++)
	{
	
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : NULL);

	}
	printf("\n"); 



	return 0;
}

