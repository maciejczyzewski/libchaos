/********************************************************************\
 *
 *      FILE:     rmd160mc.c
 *
 *      CONTENTS: A sample C-implementation of the
 *                RIPEMD160-MAC function.
 *      TARGET:   any computer with an ANSI C compiler
 *
 *      AUTHOR:   Antoon Bosselaers, ESAT-COSIC
 *      DATE:     26 March 1998
 *      VERSION:  1.0
 *
 *      Copyright (c) Katholieke Universiteit Leuven
 *      1998, All Rights Reserved
 *
\********************************************************************/

/*  header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rmd160mc.h"

/* constants T0, T1, T2 specific for RIPEMD160-MAC */
static dword T[3][4];

/***********************************************************************/
void MDMACconstT(void)
/*
   calculates T0, T1, T2 required for RIPEMD160-MAC
   this has to be done only once
*/
{
   dword         MDbuf[5];
   unsigned int  i, j;
   byte          U[65] = "00abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   dword         K[9], X[16];

   K[0] = 0x67452301UL;
   K[1] = 0xefcdab89UL;
   K[2] = 0x98badcfeUL;
   K[3] = 0x10325476UL;
   K[4] = 0xc3d2e1f0UL;
   for (i=5; i<9 ;i++)
      K[i] = 0;

   for (i=0; i<3; i++) {
      U[0] = 0x30 + i;
      U[1] = U[0];
      MDMACinit(K, MDbuf);
      for (j=0; j<16; j++)
         X[j] = BYTES_TO_DWORD(U+4*j);
      compress(K, MDbuf, X);
      T[i][0] = MDbuf[0];
      T[i][1] = MDbuf[1];
      T[i][2] = MDbuf[2];
      T[i][3] = MDbuf[3];
   }

}

/***********************************************************************/
dword *MDMACsetup(byte *key)
/*
   expands 128-bit key into (5+4+4)*32-bit K required for RIPEMD160-MAC
*/
{
   unsigned int i;
   dword        U[16];
   dword        KK[9];
   static dword K[14];

   KK[0] = 0x67452301UL;
   KK[1] = 0xefcdab89UL;
   KK[2] = 0x98badcfeUL;
   KK[3] = 0x10325476UL;
   KK[4] = 0xc3d2e1f0UL;
   for (i=5; i<9 ;i++)
      KK[i] = 0;

   MDMACinit(KK, K);
   for (i=0; i<4 ; i++)  {
      U[i]    = BYTES_TO_DWORD(key+4*i);
      U[i+4]  = T[0][i];
      U[i+8]  = T[1][i];
      U[i+12] = T[2][i];
   }
   compress(KK, K, U);
   for (i=0; i<4 ; i++)  {
      U[i]    = T[0][i];
      U[i+4]  = T[1][i];
      U[i+8]  = T[2][i];
      U[i+12] = BYTES_TO_DWORD(key+4*i);
   }
   compress(KK, K, U);

   MDMACinit(KK, K+5);
   for (i=0; i<4 ; i++)  {
      U[i]    = BYTES_TO_DWORD(key+4*i);
      U[i+4]  = T[1][i];
      U[i+8]  = T[2][i];
      U[i+12] = T[0][i];
   }
   compress(KK, K+5, U);
   for (i=0; i<4 ; i++)  {
      U[i]    = T[1][i];
      U[i+4]  = T[2][i];
      U[i+8]  = T[0][i];
      U[i+12] = BYTES_TO_DWORD(key+4*i);
   }
   compress(KK, K+5, U);

   MDMACinit(KK, K+9);
   for (i=0; i<4 ; i++)  {
      U[i]    = BYTES_TO_DWORD(key+4*i);
      U[i+4]  = T[2][i];
      U[i+8]  = T[0][i];
      U[i+12] = T[1][i];
   }
   compress(KK, K+9, U);
   for (i=0; i<4 ; i++)  {
      U[i]    = T[2][i];
      U[i+4]  = T[0][i];
      U[i+8]  = T[1][i];
      U[i+12] = BYTES_TO_DWORD(key+4*i);
   }
   compress(KK, K+9, U);

   return K;
}

/********************************************************************/

