#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// void send_sig_hanlder(){
// 	char* err_msg = 0;
//	sqlite3_exec(db,sql,Recep,0,&err_msg);

// }


   
//search all data

int Read(void* NotUsed, int argc, char** argv, char** azColName)
{	int fd;

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
	printf("\n"); 
	

	return 0;

	
}

// int Recp(void* NotUsed, int argc, char** argv, char** azColname){


// 	return 0;
// }

