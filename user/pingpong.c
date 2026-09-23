#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(2, "pingpong: invalid request...\n");
    exit(1);
  }

  int p1[2];  // pipe1 for parent to child communication
  if (pipe(p1) < 0) {
    fprintf(2, "pingpong: pipe1 failed...\n");
    exit(1);
  }

  int p2[2];  // pipe2 for child to parent communication
  if (pipe(p2) < 0) {
    fprintf(2, "pingpong: pipe2 failed...\n");
    exit(1);
  }


  int pid = fork();  // create a child process

  if (pid < 0) {
    fprintf(2, "pingpong: fork failed...\n");
    exit(1);
  }

  else if (pid == 0) {  // child process
    close(p1[1]);  // close write end of pipe1
    close(p2[0]);  // close read end of pipe2
    
    char buf[1] = {'a'};

    read(p1[0], buf, 1); // wait for the parent to send a message
    fprintf(1, "%d: received ping\n", getpid());
    write(p2[1], buf, 1);  // send a message back

    exit(0);
  } 

  else {  // parent process
    close(p1[0]);  // close read end of pipe1
    close(p2[1]);  // close write end of pipe2

    char buf[1] = {'b'};

    write(p1[1], buf, 1);  // send a message to the child
    read(p2[0], buf, 1);  // wait for the child to send a message
    fprintf(1, "%d: received pong\n", getpid());
    
    wait(0);  // wait for the child process to finish
  }
  

  exit(0);

}