void MDMACinit(dword *K, dword *MDbuf)
{
   MDbuf[0] = K[0];
   MDbuf[1] = K[1];
   MDbuf[2] = K[2];
   MDbuf[3] = K[3];
   MDbuf[4] = K[4];

   return;
}

/********************************************************************/

void compress(dword *K, dword *MDbuf, dword *X)
{
   dword aa = MDbuf[0],  bb = MDbuf[1],  cc = MDbuf[2],
         dd = MDbuf[3],  ee = MDbuf[4];
   dword aaa = MDbuf[0], bbb = MDbuf[1], ccc = MDbuf[2],
         ddd = MDbuf[3], eee = MDbuf[4];

   /* round 1 */
   FF(aa, bb, cc, dd, ee, X[ 0]+K[5], 11);
   FF(ee, aa, bb, cc, dd, X[ 1]+K[5], 14);
   FF(dd, ee, aa, bb, cc, X[ 2]+K[5], 15);
   FF(cc, dd, ee, aa, bb, X[ 3]+K[5], 12);
   FF(bb, cc, dd, ee, aa, X[ 4]+K[5],  5);
   FF(aa, bb, cc, dd, ee, X[ 5]+K[5],  8);
   FF(ee, aa, bb, cc, dd, X[ 6]+K[5],  7);
   FF(dd, ee, aa, bb, cc, X[ 7]+K[5],  9);
   FF(cc, dd, ee, aa, bb, X[ 8]+K[5], 11);
   FF(bb, cc, dd, ee, aa, X[ 9]+K[5], 13);
   FF(aa, bb, cc, dd, ee, X[10]+K[5], 14);
   FF(ee, aa, bb, cc, dd, X[11]+K[5], 15);
   FF(dd, ee, aa, bb, cc, X[12]+K[5],  6);
   FF(cc, dd, ee, aa, bb, X[13]+K[5],  7);
   FF(bb, cc, dd, ee, aa, X[14]+K[5],  9);
   FF(aa, bb, cc, dd, ee, X[15]+K[5],  8);

   /* round 2 */
   GG(ee, aa, bb, cc, dd, X[ 7]+K[6],  7);
   GG(dd, ee, aa, bb, cc, X[ 4]+K[6],  6);
   GG(cc, dd, ee, aa, bb, X[13]+K[6],  8);
   GG(bb, cc, dd, ee, aa, X[ 1]+K[6], 13);
   GG(aa, bb, cc, dd, ee, X[10]+K[6], 11);
   GG(ee, aa, bb, cc, dd, X[ 6]+K[6],  9);
   GG(dd, ee, aa, bb, cc, X[15]+K[6],  7);
   GG(cc, dd, ee, aa, bb, X[ 3]+K[6], 15);
   GG(bb, cc, dd, ee, aa, X[12]+K[6],  7);
   GG(aa, bb, cc, dd, ee, X[ 0]+K[6], 12);
   GG(ee, aa, bb, cc, dd, X[ 9]+K[6], 15);
   GG(dd, ee, aa, bb, cc, X[ 5]+K[6],  9);
   GG(cc, dd, ee, aa, bb, X[ 2]+K[6], 11);
   GG(bb, cc, dd, ee, aa, X[14]+K[6],  7);
   GG(aa, bb, cc, dd, ee, X[11]+K[6], 13);
   GG(ee, aa, bb, cc, dd, X[ 8]+K[6], 12);

   /* round 3 */
   HH(dd, ee, aa, bb, cc, X[ 3]+K[7], 11);
   HH(cc, dd, ee, aa, bb, X[10]+K[7], 13);
   HH(bb, cc, dd, ee, aa, X[14]+K[7],  6);
   HH(aa, bb, cc, dd, ee, X[ 4]+K[7],  7);
   HH(ee, aa, bb, cc, dd, X[ 9]+K[7], 14);
   HH(dd, ee, aa, bb, cc, X[15]+K[7],  9);
   HH(cc, dd, ee, aa, bb, X[ 8]+K[7], 13);
   HH(bb, cc, dd, ee, aa, X[ 1]+K[7], 15);
   HH(aa, bb, cc, dd, ee, X[ 2]+K[7], 14);
   HH(ee, aa, bb, cc, dd, X[ 7]+K[7],  8);
   HH(dd, ee, aa, bb, cc, X[ 0]+K[7], 13);
   HH(cc, dd, ee, aa, bb, X[ 6]+K[7],  6);
   HH(bb, cc, dd, ee, aa, X[13]+K[7],  5);
   HH(aa, bb, cc, dd, ee, X[11]+K[7], 12);
   HH(ee, aa, bb, cc, dd, X[ 5]+K[7],  7);
   HH(dd, ee, aa, bb, cc, X[12]+K[7],  5);

   /* round 4 */
   II(cc, dd, ee, aa, bb, X[ 1]+K[8], 11);
   II(bb, cc, dd, ee, aa, X[ 9]+K[8], 12);
   II(aa, bb, cc, dd, ee, X[11]+K[8], 14);
   II(ee, aa, bb, cc, dd, X[10]+K[8], 15);
   II(dd, ee, aa, bb, cc, X[ 0]+K[8], 14);
   II(cc, dd, ee, aa, bb, X[ 8]+K[8], 15);
   II(bb, cc, dd, ee, aa, X[12]+K[8],  9);
   II(aa, bb, cc, dd, ee, X[ 4]+K[8],  8);
   II(ee, aa, bb, cc, dd, X[13]+K[8],  9);
   II(dd, ee, aa, bb, cc, X[ 3]+K[8], 14);
   II(cc, dd, ee, aa, bb, X[ 7]+K[8],  5);
   II(bb, cc, dd, ee, aa, X[15]+K[8],  6);
   II(aa, bb, cc, dd, ee, X[14]+K[8],  8);
   II(ee, aa, bb, cc, dd, X[ 5]+K[8],  6);
   II(dd, ee, aa, bb, cc, X[ 6]+K[8],  5);
   II(cc, dd, ee, aa, bb, X[ 2]+K[8], 12);

   /* round 5 */
   JJ(bb, cc, dd, ee, aa, X[ 4]+K[5],  9);
   JJ(aa, bb, cc, dd, ee, X[ 0]+K[5], 15);
   JJ(ee, aa, bb, cc, dd, X[ 5]+K[5],  5);
   JJ(dd, ee, aa, bb, cc, X[ 9]+K[5], 11);
   JJ(cc, dd, ee, aa, bb, X[ 7]+K[5],  6);
   JJ(bb, cc, dd, ee, aa, X[12]+K[5],  8);
   JJ(aa, bb, cc, dd, ee, X[ 2]+K[5], 13);
   JJ(ee, aa, bb, cc, dd, X[10]+K[5], 12);
   JJ(dd, ee, aa, bb, cc, X[14]+K[5],  5);
   JJ(cc, dd, ee, aa, bb, X[ 1]+K[5], 12);
   JJ(bb, cc, dd, ee, aa, X[ 3]+K[5], 13);
   JJ(aa, bb, cc, dd, ee, X[ 8]+K[5], 14);
   JJ(ee, aa, bb, cc, dd, X[11]+K[5], 11);
   JJ(dd, ee, aa, bb, cc, X[ 6]+K[5],  8);
   JJ(cc, dd, ee, aa, bb, X[15]+K[5],  5);
   JJ(bb, cc, dd, ee, aa, X[13]+K[5],  6);

   /* parallel round 1 */
   JJJ(aaa, bbb, ccc, ddd, eee, X[ 5]+K[6],  8);
   JJJ(eee, aaa, bbb, ccc, ddd, X[14]+K[6],  9);
   JJJ(ddd, eee, aaa, bbb, ccc, X[ 7]+K[6],  9);
   JJJ(ccc, ddd, eee, aaa, bbb, X[ 0]+K[6], 11);
   JJJ(bbb, ccc, ddd, eee, aaa, X[ 9]+K[6], 13);
   JJJ(aaa, bbb, ccc, ddd, eee, X[ 2]+K[6], 15);
   JJJ(eee, aaa, bbb, ccc, ddd, X[11]+K[6], 15);
   JJJ(ddd, eee, aaa, bbb, ccc, X[ 4]+K[6],  5);
   JJJ(ccc, ddd, eee, aaa, bbb, X[13]+K[6],  7);
   JJJ(bbb, ccc, ddd, eee, aaa, X[ 6]+K[6],  7);
   JJJ(aaa, bbb, ccc, ddd, eee, X[15]+K[6],  8);
   JJJ(eee, aaa, bbb, ccc, ddd, X[ 8]+K[6], 11);
   JJJ(ddd, eee, aaa, bbb, ccc, X[ 1]+K[6], 14);
   JJJ(ccc, ddd, eee, aaa, bbb, X[10]+K[6], 14);
   JJJ(bbb, ccc, ddd, eee, aaa, X[ 3]+K[6], 12);
   JJJ(aaa, bbb, ccc, ddd, eee, X[12]+K[6],  6);

   /* parallel round 2 */
   III(eee, aaa, bbb, ccc, ddd, X[ 6]+K[7],  9);
   III(ddd, eee, aaa, bbb, ccc, X[11]+K[7], 13);
   III(ccc, ddd, eee, aaa, bbb, X[ 3]+K[7], 15);
   III(bbb, ccc, ddd, eee, aaa, X[ 7]+K[7],  7);
   III(aaa, bbb, ccc, ddd, eee, X[ 0]+K[7], 12);
   III(eee, aaa, bbb, ccc, ddd, X[13]+K[7],  8);
   III(ddd, eee, aaa, bbb, ccc, X[ 5]+K[7],  9);
   III(ccc, ddd, eee, aaa, bbb, X[10]+K[7], 11);
   III(bbb, ccc, ddd, eee, aaa, X[14]+K[7],  7);
   III(aaa, bbb, ccc, ddd, eee, X[15]+K[7],  7);
   III(eee, aaa, bbb, ccc, ddd, X[ 8]+K[7], 12);
   III(ddd, eee, aaa, bbb, ccc, X[12]+K[7],  7);
   III(ccc, ddd, eee, aaa, bbb, X[ 4]+K[7],  6);
   III(bbb, ccc, ddd, eee, aaa, X[ 9]+K[7], 15);
   III(aaa, bbb, ccc, ddd, eee, X[ 1]+K[7], 13);
   III(eee, aaa, bbb, ccc, ddd, X[ 2]+K[7], 11);

   /* parallel round 3 */
   HHH(ddd, eee, aaa, bbb, ccc, X[15]+K[8],  9);
   HHH(ccc, ddd, eee, aaa, bbb, X[ 5]+K[8],  7);
   HHH(bbb, ccc, ddd, eee, aaa, X[ 1]+K[8], 15);
   HHH(aaa, bbb, ccc, ddd, eee, X[ 3]+K[8], 11);
   HHH(eee, aaa, bbb, ccc, ddd, X[ 7]+K[8],  8);
   HHH(ddd, eee, aaa, bbb, ccc, X[14]+K[8],  6);
   HHH(ccc, ddd, eee, aaa, bbb, X[ 6]+K[8],  6);
   HHH(bbb, ccc, ddd, eee, aaa, X[ 9]+K[8], 14);
   HHH(aaa, bbb, ccc, ddd, eee, X[11]+K[8], 12);
   HHH(eee, aaa, bbb, ccc, ddd, X[ 8]+K[8], 13);
   HHH(ddd, eee, aaa, bbb, ccc, X[12]+K[8],  5);
   HHH(ccc, ddd, eee, aaa, bbb, X[ 2]+K[8], 14);
   HHH(bbb, ccc, ddd, eee, aaa, X[10]+K[8], 13);
   HHH(aaa, bbb, ccc, ddd, eee, X[ 0]+K[8], 13);
   HHH(eee, aaa, bbb, ccc, ddd, X[ 4]+K[8],  7);
   HHH(ddd, eee, aaa, bbb, ccc, X[13]+K[8],  5);

   /* parallel round 4 */
   GGG(ccc, ddd, eee, aaa, bbb, X[ 8]+K[5], 15);
   GGG(bbb, ccc, ddd, eee, aaa, X[ 6]+K[5],  5);
   GGG(aaa, bbb, ccc, ddd, eee, X[ 4]+K[5],  8);
   GGG(eee, aaa, bbb, ccc, ddd, X[ 1]+K[5], 11);
   GGG(ddd, eee, aaa, bbb, ccc, X[ 3]+K[5], 14);
   GGG(ccc, ddd, eee, aaa, bbb, X[11]+K[5], 14);
   GGG(bbb, ccc, ddd, eee, aaa, X[15]+K[5],  6);
   GGG(aaa, bbb, ccc, ddd, eee, X[ 0]+K[5], 14);
   GGG(eee, aaa, bbb, ccc, ddd, X[ 5]+K[5],  6);
   GGG(ddd, eee, aaa, bbb, ccc, X[12]+K[5],  9);
   GGG(ccc, ddd, eee, aaa, bbb, X[ 2]+K[5], 12);
   GGG(bbb, ccc, ddd, eee, aaa, X[13]+K[5],  9);
   GGG(aaa, bbb, ccc, ddd, eee, X[ 9]+K[5], 12);
   GGG(eee, aaa, bbb, ccc, ddd, X[ 7]+K[5],  5);
   GGG(ddd, eee, aaa, bbb, ccc, X[10]+K[5], 15);
   GGG(ccc, ddd, eee, aaa, bbb, X[14]+K[5],  8);

   /* parallel round 5 */
   FFF(bbb, ccc, ddd, eee, aaa, X[12]+K[6],  8);
   FFF(aaa, bbb, ccc, ddd, eee, X[15]+K[6],  5);
   FFF(eee, aaa, bbb, ccc, ddd, X[10]+K[6], 12);
   FFF(ddd, eee, aaa, bbb, ccc, X[ 4]+K[6],  9);
   FFF(ccc, ddd, eee, aaa, bbb, X[ 1]+K[6], 12);
   FFF(bbb, ccc, ddd, eee, aaa, X[ 5]+K[6],  5);
   FFF(aaa, bbb, ccc, ddd, eee, X[ 8]+K[6], 14);
   FFF(eee, aaa, bbb, ccc, ddd, X[ 7]+K[6],  6);
   FFF(ddd, eee, aaa, bbb, ccc, X[ 6]+K[6],  8);
   FFF(ccc, ddd, eee, aaa, bbb, X[ 2]+K[6], 13);
   FFF(bbb, ccc, ddd, eee, aaa, X[13]+K[6],  6);
   FFF(aaa, bbb, ccc, ddd, eee, X[14]+K[6],  5);
   FFF(eee, aaa, bbb, ccc, ddd, X[ 0]+K[6], 15);
   FFF(ddd, eee, aaa, bbb, ccc, X[ 3]+K[6], 13);
   FFF(ccc, ddd, eee, aaa, bbb, X[ 9]+K[6], 11);
   FFF(bbb, ccc, ddd, eee, aaa, X[11]+K[6], 11);

   /* combine results */
   ddd += cc + MDbuf[1];               /* final result for MDbuf[0] */
   MDbuf[1] = MDbuf[2] + dd + eee;
   MDbuf[2] = MDbuf[3] + ee + aaa;
   MDbuf[3] = MDbuf[4] + aa + bbb;
   MDbuf[4] = MDbuf[0] + bb + ccc;
   MDbuf[0] = ddd;

   return;
}

