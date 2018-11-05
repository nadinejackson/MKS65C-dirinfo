#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

void rwx(int permissions, char perms[])
{
  permissions = permissions % 512;
  //printf("permissions in octal: %o\n", permissions);
  char letters[9] = "rwxrwxrwx";
  int i = 0;
  while(permissions > 0){
    if(permissions % 2 == 1)
      perms[8 - i] = letters[8 - i];
    else
      perms[8 - i] = '-';
    i++;
    permissions /= 2;
  }
}
char * ll()
{
  
  DIR * directory = opendir(".");
  struct dirent * p = readdir(directory);
  struct stat * buf = malloc(sizeof(struct stat));
  long size;
  long total_size = 0;
  int exam;
  while (p) {
    exam = stat(p->d_name, buf);

    //directory or no
    if (p->d_type == 4)
      printf("d");
    else
      printf("-");
    
    //permissions
    char s[9];
    rwx(buf->st_mode, s);
    printf("%s ", s);
    
    //size
    size = buf->st_size;
    total_size += size;
    if (size < 1024)
      printf("%ld B      ", size);
    else
      printf("%ld KB     ", size /= 1024);
    while(size /= 10)
      printf("\b");
    
    //time
    int len = strlen(ctime(&(buf->st_atime)));
    char time[len];
    time[len-1] = '\0';
    strncpy(time, ctime(&(buf->st_atime)), len-1);
    printf("%s ", time);

    //filename
    printf("%s\n", p->d_name);

    p = readdir(directory);
  }
  free(buf);
  printf("\nThe total size of files in this directory is %ld KB\n", total_size / 1024);
}

int main()
{
  printf("~~~~~~~~~~~~ll-style output~~~~~~~~~~~~\n");
  ll();
  return 0;
}
