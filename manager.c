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
        struct sembuf mybuf;            //создаем семафоры
        key_t key;
        int semid;
        umask(0);
        int pid;
        pid=fork();
        char pathname[] = "manager.c";
        if(( key = ftok( pathname, 0) ) < 0)
                error("Can't generate key");
        if ( (semid = semget(key, 3, 0666|IPC_CREAT )) < 0)
                error("Can't get semid");


        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 1;

        if((semop( semid, &mybuf, 1) )< 0)                           //bufer not empty
                error("Can't wait for condition");

        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;

        if((semop( semid, &mybuf, 1) )< 0)                           //critical section
                error("Can't wait for condition");

        if ( pid > 0 )                                                  //writer
        {
                execl(argv[2], argv[2],argv[1] , NULL);
                printf("Can't run 1\n");
                exit(-1);

        }
        else
        {       if(pid==0)                                              //reader
                {
                        execl(argv[3], argv[3] , NULL);
                        printf("Can't run 2\n");

                }
                else
                {       printf("ARM");
                        exit(-1);
                }
        }

        return(0);
}
