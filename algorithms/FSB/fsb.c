#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "pi.h"
#include "fsb.h"
#include "Whirlpool.h"


/* parameters of FSB, hardcoded for each output length */
#define NUMBER_OF_PARAMETERS 6
const int parameters[NUMBER_OF_PARAMETERS][5] = {
  /*
   * parameters of FSB in order:
   * {hashbitlen, n, w, r, p}
   */
  {48, 3<<17, 24, 192, 197},
  {160, 5<<18, 80, 640, 653},
  {224, 7<<18, 112, 896, 907},
  {256, 1<<21, 1<<7, 1<<10, 1061},
  {384, 23<<16, 184, 1472, 1483},
  {512, 31<<16, 248, 1984, 1987}
};

HashReturn Init(hashState * state, int hashbitlen) {
  int i,j,k,l;
  const unsigned char* Pi_line;
  for (i=0; i<NUMBER_OF_PARAMETERS; i++) {
    if (hashbitlen == parameters[i][0]) {
      state->n = parameters[i][1];
      state->w = parameters[i][2];
      state->r = parameters[i][3];
      state->p = parameters[i][4];
      state->b = state->n/state->r;
      state->bpc = logarithm(state->n/state->w);
      state->inputsize = state->w*state->bpc-state->r;
      state->bfiv = state->r/state->w;
      state->bfm = state->inputsize/state->w;
      state->hashbitlen = hashbitlen;
      state->databitlen = 0;

      /* 
       * Uncomment this to display the various parameters of FSB before hashing.
       * Useful only if you want to create your very own set of parameters!
       *
       * printf("n=%u w=%u r=%u p=%u b=%u bpc=%u input=%u bfiv=%u bfm=%u\n",state->n, state->w, state->r, state->p, state->b, state->bpc, state->inputsize, state->bfiv, state->bfm);
       */
      
      /* compute the first QC matrix line */
      state->first_line = (unsigned char***) malloc(state->b*sizeof(unsigned char**));
      for (k=0; k<state->b; k++) {
	state->first_line[k] = (unsigned char**) malloc(8*sizeof(unsigned char*));
	state->first_line[k][0] = (unsigned char*) calloc(((state->p+state->r)>>3)+1,1);
	Pi_line = &(Pi[k*((state->p>>3)+1)]);
	for (j=0; j<(state->p>>3); j++) {
	  state->first_line[k][0][(state->r>>3)+j] = Pi_line[j];
	}
	state->first_line[k][0][(state->p+state->r)>>3] = Pi_line[state->p>>3]&(((1<<(state->p&7))-1)<<(8-(state->p&7)));
	for (j=0; j<(state->r>>3); j++) {
	  state->first_line[k][0][j] = state->first_line[k][0][(state->p>>3)+j]<<(state->p&7);
	  state->first_line[k][0][j] ^= state->first_line[k][0][(state->p>>3)+j+1]>>(8-(state->p&7));
	}
	for (j=1; j<8; j++) {
  	  state->first_line[k][j] = (unsigned char*) calloc(((state->p+state->r)>>3)+1,1);
  	  for (l=0; l<(state->p+state->r)>>3; l++) {
  	    state->first_line[k][j][l] ^= state->first_line[k][0][l] >> j;
  	    state->first_line[k][j][l+1] ^= state->first_line[k][0][l] << (8-j);
  	  }
	}	
      }

      state->syndrome = (unsigned char*) calloc(LUI(state->r),sizeof(unsigned int));
      state->new_syndrome = (unsigned int*) calloc(LUI(state->r),sizeof(unsigned int));
      state->buffer = (unsigned char*) malloc(state->inputsize>>3);
      state->count = 0;
      return SUCCESS;
    }
  }
  return BAD_HASHBITLEN;
}

