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
{	FILE *fd;

	NotUsed = 0;
	fd = fopen("menu.txt","a");
	
	//strcpy(buf,azColName[0]);
	//strcat(buf, " ");
	//strcat(buf,azColName[1]);
	//strcat(buf,"\n");
	for (int i = 0; i < argc; i++)
	{
	
		//printf("%s\n", argv[i] ? argv[i] : NULL);
		fprintf(fd, argv[i]);
		fprintf(fd," ");
		

	}
	fprintf("\n");
	fclose(fd);

	return 0;

	
}

// int Recp(void* NotUsed, int argc, char** argv, char** azColname){


// 	return 0;
// }

