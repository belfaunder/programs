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
#include<pwd.h>
#include<grp.h>
#include<time.h>

void error(char *s)
{       printf("ERROR:%s ERRNO:%s\n", s,strerror(errno));
        exit(-1);
}

char* find_type_of_file(mode_t f_type)                          //возвращает  тип файла
        {       char *type;
                if( (S_ISREG(f_type)) == 1 )
                        type ="regular";
                else
                {       if( (S_ISLNK(f_type)) == 1 )
                                 type ="link";
                        else
                        {       if( (S_ISFIFO(f_type)) == 1 )
                                 type ="fifo";
                                 else
                                 {       if( (S_ISSOCK(f_type)) == 1 )
                                         type ="socket";
                                 }
                        }
                }
                return type;
        }


char *my_sprintf_char(int a)                            //переводит число в строку
{       char *mas;
        int i=31;
        char t;
        mas =(char *)calloc( 32, sizeof(char));
        while( a != 0)
        {       t = a % 10;
                mas[i] =t + '0';
                a = a / 10;
                i--;
        }
        return (mas+1+i);
}


int main()
{       struct for_send
        {       char    name[255];
                mode_t  mode;
                uid_t   uid;
                gid_t   gid;
                off_t   size;
                time_t  mtime;
        } *start;
        struct passwd *for_login;
        struct group *for_group;

        int flag =0;
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
        int length;
        if( (datafd=open("mapped.dat",O_RDWR|O_CREAT, 0666)) < 0)
                error("Can't open file");

        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 3;
        if((semop( semid, &mybuf, 1) )< 0)                           //ждет разрешения от read.c
              error("Can't wait for condition");


        while(flag == 0)                         //cчитываем, пока не прийдет сообщение".."
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

                length = sizeof( struct for_send);
                start = (struct for_send*)mmap( NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);
                if(start == MAP_FAILED)
                        error("Mapping failed!");
                if(strcmp( start->name , "..") == 0)
                {       flag = 1;
                }
                else
                {
                        printf("\nname of file : %s\n", start->name);
                        char *type = find_type_of_file(start->mode);
                        printf("type of file: %s\n", type);
                        for_login = getpwuid( start->uid);
                        for_group = getgrgid(start->gid);

                        printf("user login : %s\n",for_login->pw_name);
                        printf("user's group login : %s\n",for_group->gr_name);

                        int permission_oct = (start->mode & 0x000001ff);                //маска доступа к файлу кодируется первыми 9 битами поля st_mode
                        printf("maska dostypa : %o\n", permission_oct);
                        printf("last modification : %s", ctime(&(start->mtime)) );
                        printf("size : %d\n", start->size);
                }

                munmap((void*)start, length);

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