HashReturn Update(hashState * state, const BitSequence *data, DataLength databitlen) {
  int tmp,i;
  unsigned char remaining;
  if (databitlen == 0) {
    return SUCCESS;
  }
  /* we check if this Update will fill one buffer */
  if(databitlen + state->count < state->inputsize) {
    /* we simply need to copy data to the buffer. Either it is aligned or not. */
    if((state->count & 7) == 0) {
      memcpy(&(state->buffer[state->count>>3]), data, ((databitlen-1)>>3) + 1);
      state->databitlen += databitlen;
      state->count += databitlen;
      return SUCCESS;
    } else {
      state->buffer[state->count>>3] ^= state->buffer[state->count>>3] & ((1<<(8-(state->count&7)))-1);
      for (i=0; i<=(databitlen>>3); i++) {
	state->buffer[(state->count>>3)+i] ^= data[i]>>(state->count&7);
	state->buffer[(state->count>>3)+i+1] = data[i]<<(8-(state->count&7));
      }
      state->databitlen += databitlen;
      state->count += databitlen;
      return SUCCESS;
    }
  } else {
    /* we fill up the buffer, perform a hash and recursively call Update */
    if((state->count & 7) == 0) {
      tmp = state->inputsize - state->count;
      memcpy(&(state->buffer[state->count>>3]), data, tmp>>3);
      state->databitlen += tmp;
      state->count += tmp;
      performHash(state);
      return Update(state, &(data[tmp>>3]), databitlen-tmp);
    } else {
      /* tmp contains the number of bits we have to read to fill up the buffer */
      tmp = state->inputsize - state->count;
      state->buffer[state->count>>3] ^= state->buffer[state->count>>3] & ((1<<(8-(state->count&7)))-1);
      for (i=0; i<(tmp>>3); i++) {
	state->buffer[(state->count>>3)+i] ^= data[i]>>(state->count&7);
	state->buffer[(state->count>>3)+i+1] = data[i]<<(8-(state->count&7));
      }
      state->buffer[(state->inputsize>>3)-1] ^= data[tmp>>3]>>(state->count&7);
      /* perform this round's hash */
      state->databitlen += tmp;
      state->count += tmp;
      performHash(state);
      /* we check if there are still some bits to input */
      if (databitlen > tmp) {
	/* we check if these bits are stored in more than the end of the byte data[tmp>>3] already read */
	if (databitlen > (((tmp>>3)+1)<<3)) {
	  /* we first re-input the remaining bits in data[tmp>>3] then perform the recursive call */
	  remaining = data[tmp>>3] << (tmp&7) ;
	  Update(state, &remaining, 8-(tmp&7));
	  return Update(state, &(data[tmp>>3]), databitlen-(((tmp>>3)+1)<<3));
	} else {
	  /* we simply input the remaining bits of data[tmp>>3] */
	  remaining = data[tmp>>3] << (tmp&7) ;
	  return Update(state, &remaining, databitlen - tmp);	  
	}
      } else {
	return SUCCESS;
      }
    }
  }
}

HashReturn Final(hashState * state, BitSequence *hashval) {
  unsigned char* padding, *whirlOutput;
  int i,j;
  struct NESSIEstruct * whirlpoolState;
  DataLength databitlen = state->databitlen;
  if (state->count+65 > state->inputsize) {
    padding = (unsigned char*) calloc(state->inputsize>>3,1);
    padding[0] = 1<<7;
    Update(state,padding,state->inputsize-state->count);
    padding[0] = 0;
    for (i=0; i<8; i++) {
      padding[(state->inputsize>>3)-1-i] = (unsigned char) (databitlen>>(8*i));
    }
    Update(state,padding,state->inputsize);
  } else {
    padding = (unsigned char*) calloc(((state->inputsize-state->count)>>3)+1,1);
    padding[0] = 1<<7;
    Update(state,padding,state->inputsize-state->count-64);
    for (i=0; i<8; i++) {
      padding[7-i] = (unsigned char) (databitlen>>(8*i)) ;
    }
    Update(state,padding,64);
  }
  free(padding);

  /* The final round of FSB is finished, now we simply apply the final transform: Whirlpool */
  whirlpoolState = (struct NESSIEstruct *) malloc(sizeof(struct NESSIEstruct));
  whirlOutput = (unsigned char*) malloc(64);
  NESSIEinit(whirlpoolState);
  
  NESSIEadd(state->syndrome,state->r,whirlpoolState);
  NESSIEfinalize(whirlpoolState, whirlOutput);
  
  for(i=0; i<(state->hashbitlen>>3); i++) {
    hashval[i] = whirlOutput[i];
  }

  /*
   * Now we also have to free all the memory allocated during Init
   */
  free(whirlOutput);
  free(whirlpoolState);
  for (i=0; i<state->b; i++) {
    for (j=0; j<8; j++) {
      free(state->first_line[i][j]);
    }
    free(state->first_line[i]);
  }
  free(state->first_line);
  free(state->syndrome);
  free(state->new_syndrome);
  free(state->buffer);
  return SUCCESS;
}

HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval) {
  hashState* state = (hashState*) malloc(sizeof(hashState));
  int return_value;
  return_value = Init(state, hashbitlen);
  if (return_value != SUCCESS) {
    return return_value;
  }
  return_value = Update(state, data, databitlen);
  if (return_value != SUCCESS) {
    return return_value;
  }
  return_value = Final(state, hashval);
  free(state);
  return return_value;  
}

