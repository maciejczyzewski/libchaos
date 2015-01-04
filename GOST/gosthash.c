/*
 *  gosthash.c 
 *  21 Apr 1998  Markku-Juhani Saarinen <mjos@ssh.fi>
 * 
 *  GOST R 34.11-94, Russian Standard Hash Function
 *
 *  Copyright (c) 1998 SSH Communications Security, Finland
 *  All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gosthash.h"

/* lookup tables : each of these has two rotated 4-bit S-Boxes */

unsigned long gost_sbox_1[256];
unsigned long gost_sbox_2[256];
unsigned long gost_sbox_3[256];
unsigned long gost_sbox_4[256];

/* initialize the lookup tables */

void gosthash_init()
{
  int a, b, i;
  unsigned long ax, bx, cx, dx;
  
  /* 4-bit S-Boxes */ 
  
  unsigned long sbox[8][16] =
    {
	{  4, 10,  9,  2, 13,  8,  0, 14,  6, 11,  1, 12,  7, 15,  5,  3 },
	{ 14, 11,  4, 12,  6, 13, 15, 10,  2,  3,  8,  1,  0,  7,  5,  9 },
	{  5,  8,  1, 13, 10,  3,  4,  2, 14, 15, 12,  7,  6,  0,  9, 11 },
	{  7, 13, 10,  1,  0,  8,  9, 15, 14,  4,  6, 12, 11,  2,  5,  3 },
	{  6, 12,  7,  1,  5, 15, 13,  8,  4, 10,  9, 14,  0,  3, 11,  2 },
	{  4, 11, 10,  0,  7,  2,  1, 13,  3,  6,  8,  5,  9, 12, 15, 14 },
	{ 13, 11,  4,  1,  3, 15,  5,  9,  0, 10, 14,  7,  6,  8,  2, 12 },
	{  1, 15, 13,  0,  5,  7, 10,  4,  9,  2,  3, 14,  6, 11,  8, 12 }  
    };
  
  /* s-box precomputation */
  
  i = 0;
  for (a = 0; a < 16; a++)
    {
      ax = sbox[1][a] << 15;	  
      bx = sbox[3][a] << 23;
      cx = sbox[5][a];	      
      cx = (cx >> 1) | (cx << 31);
      dx = sbox[7][a] << 7;
      
      for (b = 0; b < 16; b++)
	{
	  gost_sbox_1[i] = ax | (sbox[0][b] << 11);		  
	  gost_sbox_2[i] = bx | (sbox[2][b] << 19);
	  gost_sbox_3[i] = cx | (sbox[4][b] << 27);	  
	  gost_sbox_4[i++] = dx | (sbox[6][b] << 3);
	}
    }
}

/*
 *  A macro that performs a full encryption round of GOST 28147-89.
 *  Temporary variable t assumed and variables r and l for left and right
 *  blocks
 */ 

#define GOST_ENCRYPT_ROUND(k1, k2) \
t = (k1) + r; \
l ^= gost_sbox_1[t & 0xff] ^ gost_sbox_2[(t >> 8) & 0xff] ^ \
gost_sbox_3[(t >> 16) & 0xff] ^ gost_sbox_4[t >> 24]; \
t = (k2) + l; \
r ^= gost_sbox_1[t & 0xff] ^ gost_sbox_2[(t >> 8) & 0xff] ^ \
gost_sbox_3[(t >> 16) & 0xff] ^ gost_sbox_4[t >> 24]; \

/* encrypt a block with the given key */

