#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

//comment
int main(int argc, char **argv )
{
        int pid;
        pid=fork();

        if ( pid > 0 )                                                  //запускаем read.c - на фвход подаемстя путь к директории
        {
                execl(argv[2], argv[2],argv[1] , NULL);
                printf("Can't run %s\n",argv[2]);
                exit(-1);
        }
        else
        {       if(pid==0)                                              //запускаем write.с
                {
                        execl(argv[3], argv[3] , NULL);
                        printf("Can't run %s\n", argv[2]);
                        exit(-1);
                }
                else
                {       printf("ARM");
                        exit(-1);
                }
        }
        return(0);
}
