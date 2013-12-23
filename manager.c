#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/sem.h>

int main(int argc, char **argv )
{
        struct sembuf mybuf;            //создаем семафоры( write.c и read.c работают по схеме продьюсер -консьюмер, описанной на лекции)
        key_t key;                      //семафор №0 отвечаает за вход в critical section
                                        // №1=1  - если файл пуст
                                        //№2=0 - если файл пуст
        int semid;
        umask(0);
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
