#include "types.h"
#include "user.h"

int main() {
  int x = 0;
  for (int i = 0; i < 200000000; i++) x = (x + 213) % 94714521;
  printf(1, "I/O section 1 running, process 1 complete: %d\n", x);
  for (int i = 0; i < 100000000; i++) x = (x + 784) % 94714521;
  printf(1, "I/O section 2 running, process 2 complete: %d\n", x);
  for (int i = 0; i < 500000000; i++) x = (x + 139) % 94714521;
  printf(1, "I/O section 3 running, process 3 complete: %d\n", x);
}