#define GOST_ENCRYPT(key) \
GOST_ENCRYPT_ROUND(key[0], key[1]) \
GOST_ENCRYPT_ROUND(key[2], key[3]) \
GOST_ENCRYPT_ROUND(key[4], key[5]) \
GOST_ENCRYPT_ROUND(key[6], key[7]) \
GOST_ENCRYPT_ROUND(key[0], key[1]) \
GOST_ENCRYPT_ROUND(key[2], key[3]) \
GOST_ENCRYPT_ROUND(key[4], key[5]) \
GOST_ENCRYPT_ROUND(key[6], key[7]) \
GOST_ENCRYPT_ROUND(key[0], key[1]) \
GOST_ENCRYPT_ROUND(key[2], key[3]) \
GOST_ENCRYPT_ROUND(key[4], key[5]) \
GOST_ENCRYPT_ROUND(key[6], key[7]) \
GOST_ENCRYPT_ROUND(key[7], key[6]) \
GOST_ENCRYPT_ROUND(key[5], key[4]) \
GOST_ENCRYPT_ROUND(key[3], key[2]) \
GOST_ENCRYPT_ROUND(key[1], key[0]) \
t = r; \
r = l; \
l = t;

/* 
 *  "chi" compression function. the result is stored over h
 */

void gosthash_compress(unsigned long *h, unsigned long *m)
{
  int i;
  unsigned long l, r, t, key[8], u[8], v[8], w[8], s[8];
  
  memcpy(u, h, sizeof(u));
  memcpy(v, m, sizeof(u));
  
  for (i = 0; i < 8; i += 2)
    {        
      w[0] = u[0] ^ v[0];	       /* w = u xor v */
      w[1] = u[1] ^ v[1];
      w[2] = u[2] ^ v[2];
      w[3] = u[3] ^ v[3];
      w[4] = u[4] ^ v[4];
      w[5] = u[5] ^ v[5];
      w[6] = u[6] ^ v[6];
      w[7] = u[7] ^ v[7];      
      
      /* P-Transformation */
      
      key[0] = (w[0]  & 0x000000ff) | ((w[2] & 0x000000ff) << 8) |
	((w[4] & 0x000000ff) << 16) | ((w[6] & 0x000000ff) << 24);
      key[1] = ((w[0] & 0x0000ff00) >> 8)  | (w[2]  & 0x0000ff00) |
	((w[4] & 0x0000ff00) << 8) | ((w[6] & 0x0000ff00) << 16);
      key[2] = ((w[0] & 0x00ff0000) >> 16) | ((w[2] & 0x00ff0000) >> 8) |
	(w[4] & 0x00ff0000) | ((w[6] & 0x00ff0000) << 8);
      key[3] = ((w[0] & 0xff000000) >> 24) | ((w[2] & 0xff000000) >> 16) |
	((w[4] & 0xff000000) >> 8) | (w[6] & 0xff000000);  
      key[4] = (w[1] & 0x000000ff) | ((w[3] & 0x000000ff) << 8) |
	((w[5] & 0x000000ff) << 16) | ((w[7] & 0x000000ff) << 24);
      key[5] = ((w[1] & 0x0000ff00) >> 8) | (w[3]  & 0x0000ff00) |
	((w[5] & 0x0000ff00) << 8) | ((w[7] & 0x0000ff00) << 16);
      key[6] = ((w[1] & 0x00ff0000) >> 16) | ((w[3] & 0x00ff0000) >> 8) |
	(w[5] & 0x00ff0000) | ((w[7] & 0x00ff0000) << 8);
      key[7] = ((w[1] & 0xff000000) >> 24) | ((w[3] & 0xff000000) >> 16) |
	((w[5] & 0xff000000) >> 8) | (w[7] & 0xff000000);  
            
      r = h[i];			       /* encriphering transformation */
      l = h[i + 1];      
      GOST_ENCRYPT(key);
      
      s[i] = r;
      s[i + 1] = l;
            
      if (i == 6)
	break;
      
      l = u[0] ^ u[2];		       /* U = A(U) */
      r = u[1] ^ u[3];
      u[0] = u[2];
      u[1] = u[3];
      u[2] = u[4];
      u[3] = u[5];
      u[4] = u[6];
      u[5] = u[7];
      u[6] = l;
      u[7] = r;
            
      if (i == 2)		       /* Constant C_3 */
	{
	  u[0] ^= 0xff00ff00; 
	  u[1] ^= 0xff00ff00; 
	  u[2] ^= 0x00ff00ff;
	  u[3] ^= 0x00ff00ff;
	  u[4] ^= 0x00ffff00;
	  u[5] ^= 0xff0000ff;
	  u[6] ^= 0x000000ff;
	  u[7] ^= 0xff00ffff;	    
	}
      
      l = v[0];			       /* V = A(A(V)) */
      r = v[2];
      v[0] = v[4];
      v[2] = v[6];
      v[4] = l ^ r;
      v[6] = v[0] ^ r;
      l = v[1];
      r = v[3];
      v[1] = v[5];
      v[3] = v[7];
      v[5] = l ^ r;
      v[7] = v[1] ^ r;
    }
  
  /* 12 rounds of the LFSR (computed from a product matrix) and xor in M */
  
  u[0] = m[0] ^ s[6];
  u[1] = m[1] ^ s[7];
  u[2] = m[2] ^ (s[0] << 16) ^ (s[0] >> 16) ^ (s[0] & 0xffff) ^ 
    (s[1] & 0xffff) ^ (s[1] >> 16) ^ (s[2] << 16) ^ s[6] ^ (s[6] << 16) ^
    (s[7] & 0xffff0000) ^ (s[7] >> 16);
  u[3] = m[3] ^ (s[0] & 0xffff) ^ (s[0] << 16) ^ (s[1] & 0xffff) ^ 
    (s[1] << 16) ^ (s[1] >> 16) ^ (s[2] << 16) ^ (s[2] >> 16) ^
    (s[3] << 16) ^ s[6] ^ (s[6] << 16) ^ (s[6] >> 16) ^ (s[7] & 0xffff) ^ 
    (s[7] << 16) ^ (s[7] >> 16);
  u[4] = m[4] ^ 
    (s[0] & 0xffff0000) ^ (s[0] << 16) ^ (s[0] >> 16) ^ 
    (s[1] & 0xffff0000) ^ (s[1] >> 16) ^ (s[2] << 16) ^ (s[2] >> 16) ^
    (s[3] << 16) ^ (s[3] >> 16) ^ (s[4] << 16) ^ (s[6] << 16) ^ 
    (s[6] >> 16) ^(s[7] & 0xffff) ^ (s[7] << 16) ^ (s[7] >> 16);
  u[5] = m[5] ^ (s[0] << 16) ^ (s[0] >> 16) ^ (s[0] & 0xffff0000) ^
    (s[1] & 0xffff) ^ s[2] ^ (s[2] >> 16) ^ (s[3] << 16) ^ (s[3] >> 16) ^
    (s[4] << 16) ^ (s[4] >> 16) ^ (s[5] << 16) ^  (s[6] << 16) ^ 
    (s[6] >> 16) ^ (s[7] & 0xffff0000) ^ (s[7] << 16) ^ (s[7] >> 16);
  u[6] = m[6] ^ s[0] ^ (s[1] >> 16) ^ (s[2] << 16) ^ s[3] ^ (s[3] >> 16) ^
    (s[4] << 16) ^ (s[4] >> 16) ^ (s[5] << 16) ^ (s[5] >> 16) ^ s[6] ^ 
    (s[6] << 16) ^ (s[6] >> 16) ^ (s[7] << 16);
  u[7] = m[7] ^ (s[0] & 0xffff0000) ^ (s[0] << 16) ^ (s[1] & 0xffff) ^ 
    (s[1] << 16) ^ (s[2] >> 16) ^ (s[3] << 16) ^ s[4] ^ (s[4] >> 16) ^
    (s[5] << 16) ^ (s[5] >> 16) ^ (s[6] >> 16) ^ (s[7] & 0xffff) ^ 
    (s[7] << 16) ^ (s[7] >> 16);
      
  /* 16 * 1 round of the LFSR and xor in H */
  
  v[0] = h[0] ^ (u[1] << 16) ^ (u[0] >> 16);
  v[1] = h[1] ^ (u[2] << 16) ^ (u[1] >> 16);
  v[2] = h[2] ^ (u[3] << 16) ^ (u[2] >> 16);
  v[3] = h[3] ^ (u[4] << 16) ^ (u[3] >> 16);
  v[4] = h[4] ^ (u[5] << 16) ^ (u[4] >> 16);
  v[5] = h[5] ^ (u[6] << 16) ^ (u[5] >> 16);
  v[6] = h[6] ^ (u[7] << 16) ^ (u[6] >> 16);
  v[7] = h[7] ^ (u[0] & 0xffff0000) ^ (u[0] << 16) ^ (u[7] >> 16) ^
    (u[1] & 0xffff0000) ^ (u[1] << 16) ^ (u[6] << 16) ^ (u[7] & 0xffff0000);
  
  /* 61 rounds of LFSR, mixing up h (computed from a product matrix) */

  h[0] = (v[0] & 0xffff0000) ^ (v[0] << 16) ^ (v[0] >> 16) ^ (v[1] >> 16) ^ 
    (v[1] & 0xffff0000) ^ (v[2] << 16) ^ (v[3] >> 16) ^ (v[4] << 16) ^
    (v[5] >> 16) ^ v[5] ^ (v[6] >> 16) ^ (v[7] << 16) ^ (v[7] >> 16) ^ 
    (v[7] & 0xffff);
  h[1] = (v[0] << 16) ^ (v[0] >> 16) ^ (v[0] & 0xffff0000) ^ (v[1] & 0xffff) ^ 
    v[2] ^ (v[2] >> 16) ^ (v[3] << 16) ^ (v[4] >> 16) ^ (v[5] << 16) ^ 
    (v[6] << 16) ^ v[6] ^ (v[7] & 0xffff0000) ^ (v[7] >> 16);
  h[2] = (v[0] & 0xffff) ^ (v[0] << 16) ^ (v[1] << 16) ^ (v[1] >> 16) ^ 
    (v[1] & 0xffff0000) ^ (v[2] << 16) ^ (v[3] >> 16) ^ v[3] ^ (v[4] << 16) ^
    (v[5] >> 16) ^ v[6] ^ (v[6] >> 16) ^ (v[7] & 0xffff) ^ (v[7] << 16) ^
    (v[7] >> 16);
  h[3] = (v[0] << 16) ^ (v[0] >> 16) ^ (v[0] & 0xffff0000) ^ 
    (v[1] & 0xffff0000) ^ (v[1] >> 16) ^ (v[2] << 16) ^ (v[2] >> 16) ^ v[2] ^ 
    (v[3] << 16) ^ (v[4] >> 16) ^ v[4] ^ (v[5] << 16) ^ (v[6] << 16) ^ 
    (v[7] & 0xffff) ^ (v[7] >> 16);
  h[4] = (v[0] >> 16) ^ (v[1] << 16) ^ v[1] ^ (v[2] >> 16) ^ v[2] ^ 
    (v[3] << 16) ^ (v[3] >> 16) ^ v[3] ^ (v[4] << 16) ^ (v[5] >> 16) ^ 
    v[5] ^ (v[6] << 16) ^ (v[6] >> 16) ^ (v[7] << 16);
  h[5] = (v[0] << 16) ^ (v[0] & 0xffff0000) ^ (v[1] << 16) ^ (v[1] >> 16) ^ 
    (v[1] & 0xffff0000) ^ (v[2] << 16) ^ v[2] ^ (v[3] >> 16) ^ v[3] ^ 
    (v[4] << 16) ^ (v[4] >> 16) ^ v[4] ^ (v[5] << 16) ^ (v[6] << 16) ^
    (v[6] >> 16) ^ v[6] ^ (v[7] << 16) ^ (v[7] >> 16) ^ (v[7] & 0xffff0000);
  h[6] = v[0] ^ v[2] ^ (v[2] >> 16) ^ v[3] ^ (v[3] << 16) ^ v[4] ^ 
    (v[4] >> 16) ^ (v[5] << 16) ^ (v[5] >> 16) ^ v[5] ^ (v[6] << 16) ^ 
    (v[6] >> 16) ^ v[6] ^ (v[7] << 16) ^ v[7];
  h[7] = v[0] ^ (v[0] >> 16) ^ (v[1] << 16) ^ (v[1] >> 16) ^ (v[2] << 16) ^
    (v[3] >> 16) ^ v[3] ^ (v[4] << 16) ^ v[4] ^ (v[5] >> 16) ^ v[5] ^
    (v[6] << 16) ^ (v[6] >> 16) ^ (v[7] << 16) ^ v[7];
}

