#include <sqlite3.h>
#include <stdio.h>

int main(void){
	char* menu_name;
	char* err_msg = 0;
	int  menu_num;
	int price;
	sqlite3* db;
	scanf("%s",menu_name);
	int rc = sqlite3_open(":memory:", &db);
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
	//menu name,num,
	//
	//insert query execute
	char* query = ("INSERT INTO MENU VALUES(%s,%d,%d);",menu_name,menu_num,
		price);
	rc = sqlite3_exec(db, query, 0, 0, &err_msg);
	
	sqlite3_close(db);
	return 0;
}

