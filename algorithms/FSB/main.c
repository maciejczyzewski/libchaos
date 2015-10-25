#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "fsb.h"

void printFirstLines(hashState * state) {
  int i,j;
  for (i=0; i<state->b; i++) {
    printf("first_line[%d] = ",i);
    for (j=0; j<(state->p>>3)+1; j++) {
      printf("%.2X",state->first_line[i][0][(state->r>>3)+j]);
    }
    printf("\n\n");
  }
}

int main(int argc, char* argv[]) {
  int i;
  FILE* file_to_hash;
  int return_status = 0;
  int hashbitlen = 224;
  BitSequence* hashval;
  unsigned int number_of_files = 0;
  char** files = (char**) malloc(argc*sizeof(char*));
  
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i],"-s") == 0) {
      if (argc > i+1) {
        hashbitlen = atoi(argv[i+1]);
        i++;
      } else {
        fprintf(stderr, "Error: option \"-s\" must be followed by the desired hash size in bits\n");
        return 1;
      }
      if (hashbitlen <= 0) {
        fprintf(stderr, "Error: \"%s\" is not a correct hash length value.\n",argv[i]);
        return 2;
      }
    } else {
      files[number_of_files] = argv[i];
      number_of_files++;
    }
  }
  
  hashval = (BitSequence*) malloc(hashbitlen>>3);

  if (number_of_files == 0) {
    /* if no files were specified, hash from stdin */
    if (HashFile(hashbitlen, stdin, hashval) != SUCCESS) {
      fprintf(stderr, "Error: given input could not be hashed properly.\n");
    }
    printf("Standard input: ");
    printHash(hashval, hashbitlen);
    printf("\n");
  } else {
    /* otherwise, hash each of the given files */
    for (i=0; i<number_of_files; i++) {
      file_to_hash = fopen(files[i],"rb");
      if (file_to_hash) {
        if (HashFile(hashbitlen, file_to_hash, hashval) != SUCCESS) {
          fprintf(stderr, "Error: file \"%s\" could not be hashed properly.\n", files[i]);
          return_status += 1;
        } else {
	  printf("%s: ",files[i]);
	  printHash(hashval, hashbitlen);
	  printf("\n");
	}
        fclose(file_to_hash);
      } else {
        fprintf(stderr, "Error: file \"%s\" could not be opened.\n", files[i]);
        return_status += 16;
      }
    }
  }
  free(files);
  free(hashval);
  return return_status;
}