/* Clear the state of the given context structure. */

void gosthash_reset(GostHashCtx *ctx)
{
  memset(ctx->sum, 0, 32);
  memset(ctx->hash, 0, 32);
  memset(ctx->len, 0, 32);
  memset(ctx->partial, 0, 32);
  ctx->partial_bytes = 0;  
}

/* Mix in a 32-byte chunk ("stage 3") */

void gosthash_bytes(GostHashCtx *ctx, const unsigned char *buf, size_t bits)
{
  int i, j;
  unsigned long a, b, c, m[8];
  
  /* convert bytes to a long words and compute the sum */
  
  j = 0;
  c = 0;
  for (i = 0; i < 8; i++)
    {
      a = ((unsigned long) buf[j]) | 
	(((unsigned long) buf[j + 1]) << 8) | 
	(((unsigned long) buf[j + 2]) << 16) | 
	(((unsigned long) buf[j + 3]) << 24);
      j += 4;
      m[i] = a;
      b = ctx->sum[i];
      c = a + c + ctx->sum[i];
      ctx->sum[i] = c;
      c = ((c < a) || (c < b)) ? 1 : 0;     
    }
    
  /* compress */
  
  gosthash_compress(ctx->hash, m);
  
  /* a 64-bit counter should be sufficient */
  
  ctx->len[0] += bits;
  if (ctx->len[0] < bits)
    ctx->len[1]++;  
}

