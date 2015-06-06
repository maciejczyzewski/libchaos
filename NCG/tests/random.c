#include <stdio.h>
#include <stdlib.h>

#include "../src/ncg.c"
#include "../src/include/random.c"

int main (int argc, char const *argv[])
{
  if (argc < 2) {
    printf("usage: %s <number> \n", argv[0]);
    return 1;
  }

  ncg((uint32_t) atoi(argv[1]));

  while (1) {
    putc_unlocked(pull(), stdout);
  }

  return 0;
}