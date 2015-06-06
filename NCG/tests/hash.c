#include <stdio.h>
#include <stdlib.h>

#include "../src/ncg.c"
#include "../src/include/hash.c"

int main (int argc, char const *argv[])
{
  const char *initial_message = argv[1];
  size_t initial_length = strlen(initial_message);

  if (argc < 3) {
    printf("usage: %s 'string' <number> \n", argv[0]);
    return 1;
  }

  size_t result_length = atoi(argv[2]);
  uint8_t *result = (uint8_t*) malloc(result_length * 8);

  printf(">> ");
  for (int i = 0; i < initial_length; i++)
    printf("%p ", initial_message[i]);
  printf("\n");

  ncg((uint8_t*) initial_message, initial_length, result, result_length);

  printf("<< ");
  for (int i = 0; i < result_length; i++)
    printf("%p ", result[i]);
  printf("\n");

  return 0;
}