/* Mix in len bytes of data for the given buffer. */

void gosthash_update(GostHashCtx *ctx, const unsigned char *buf, size_t len)
{
  size_t i, j;
  
  i = ctx->partial_bytes;
  j = 0;
  while (i < 32 && j < len)
    ctx->partial[i++] = buf[j++];
  
  if (i < 32)
    {
      ctx->partial_bytes = i;
      return;
    }  
  gosthash_bytes(ctx, ctx->partial, 256);
  
  while ((j + 32) < len)
    {
      gosthash_bytes(ctx, &buf[j], 256);
      j += 32;
    }
  
  i = 0;
  while (j < len)
    ctx->partial[i++] = buf[j++];
  ctx->partial_bytes = i;
}


/* Compute and save the 32-byte digest. */

void gosthash_final(GostHashCtx *ctx, unsigned char *digest)
{
  int i, j;
  unsigned long a;
  
  /* adjust and mix in the last chunk */
  
  if (ctx->partial_bytes > 0)
    {
      memset(&ctx->partial[ctx->partial_bytes], 0, 32 - ctx->partial_bytes);
      gosthash_bytes(ctx, ctx->partial, ctx->partial_bytes << 3);      
    }
  
  /* mix in the length and the sum */
  
  gosthash_compress(ctx->hash, ctx->len);  
  gosthash_compress(ctx->hash, ctx->sum);  
  
  /* convert the output to bytes */
  
  j = 0;
  for (i = 0; i < 8; i++)
    {
      a = ctx->hash[i];
      digest[j] = (unsigned char) a;
      digest[j + 1] = (unsigned char) (a >> 8);
      digest[j + 2] = (unsigned char) (a >> 16);
      digest[j + 3] = (unsigned char) (a >> 24);	
      j += 4;
    }  
}
