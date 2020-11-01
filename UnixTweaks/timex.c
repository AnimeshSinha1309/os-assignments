#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int proc_id = fork();

  if (proc_id == 0) {
    exec(argv[1], argv + 1);
  } else {
    int *w_time = (int *)malloc(sizeof(int));
    int *r_time = (int *)malloc(sizeof(int));
    waitx(w_time, r_time);
    printf(1, "\n\n");
    printf(1, "timex: Running time: %d\n", *r_time);
    printf(1, "timex: Waiting_time: %d\n", *w_time);
  }
  exit();
}