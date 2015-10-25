/*
 *  gostan.c 
 *  26 Apr 1998  Markku-Juhani Saarinen <mjos@ssh.fi>
 * 
 *  Will print the linear dependecy matrices of 
 *  GOST R 34.11-94, Russian Standard Hash Function
 *
 *  Copyright (c) 1998 SSH Communications Security, Finland
 *  All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gosthash.h"

typedef unsigned char byte;

/* key generation matrix:  key no - key - h | m | 0xff   (bytes) */
byte keygen_matrix[4][32][65];

/* mixing function matrix:  h' - h | m | s   (16-bit words) */
byte mixing_matrix[16][48];

/* these are in gosthash.c */

extern unsigned long gost_sbox_1[256];
extern unsigned long gost_sbox_2[256];
extern unsigned long gost_sbox_3[256];
extern unsigned long gost_sbox_4[256];

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

/* Compute the key generation matrix */

void gen_keygen_matrix()
{
  int i, j, z;
  unsigned long l, r, key[4][8], u[8], v[8], w[8];
  
  memset(keygen_matrix, 0, sizeof(keygen_matrix));

  for (z = 64; z >= 0; z--)
    {
      memset(u, 0, 32);
      memset(v, 0, 32);
       
      if (z < 64)
	{	  
	  if (z < 32)
	    u[z >> 2] = 0xff << ((z & 3) << 3);
	  else
	    v[(z - 32) >> 2] = 0xff << ((z & 3) << 3);
	}
        
      for (i = 0; i < 4; i ++)
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
	  
	  key[i][0] = (w[0]  & 0x000000ff) | ((w[2] & 0x000000ff) << 8) |
	    ((w[4] & 0x000000ff) << 16) | ((w[6] & 0x000000ff) << 24);
	  key[i][1] = ((w[0] & 0x0000ff00) >> 8)  | (w[2]  & 0x0000ff00) |
	    ((w[4] & 0x0000ff00) << 8) | ((w[6] & 0x0000ff00) << 16);
	  key[i][2] = ((w[0] & 0x00ff0000) >> 16) | ((w[2] & 0x00ff0000) >> 8) |
	    (w[4] & 0x00ff0000) | ((w[6] & 0x00ff0000) << 8);
	  key[i][3] = ((w[0] & 0xff000000) >> 24) | ((w[2] & 0xff000000) >> 16) |
	    ((w[4] & 0xff000000) >> 8) | (w[6] & 0xff000000);  
	  key[i][4] = (w[1] & 0x000000ff) | ((w[3] & 0x000000ff) << 8) |
	    ((w[5] & 0x000000ff) << 16) | ((w[7] & 0x000000ff) << 24);
	  key[i][5] = ((w[1] & 0x0000ff00) >> 8) | (w[3]  & 0x0000ff00) |
	    ((w[5] & 0x0000ff00) << 8) | ((w[7] & 0x0000ff00) << 16);
	  key[i][6] = ((w[1] & 0x00ff0000) >> 16) | ((w[3] & 0x00ff0000) >> 8) |
	    (w[5] & 0x00ff0000) | ((w[7] & 0x00ff0000) << 8);
	  key[i][7] = ((w[1] & 0xff000000) >> 24) | ((w[3] & 0xff000000) >> 16) |
	    ((w[5] & 0xff000000) >> 8) | (w[7] & 0xff000000);  
	  
	  if (i == 3)
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
	  
	  if (i == 1)		       /* Constant C_3 */
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
      
      for (i = 0; i < 4; i++)
	{
	  for (j = 0; j < 32; j += 4)
	    {
	      l = key[i][j >> 2];
	      
	      keygen_matrix[i][j][z] = (l & 0xff ? 1 : 0) ^ 
		keygen_matrix[i][j][64];
	      keygen_matrix[i][j + 1][z] = ((l >> 8) & 0xff ? 1 : 0) ^
		keygen_matrix[i][j + 1][64];
	      keygen_matrix[i][j + 2][z] = ((l >> 16) & 0xff ? 1 : 0) ^
		keygen_matrix[i][j + 2][64];
	      keygen_matrix[i][j + 3][z] = ((l >> 24) & 0xff ? 1 : 0) ^
		keygen_matrix[i][j + 3][64];
	    }  
	}     
    }      
}

/* print the key generation matrix */

void print_keygen_matrix()
{
  int i, j, k;
  
  for (i = 0; i < 4; i++)
    {
      printf("\n Key %d\n\n", i);

      printf("    H                                "
	     "M                                C\n");
      
      printf("    00000000000000001111111111111111 "
	     "00000000000000001111111111111111 0\n");
      
      printf("    0123456789abcdef0123456789abcdef "
	     "0123456789abcdef0123456789abcdef 0\n\n");

      for (j = 0; j < 32; j++)
	{
	  printf("%2x  ", j); 
	  for (k = 0; k < 65; k++)
	    {
	      if (k == 32 || k == 64)
		putchar(' ');
	      putchar(keygen_matrix[i][j][k] ? '1' : '0');
	    }
	  printf("\n");
	}
      printf("\n\n");      
    }
}

