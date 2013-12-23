#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/sem.h>
void error(char *s)
{       printf("ERROR:%s ERRNO:%s\n", s,strerror(errno));
        exit(-1);
}


int main()
{       int flag =0;
        struct sembuf mybuf;            //создаем семафоры
        key_t key;
        int semid;
        umask(0);
        char pathname[] = "manager.c";
        if(( key = ftok( pathname, 0) ) < 0)
                error("Can't generate key");
        if ( (semid = semget(key, 4, 0666|IPC_CREAT )) < 0)
                error("Can't get semid");

        int datafd;
        char *x;
        int length = 256*2+32+9+3+1+26+114;   //вычисляю максимальнй размер строки
        if( (datafd=open("mapped.dat",O_RDWR|O_CREAT, 0666)) < 0)
                error("Can't open file\n");
        char *start;            //адрес отображенной памяти

        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 3;
        if((semop( semid, &mybuf, 1) )< 0)                           //ждет разрешения от read.c
              error("Can't wait for condition");


        while(flag ==0)                         //cчитываем, пока не прийдет сообщение".."
        {
                mybuf.sem_op = -1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 2;
                if((semop( semid, &mybuf, 1) )< 0)                           //bufer not full
                      error("Can't wait for condition");

                mybuf.sem_op = -1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 0;
                if((semop( semid, &mybuf, 1) )< 0)                           //critical section
                      error("Can't wait for condition");

                start = (char *)mmap( NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);
                if(start == MAP_FAILED)
                        error("Mapping failed!");

                x = (char*)start;

                if(strcmp( x , "..") != 0)
                {
                      printf("%s\n", x);                      //т.к. в файл записаны строки,оканчивающиеся на \0, то обращаемся с *start как с динамической строкой
                      munmap((void*)start, length);
                }
                else
                {     flag =1;
                }
                mybuf.sem_op = 1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 0;
                if((semop( semid, &mybuf, 1) )< 0)                           //bufer empty
                      error("Can't wait for condition");

                mybuf.sem_op = 1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 1;
                if((semop( semid, &mybuf, 1) )< 0)                           //out of critical section
                      error("Can't wait for condition");
        }
        if( (semctl(semid, 0, IPC_RMID, 0)) < 0)
                error("Can't delete semafors");

        close (datafd);
        return 0;
}
