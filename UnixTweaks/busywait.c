#include "types.h"
#include "user.h"

int main() {
  int x = 0;
  fork();
  fork();
  for (long long i = 0; i < 2000000000ll; i++) x = (x + 213) % 94714521;
  printf(1, "I/O section 1 running, process 1 complete: %d\n", x);
  for (long long i = 0; i < 1000000000ll; i++) x = (x + 784) % 94714521;
  printf(1, "I/O section 2 running, process 2 complete: %d\n", x);
  for (long long i = 0; i < 5000000000ll; i++) x = (x + 139) % 94714521;
  printf(1, "I/O section 3 running, process 3 complete: %d\n", x);
  exit();
}