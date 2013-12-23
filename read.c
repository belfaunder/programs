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

int my_sprintf_oct( int a)                              // переводит в восьмиричную систему счисления
{       int i=1;
        int c =0;
        while( a != 0)
        {       c = (a % 8)*i+c;
                a = a/8;
                i=i*10;
        }
        return c;
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


void error(char *s)
{        printf("ERROR:%s ERRNO:%s\n", s,strerror(errno));
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
int main(int argc, char **argv)
{       int datafd;
        DIR *dirfd;
        char *pathdir=argv[1];
        char *path, *str_send, *ptr ;
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
         while((file=readdir(dirfd))!=NULL)
        {
                if((strcmp(file->d_name, ".")!=0)&&(strcmp(file->d_name,"..")!=0))               // если очередная запись соответсвует текущей директории или родительской, то не обрабатываем
                {
                        path=(char*)calloc((strlen(file->d_name)+len),sizeof(char));            //путь к файлу
                        strcpy(path, pathdir);
                        strcpy(&(path[len]),file->d_name);

                        if((stat(path, &buf)) < 0)                                      //читаем атрибуты этого файла
                                error( "Can't get file's data");

                        if ((S_ISDIR(buf.st_mode)) != 1)                                //есди директория, то
                        {
                                char *type = find_type_of_file(buf.st_mode);                    //тип файла
                                for_login = getpwuid( buf.st_uid);                              //находим логин( подобных функций не проходили, но как иначе, не знаю)
                                for_group = getgrgid(buf.st_gid);                               //имя группы

                                int permission_oct = (buf.st_mode & 0x000001ff);                //маска доступа к файлу кодируется первыми 9 битами поля st_mode
                                int permission_dec_int = my_sprintf_oct(permission_oct);                        // маска кода кодируется в восьмиричной системе
                                permission_dec = my_sprintf_char(permission_dec_int);
                                char *time_change;                                              //время последней модификации
                                time_change = ctime(&(buf.st_ctime));

                                char *size;
                                size =(char *)calloc( 32, sizeof(char));
                                size = my_sprintf_char(buf.st_size);                            //размер файла

                                char *str0 = "\nname of file : ";                               //формируем строку
                                char *str1 ="\ntype of file: ";
                                char *str2 = "\nmask of permission : ";
                                char *str3 ="\nusername: ";
                                char *str4 = "\nuser group name : ";
                                char *str5 ="\nsize : ";
                                char *str6 = "\nlast time of change : ";
                                int max = strlen(str0)+strlen(str1)+strlen(str2)+strlen(str3)+strlen(str4)+strlen(str5)+strlen(str6);
                                size_send = strlen(file->d_name) + strlen(permission_dec) + strlen(for_login->pw_name) + strlen( for_group->gr_name) + strlen(type) + strlen(time_change)+strlen(size)+strlen(str0)+strlen(str1)+strlen(str2)+strlen(str3)+strlen(str4)+strlen(str5)+strlen(str6);
                                str_send =(char*) calloc(size_send, sizeof(char));
                                strcat(str_send, str0);
                                strcat(str_send, file->d_name);
                                strcat(str_send, str1);
                                strcat(str_send, type);
                                strcat(str_send, str2);
                                strcat(str_send, permission_dec);
                                strcat(str_send, str3);
                                strcat(str_send, for_login->pw_name);
                                strcat(str_send, str4);
                                strcat(str_send, for_group->gr_name);
                                strcat(str_send, str5);
                                strcat(str_send, size);
                                strcat(str_send, str6);
                                strcat(str_send, time_change);
                                length = size_send+1;

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

                                ptr = (char*) mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);//отображаем файл в память
                                if(ptr == MAP_FAILED)
                                        error("Mapping failed!");

                                strcpy(ptr, str_send);                  //обращаемся с ptr как с указателем типа (*char)
                                munmap( (void*)ptr, length);            //прекращаем отображать в память

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
                str_send ="..\0";                                       //после отправки последнего файла отправляем отсеянный ранее файл, чтобы wriet.c узнал о конце работы
                length =strlen(str_send)+1;

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

                ptr = (char*) mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, datafd, 0);
                if(ptr == MAP_FAILED)
                        error("Mapping failed!");

                strcpy(ptr, str_send);
                munmap( (void*)ptr, length);

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
        


