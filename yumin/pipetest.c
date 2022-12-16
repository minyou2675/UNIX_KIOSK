#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    int pd;
    int n;
    char inmsg[80];
    
    printf("1");
    if((pd = open("./menu_pipe", O_RDONLY)) == -1)
    {
        perror("openfifoin");
        exit(1);
    }

    printf("1");
    while((n=read(pd, inmsg, 80)) > 0)
        write(1, inmsg, 80);

    printf("1");
    if(n==-1)
    {
        perror("read");
        exit(1);
    }
    close(pd);

    printf("1");
    printf("%s\n", inmsg);
} 
