#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
  int i;

  if (argc < 2) {
    fprintf(2, "Usage: sleep ticks...\n");
    exit(1);
  }

  for (i = 1; i < argc; i++) {
    int ticks = atoi(argv[i]);
      if (ticks <= 0) {
        fprintf(2, "sleep: invalid ticks %s\n", argv[i]);
        exit(1);
      }
      sleep(ticks);
  }
  exit(0);
}