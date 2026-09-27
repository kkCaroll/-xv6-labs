#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int
main(int argc, char *argv[]) {
  char c;
  char line[MAXARG];  // buffer to store the argument string
  int index = 0;
  int base = argc - 1;  
  char *args[MAXARG];
  
  if (argc < 2) {
    write(2, "xargs: invalid request...\n", 26);
    exit(1);
  }

  int i;
  for (i = 1; i < argc; i++) {
    args[i - 1] = argv[i];  
  }

  if(base + 2 > MAXARG){
    fprintf(2, "xargs: too many args\n");
    exit(1);
  }

  while (read(0, &c, 1) == 1) {
    if (c == '\n') {
      line[index] = 0;  // null-terminate the argument string
      args[base] = line;  // add the argument to the list
      args[base + 1] = 0;  // null-terminate the argument list

      if (fork() == 0) {
        exec(args[0], args);
        fprintf(2, "xargs: exec failed...\n", 22);
        exit(1);
      }
      
      wait(0);   // wait for the child process to finish
      index = 0;  // reset index for the next argument
    } 
    
    else {
      if (index >= MAXARG - 1) {
        fprintf(2, "xargs: argument too long\n");
        exit(1);
      }
      line[index] = c;  // store the address of the character in the argument list
      index++;
    }
  }
  
  exit(0);  // exit the program
}