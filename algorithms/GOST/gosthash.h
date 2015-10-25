/*
 *  gosthash.h 
 *  21 Apr 1998  Markku-Juhani Saarinen <mjos@ssh.fi>
 * 
 *  GOST R 34.11-94, Russian Standard Hash Function 
 *  header with function prototypes.
 *
 *  Copyright (c) 1998 SSH Communications Security, Finland
 *  All rights reserved.                    
 */

#ifndef GOSTHASH_H
#define GOSTHASH_H

#include <stdlib.h>

/* State structure */

typedef struct 
{
  unsigned long sum[8];
  unsigned long hash[8];
  unsigned long len[8];
  unsigned char partial[32];
  size_t partial_bytes;  
} GostHashCtx;
  
/* Compute some lookup-tables that are needed by all other functions. */

void gosthash_init();     

/* Clear the state of the given context structure. */

void gosthash_reset(GostHashCtx *ctx);  

/* Mix in len bytes of data for the given buffer. */

void gosthash_update(GostHashCtx *ctx, const unsigned char *buf, size_t len);

/* Compute and save the 32-byte digest. */

void gosthash_final(GostHashCtx *ctx, unsigned char *digest);

#endif /* GOSTHASH_H */