/********************************************************************/

void MDMACfinish(dword *K, dword *MDbuf, byte *strptr,
                 dword lswlen, dword mswlen)
{
   unsigned int i;                                 /* counter       */
   dword        X[16];                             /* message words */

   memset(X, 0, 16*sizeof(dword));

   /* put bytes from strptr into X */
   for (i=0; i<(lswlen&63); i++) {
      /* byte i goes into word X[i div 4] at pos.  8*(i mod 4)  */
      X[i>>2] ^= (dword) *strptr++ << (8 * (i&3));
   }

   /* append the bit m_n == 1 */
   X[(lswlen>>2)&15] ^= (dword)1 << (8*(lswlen&3) + 7);

   if ((lswlen & 63) > 55) {
      /* length goes to next block */
      compress(K, MDbuf, X);
      memset(X, 0, 16*sizeof(dword));
   }

   /* append length in bits*/
   X[14] = lswlen << 3;
   X[15] = (lswlen >> 29) | (mswlen << 3);
   compress(K, MDbuf, X);

   /* last block */
   for (i=0; i<4; i++) {
      X[i]    = K[9+i];
      X[i+4]  = K[9+i] ^ T[0][i];
      X[i+8]  = K[9+i] ^ T[1][i];
      X[i+12] = K[9+i] ^ T[2][i];
   }
   compress(K, MDbuf, X);

   return;
}

/*********************** end of file rmd160mc.c *********************/
