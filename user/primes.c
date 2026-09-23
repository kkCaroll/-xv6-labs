#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


// Function to find prime numbers using pipes and fork
void
fork_prime(int left) {
  int n;
  if (read(left, &n, sizeof(int)) <= 0) {  // if there are no more numbers to read
    exit(0);
  }
  
  fprintf(1, "prime %d\n", n);  // print the prime number

  int p_new[2];
  if (pipe(p_new) < 0) {  // create a new pipe for the next child process
    fprintf(2, "primes: pipe failed...\n");
    exit(1);
  }

  int pid_new = fork();  // create a new child process

  if (pid_new < 0) {
    fprintf(2, "primes: fork failed...\n");
    exit(1);
  }

  else if (pid_new == 0) {  // new child process
    close(p_new[1]);  
    close(left);  // close the read end of the previous pipe
    fork_prime(p_new[0]);  // call the function recursively
    close(p_new[0]);  // close read end of the new pipe
  }

  else {  // parent process
    int num;
    while (read(left, &num, sizeof(int)) > 0) {  // read numbers from the new pipe
      if (num % n != 0) {  // if the number is not divisible by the prime number
        write(p_new[1], &num, sizeof(int));  // send it to the next child process
      }
    }

    close(p_new[1]);  // close write end of the new pipe
    close(p_new[0]);  // close read end of the new pipe
    wait(0);  // wait for the child process to finish
  }
}



// -------------------------------------------------------------------------
// Main function to initiate the prime number finding process
int
main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(2, "primes: invalid request...\n");
    exit(1);
  }

  int p[2];  // create pipe
  if (pipe(p) < 0) {
    fprintf(2, "primes: pipe failed...\n");
    exit(1);
  }

  // create a child process
  int pid = fork();  

  if (pid < 0) {
    fprintf(2, "primes: fork failed...\n");
    exit(1);
  }

  else if (pid == 0) {  // child process
    close(p[1]);  // close write end of the pipe
    fork_prime(p[0]);  // call the function to find prime numbers
  }

  else {  // parent process
    close(p[0]);  // close read end of the pipe
    for (int i = 2; i <= 35; i++) {  // send numbers from 2 to 35 to the child process
      if(write(p[1], &i, sizeof(int)) != sizeof(int)) {
        fprintf(2, "primes: write failed...\n");
        exit(1);
      }
    }
    close(p[1]);  // close write end of the pipe
    wait(0);  // wait for the child process to finish
  }

  exit(0);
}

