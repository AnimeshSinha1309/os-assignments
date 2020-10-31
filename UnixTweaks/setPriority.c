#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(1, "setPriority: requires 2 arguments, %d provided\n", argc);
    exit();
  }
  int priority = atoi(argv[1]);
  int pid = atoi(argv[2]);
  int old_priority = set_priority(priority, pid);
  printf(1, "setPriority: changed priority from %d to %d\n", old_priority, priority);
  exit();
}
