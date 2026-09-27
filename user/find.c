#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void
func_find(char* head, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(head, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", head);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", head);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    break;
  

  case T_DIR:    
    if(strlen(head) + 1 + DIRSIZ + 1 > sizeof buf){
      fprintf(2, "find: path too long\n");
      break;
    }

    strcpy(buf, head);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0) continue;

      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if(stat(buf, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", buf);
        continue;
      }

      if(strcmp(de.name, target) == 0) printf("%s\n", buf);

      if(st.type == T_DIR) func_find(buf, target);
    }

    break;
  }
  
  close(fd);
}



int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "find: invalid request...\n");
    exit(1);
  }

  func_find(argv[1], argv[2]);

  exit(0);

}