HashReturn HashFile(int hashbitlen, FILE* file_to_hash, BitSequence* hashval) {
  int size, return_value;
  hashState* state = (hashState*) malloc(sizeof(hashState));
  unsigned char* buffer = (unsigned char*) malloc(4096*sizeof(unsigned char));

  return_value = Init(state, hashbitlen);
  if (return_value != SUCCESS) {
    return return_value;
  }
  
  while (!feof(file_to_hash)) {
    size = fread(buffer, 1, 4096, file_to_hash);
    return_value = Update(state, buffer, size<<3);
    if (return_value != SUCCESS) {
      return return_value;
    }
  }
  free(buffer);
  
  return_value = Final(state, hashval);
  free(state);
  return return_value;
}

void printHash(BitSequence* hashval, int hashbitlen) {
  int i;
  for (i=0; i<hashbitlen; i+=8) {
    printf("%.2x",(unsigned int) (hashval[i>>3]));
  }
}

void performHash(hashState * state) {
  int i,j,index,bidx,tmp;
  unsigned int * temp;
  for (i=0; i<LUI(state->r); i++) {
    state->new_syndrome[i] = 0;
  }
  for (i=0; i<state->w; i++) {
    index = i<<state->bpc;
    switch (state->bfiv) {
    case 2:
      index ^= (state->syndrome[i>>2]>>(6-((i&3)<<1)))&3;
      break;
    case 4:
      index ^= (state->syndrome[i>>1]>>(4-((i&1)<<2)))&15;
      break;
    case 8:
      index ^= state->syndrome[i];
      break;
    default:
      tmp = (i+1)*state->bfiv;
      for (j=i*state->bfiv; j<tmp; j++) {
	index ^= ((state->syndrome[j>>3]>>(7-(j&7)))&1)<<(tmp-j-1);
      }
    }
    
    switch (state->bfm) {
    case 2:
      index ^= ((unsigned int) ((state->buffer[i>>2]>>(6-((i&3)<<1)))&3)) << state->bfiv;
      break;
    case 4:
      index ^= ((unsigned int) ((state->buffer[i>>1]>>(4-((i&1)<<2)))&15)) << state->bfiv;
      break;
    case 8:
      index ^= ((unsigned int) state->buffer[i]) << state->bfiv;
      break;
    default:
      tmp = (i+1)*state->bfm;
      for (j=i*state->bfm; j<tmp; j++) {
	index ^= ((state->buffer[j>>3]>>(7-(j&7)))&1)<<(tmp-j-1+state->bfiv);
      }
    }
    
    bidx = index/state->r; /* index of the vector */
    index = index - bidx*state->r; /* shift to perform on the vector */

    /* we have finished computing the vector index and shift, now we XOR it! */
    temp = (unsigned int*) &(state->first_line[bidx][index&7][(state->r>>3)-(index>>3)]);
    for (j=state->r/(sizeof(int)<<3)-1; j>=0; j--) {
      state->new_syndrome[j] ^= temp[j];
    }
  }
  temp = state->new_syndrome;
  state->new_syndrome = (unsigned int*) state->syndrome;
  state->syndrome = (unsigned char*) temp;
  state->count = 0;
}

void printBuffer(hashState * state) {
  int i;
  printf("[%d] ", state->count);
  for (i=0; i<(state->count>>3); i++) {
    printf("%.2x",state->buffer[i]);
  }
  if ((state->count&7) != 0) {
    printf("%.2x",state->buffer[state->count>>3]);
  }
  printf("\n");
}

unsigned int logarithm(unsigned int a) {
  int i;
  for (i=0; i<32; i++) {
    if (a == (1<<i)) {
      return i;
    }
  }
  fprintf(stderr,"Error: bad parameters, log %u does not exists.\n",a);
  return -1;
}

void dumpTables(hashState * state) {
  int i,j,k;
  FILE* dumpFile;
  char dumpName[256];

  sprintf(dumpName, "tables_%d.h", state->hashbitlen);
  if (!(dumpFile = fopen(dumpName, "wb"))) {
    fprintf(stderr,"Error: could not write dumpTable file \"%s\".\n",dumpName);
    return;
  }
  fprintf(dumpFile, "unsigned char first_line_%d[%u] = {\n",state->hashbitlen, state->b*8*(((state->r+state->p)>>3)+1));
  for (i=0; i<state->b; i++) {
    for (j=0; j<8; j++) {
      for (k=0; k<((state->r+state->p)>>3)+1; k++) {
        fprintf(dumpFile, "0x%.2x,", state->first_line[i][j][k]);
      }
      fprintf(dumpFile, "\n");
    }
    fprintf(dumpFile, "\n");  
  }
  fseek(dumpFile, -3, SEEK_CUR);
  fprintf(dumpFile, "\n};\n");  
  fclose(dumpFile);
}