/* compute the mixing matrix */

void gen_mixing_matrix()
{
  int i, j;
  unsigned long u[8], v[8], m[8], h[8], s[8];
  
  for (i = 0; i < 48; i++)
    {
      memset(h, 0, 32);
      memset(m, 0, 32);
      memset(s, 0, 32);
      
      if (i < 16)
	h[i >> 1] = 1 << (16 * (i & 1));
      else
	{	  
	  if (i < 32)
	    m[(i - 16) >> 1] = 1 << (16 * (i & 1));
	  else
	    s[(i - 32) >> 1] = 1 << (16 * (i & 1));
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

      for (j = 0; j < 16; j += 2)
	{
	  mixing_matrix[j][i] = h[j >> 1] & 0xffff ? 1 : 0;
	  mixing_matrix[j + 1 ][i] = h[j >> 1] >> 16 ? 1 : 0;	  
	}      
    }
}

/* print the mixing matrix */

void print_mixing_matrix()
{
 
  int i, j;
  
  printf("    H                M                S\n");
  printf("    0123456789abcdef 0123456789abcdef 0123456789abcdef\n\n");

  for (i = 0; i < 16; i++)
    {
      printf("%2x  ", i); 
      for (j = 0; j < 48; j++)
	{
	  if (j == 16 || j == 32)
	    putchar(' ');
	  putchar(mixing_matrix[i][j] ? '1' : '0');
	}
      printf("\n");      
    }  
}

#define PRINT_VEC(label, v) \
printf("%5s  %08lx %08lx %08lx %08lx  %08lx %08lx %08lx %08lx\n", \
label, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); 

/* compression function using the matrices -- for testing */

void matrix_compress(unsigned long *h, unsigned long *m)
{
  int i, j, k;
  unsigned long key[8], s[8], v[65], t, r, l;
  
  /* compute s */
  
  for (i = 0; i < 32 ; i += 4)
    {      
      v[i] = h[i >> 2] & 0xff;
      v[i+1] = (h[i >> 2] >> 8) & 0xff;
      v[i+2] = (h[i >> 2] >> 16) & 0xff;
      v[i+3] = h[i >> 2] >> 24;
      v[i+32] = m[i >> 2] & 0xff;
      v[i+33] = (m[i >> 2] >> 8) & 0xff;
      v[i+34] = (m[i >> 2] >> 16) & 0xff;
      v[i+35] = m[i >> 2] >> 24;
    }
      
  for (i = 0; i < 4; i++)
    {
      memset(key, 0, 32);
      
      for (j = 0; j < 32; j++)
	{
	  t = keygen_matrix[i][j][64] ? 0xff : 0;
	  for (k = 0; k < 64; k++)
	    {
	      if (keygen_matrix[i][j][k])
		t ^= v[k]; 
	    }
	  key[j >> 2] ^= t << ((j & 3) << 3);	  
	}    
      
      r = h[i << 1];          
      l = h[(i << 1) + 1];      
      GOST_ENCRYPT(key);      
      s[i << 1] = r;
      s[(i << 1) + 1] = l;       
    }
  
  /* final mixing */
  
  for (i = 0; i < 16; i += 2)
    {
      v[i] = h[i >> 1] & 0xffff;
      v[i + 1] = h[i >> 1] >> 16;
      v[i + 16] = m[i >> 1] & 0xffff;
      v[i + 17] = m[i >> 1] >> 16;
      v[i + 32] = s[i >> 1] & 0xffff;
      v[i + 33] = s[i >> 1] >> 16;      
    }
  
  memset(h, 0, 32);
  
  for (i = 0; i < 16; i++)
    {
      t = 0;
      for (j = 0; j < 48; j++)
	{
	  if (mixing_matrix[i][j])
	    t ^= v[j];	    
	}
      h[i >> 1] ^= t << ((i & 1) << 4);
    }
  
}

/*
 *  this is in gosthash.c -- prototyped here to eliminate a compiler
 *  warning 
 */
void gosthash_compress(unsigned long *h, unsigned long *m);

/*
 *  test that the matrices are ok by running random data through
 *  the compression function
 */

void test_matrices()
{
  int i, j;
  unsigned long h1[8], h2[8], m[8];
      
  for (i = 0; i < 100; i++)
    {
      for (j = 0; j < 8; j++)
	{	  
	  h1[j] = h2[j] = rand();
	  m[j] = rand(); 
	}
      
      gosthash_compress(h1, m);
      matrix_compress(h2, m);
            
      if (memcmp(h1, h2, 32) != 0)
	{
	  fprintf(stderr, "test_matrices() failed during round %d.\n", i);
	  exit(-1);	  	  
	}
    }  
  
  printf("Matrices are ok.\n");  
}

int main(int argc, char **argv)
{  
  srand(time(NULL));  
  
  gosthash_init();    
  gen_keygen_matrix();
  gen_mixing_matrix();
  test_matrices();

  print_keygen_matrix();
  print_mixing_matrix();
  
  return 0;
}
