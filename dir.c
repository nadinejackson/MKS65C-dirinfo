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
void ll(char * path)
{
  DIR * directory = opendir(path);
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
  closedir(directory);
}


void tree(char * path, int level)
{
  DIR * directory = opendir(path);
  struct dirent * p = readdir(directory);
  struct stat * buf = malloc(sizeof(struct stat));
  int exam;
  while (p) {
    exam = stat(p->d_name, buf);
    if (strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
      {
	int ctr = level;
	while(ctr--)
	  printf("│ ");
	printf("├─%s\n", p->d_name);
	if (p->d_type == 4)
	  {
	    char new[32];
	    strcpy(new, path);
	    strcpy(new + strlen(path), "/");
	    strcpy(new + strlen(path) + 1, p->d_name);
	    tree(new, level + 1);
	  }
      }
    p = readdir(directory);
  }
  free(buf);
  closedir(directory);
}

long total_size(char * path)
{
  DIR * directory = opendir(path);
  struct dirent * p = readdir(directory);
  struct stat * buf = malloc(sizeof(struct stat));
  long size;
  long total = 0;
  int exam;
  while (p) {
    exam = stat(p->d_name, buf);
    if (strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
      {
	if (p->d_type == 4)
	  {
	    char new[32];
	    strcpy(new, path);
	    strcpy(new + strlen(path), "/");
	    strcpy(new + strlen(path) + 1, p->d_name);
	    total += total_size(new);
	  }
	else
	  {
	    total += buf->st_size;
	  }
      }
    p = readdir(directory);
  }
  free(buf);
  closedir(directory);
  return total;
}
  
  
int main(int argc, char * argv[])
{
  char dir[32];
  if (argc < 2)
    {
      printf("Which directory would you like to see?\n");
      scanf("%s", dir);
    }
  else
    strcpy(dir, argv[1]);
  DIR * directory = opendir(dir);
  if (!errno)
    {
      printf("~~~~~~~~~~~~ll-style output~~~~~~~~~~~~\n");
      ll(dir);
      printf("~~~~~~~~~~~~tree-style output~~~~~~~~~~~~\n");
      tree(dir, 0);
      printf("~~~~~~~~~~~~total size including subdirectories~~~~~~~~~~~~\n");
      printf("Total size: %ld KB\n", total_size(".") / 1024);
      //printf("│= pipe, ├ = junction,  ─ = horizontal bar\n");
      closedir(directory);
    }
  else
    perror("Error");
  return 0;
}
