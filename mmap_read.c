#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>

void error(char *s)
{        printf("ERROR:%s ERRNO:%s\n", s,strerror(errno));
        exit(-1);
}


int main(int argc, char **argv)
{       struct for_send
        {       char    name[255];
                mode_t  mode;
                uid_t   uid;
                gid_t   gid;
                off_t   size;
                time_t  mtime;
        } *send;
        int i;
        int datafd;
        DIR *dirfd;
        char *pathdir=argv[1];
        char *path, *str_send;
        struct dirent *file;
        struct stat buf;
        struct passwd *for_login;
        struct group *for_group;
        char *permission_dec;
        permission_dec = (char *)calloc( 3, sizeof(char));
        int size_send=0, length =0;
        umask(0);
        struct sembuf mybuf;                                    //создаем семафоры
        key_t key;
        int semid;
        char pathname[] = "manager.c";
        if(( key = ftok( pathname, 0) ) < 0)
                error("Can't generate key");
        if ( (semid = semget(key, 4, 0666 | IPC_CREAT )) < 0)
                error("Can't get semid");

        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 1;

        if((semop( semid, &mybuf, 1) )< 0)                           //bufer empty
                error("Can't wait for condition");

        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;

        if((semop( semid, &mybuf, 1) )< 0)                           //critical section
                error("Can't wait for condition");

        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 3;

        if((semop( semid, &mybuf, 1) )< 0)                           //permission for write.c
                error("Can't wait for condition");

        if ((dirfd=opendir(argv[1]))==NULL)
                error("Can't open directory");
        if((datafd=open("mapped.dat", O_RDWR | O_CREAT | O_TRUNC, 0666))<0)
                error("Can't open file");

        int len=strlen(pathdir);                                //длина имени директории

        if (pathdir[len-1]!='/')                                //форматируем имя директории
        {       path=(char*)calloc(len+2, sizeof(char));
                strcpy(path, pathdir);
                path[len]='/';
                path[len+1]='\0';
                len++;
                pathdir=path;
        }
        length = sizeof(struct for_send);
        while((file = readdir(dirfd))!=NULL)
        {
                if((strcmp(file->d_name, ".")!=0)&&(strcmp(file->d_name,"..")!=0))// если очередная запись соответсвует текущей директории или родительской, то не обрабатываем
                {
                        path=(char*)calloc((strlen(file->d_name)+len),sizeof(char));            //путь к файлу
                        strcpy(path, pathdir);
                        strcpy(&(path[len]),file->d_name);

                        if((stat(path, &buf)) < 0)                                      //читаем атрибуты этого файла
                                error( "Can't get file's data");

                        if ((S_ISDIR(buf.st_mode)) != 1)                                //есди директория, то
                        {
                                ftruncate( datafd, length);                     //увеличиваем размер файла, до размера строки+ 1 символ на \0

                                mybuf.sem_op = -1;
                                mybuf.sem_flg = 0;
                                mybuf.sem_num = 1;
                                if((semop( semid, &mybuf, 1) )< 0)                           //bufer not empty
                                        error("Can't wait for condition");

                                mybuf.sem_op = -1;
                                mybuf.sem_flg = 0;
                                mybuf.sem_num = 0;
                                if((semop( semid, &mybuf, 1) )< 0)                           //critical section
                                        error("Can't wait for condition");

                                send = (struct for_send*)mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);   //отображаем файл в память
                                if(send == MAP_FAILED)
                                        error("Mapping failed!");

                                for(i=0;i<255;i++)send->name[i] = 0;

                                strcat(send->name,file->d_name);
                                send->mode = buf.st_mode;
                                send->uid = buf.st_uid;
                                send->gid = buf.st_gid;
                                send->size =buf.st_size;
                                send->mtime = buf.st_mtime;

                                munmap( (void*)send, length);            //прекращаем отображать в память

                                mybuf.sem_op = 1;
                                mybuf.sem_flg = 0;
                                mybuf.sem_num = 0;
                                if((semop( semid, &mybuf, 1) )< 0)                           //out of critical section
                                        error("Can't wait for condition");

                                mybuf.sem_op = 1;
                                mybuf.sem_flg = 0;
                                mybuf.sem_num = 2;
                                if((semop( semid, &mybuf, 1) )< 0)                           //bufer full
                                        error("Can't wait for condition");
                        }
                }
        }

                 //после отправки последнего файла отправляем отсеянный ранее файл, чтобы wriet.c узнал о конце работы

                mybuf.sem_op = -1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 1;
                if((semop( semid, &mybuf, 1) )< 0)                           //bufer not empty
                        error("Can't wait for condition");

                mybuf.sem_op = -1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 0;
                if((semop( semid, &mybuf, 1) )< 0)                           //critical section
                        error("Can't wait for condition");

                send = (struct for_send*) mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);
                if(send == MAP_FAILED)
                        error("Mapping failed!");

                for(i=0;i<255;i++)send->name[i] = 0;
                strcat(send->name, "..");

                munmap( (void*)send, length);

                mybuf.sem_op = 1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 0;
                if((semop( semid, &mybuf, 1) )< 0)                           //out of critical section
                       error("Can't wait for condition");

                mybuf.sem_op = 1;
                mybuf.sem_flg = 0;
                mybuf.sem_num = 2;
                if((semop( semid, &mybuf, 1) )< 0)                           //bufer full
                        error("Can't wait for condition");

                close(datafd);
        return 0;
}
