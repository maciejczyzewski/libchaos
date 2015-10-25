/*This program gives the 8-bit optimized implementation of JH.
  The target is to achieve small codes size and relatively fast speed.
  It follows the standard (not bit-slice) description of JH.

  Last Modified: January 16, 2011

  ---------------------------------------------
  The following techniques are used to reduce the code size:
  a) The 1024-bit initial hash values are computed from the message digest sizes.
  b) The round constants are generated on the fly
  c) In each "for" loop, only one term is processed. (no loop unrolling)

  The following techniques are used to improve the speed:
  a) The Sbox and MDS are combined together into lookup tables. (these tables takes 64 bytes)
  b) The Permutation P8 is implemented so that those three sub-permutations are combined together

*/

#include <string.h>

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

typedef struct {
      int hashbitlen;                     /*the message digest size*/
      unsigned long long databitlen;      /*the message size in bits*/
      unsigned int  datasize_in_buffer;    /*the size of the message remained in buffer; assumed to be multiple of 8bits except for the last partial block at the end of the message*/
      unsigned char H[128];               /*the hash state; 128 bytes = 256 4-bit elements*/
      unsigned char roundconstant[32];    /*round constant for one round; 32 bytes = 64 4-bit elements*/
      unsigned char buffer[64];           /*the message block to be hashed; 64 bytes*/
} hashState;

void R8(hashState *state);                    /* The round function of E8 */
void update_roundconstant(hashState *state);  /* Update the round constant of E8 */
void E8_initialgroup(hashState *state);       /* Grouping the state into 4-bit elements at the beginning of E8 */
void E8_finaldegroup(hashState *state);       /* Inverse of the grouping at the end of E8 */
void E8(hashState *state);                    /* The bijective function E8 */
void F8(hashState *state);                    /* The compression function F8 */

HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data,DataLength databitlen, BitSequence *hashval);

/*The round constant for the Round 0 of E8*/
const unsigned char roundconstant_zero[32] = {0x6a,0x9,0xe6,0x67,0xf3,0xbc,0xc9,0x8,0xb2,0xfb,0x13,0x66,0xea,0x95,0x7d,0x3e,0x3a,0xde,0xc1,0x75,0x12,0x77,0x50,0x99,0xda,0x2f,0x59,0xb,0x6,0x67,0x32,0x2a};

/*The two Sboxes S0 and S1*/
/*
  unsigned char S[2][16] = {{9,0,4,11,13,12,3,15,1,10,2,6,7,5,8,14},{3,12,6,13,5,7,1,9,15,2,0,4,11,10,14,8}};
*/

/*combine the Sbox and the MDS transform into one table
    SboxMDS_A[i][j] = MDS(S[i][j],0)
    SboxMDS_B[i][j] = MDS(0,S[i][j])
*/
unsigned char SboxMDS_A[2][16] = {{0xb1,0x0,0x78,0x15,0xc9,0x9b,0xf6,0x6d,0x52,0x47,0xa4,0xdc,0x8e,0x2a,0xe3,0x3f}, {0xf6,0x9b,0xdc,0xc9,0x2a,0x8e,0x52,0xb1,0x6d,0xa4,0x0,0x78,0x15,0x47,0x3f,0xe3}};
unsigned char SboxMDS_B[2][16] = {{0x19,0x0,0x84,0x5b,0x9d,0xbc,0x63,0xdf,0x21,0x7a,0x42,0xc6,0xe7,0xa5,0x38,0xfe}, {0x63,0xbc,0xc6,0x9d,0xa5,0xe7,0x21,0x19,0xdf,0x42,0x0,0x84,0x5b,0x7a,0xfe,0x38}};

/* round function R8 */
void R8(hashState *state)
{
      unsigned char i,t;
      unsigned char tem[128];
      unsigned char roundconstantbit0, roundconstantbit1;

      /*S box layer and MDS layer, the Sbox and MDS are combined into one table lookup*/
      /*Each rounstant byte consists of eight bits, each bit selects an Sbox*/
      /*S box layer and MDS layer*/
      for (i = 0; i < 128; i++) {
            t = 7 - ((i << 1) & 7);
            roundconstantbit0 = (state->roundconstant[i >> 2] >> t ) & 1;
            roundconstantbit1 = (state->roundconstant[i >> 2] >> (t - 1) ) & 1;
            tem[i] = SboxMDS_A[roundconstantbit0][state->H[i] >> 4] ^ SboxMDS_B[roundconstantbit1][state->H[i] & 0xf]; /*constant bits are used to determine which Sbox to use*/
      }

      /* The following is the permuation layer P_8 */
      for (i = 0; i < 128; i=i+2) {
            state->H[i>>1] = (tem[i] & 0xf0) | (tem[i+1] & 0x0f);
            state->H[(i>>1)+64] = (tem[i] & 0x0f) | (tem[i+1] & 0xf0);
      }
}

/*The following function generates the next round constant from
  the current round constant;  R6 is used for generating round constants for E8, and
  the round constants of R6 is set as 0;
*/
void update_roundconstant(hashState *state) {
      unsigned char i;
      unsigned char tem[32];

      /* S box layer and MDS layer */
      for (i = 0; i < 32; i++) tem[i] = SboxMDS_A[0][state->roundconstant[i] >> 4] ^ SboxMDS_B[0][state->roundconstant[i] & 0xf]; /*constant bits are set as 0*/

      /* The following is the permuation layer P_6 */
      for (i = 0; i < 32; i=i+2) {
            state->roundconstant[i>>1] = (tem[i] & 0xf0) | (tem[i+1] & 0x0f);
            state->roundconstant[(i>>1)+16] = (tem[i] & 0x0f) | (tem[i+1] & 0xf0);
      }
}


/*initial group at the begining of E_8: group the H bits into 4-bit elements.
  After the grouping, the i-th, (i+256)-th, (i+512)-th, (i+768)-th, (i+128)-th
  (i+256+128)-th, (i+512+128)-th, (i+768+128)-th bits of H would become the i-th byte of H
*/
void E8_initialgroup(hashState *state) {
      unsigned char i,u,t;
      unsigned char t0,t1,t2,t3,t4,t5,t6,t7;
      unsigned char tem[128];

      /*t0 is the i-th bit of H, i = 0, 1, 2, 3, ... , 127*/
      /*t1 is the (i+256)-th bit of H*/
      /*t2 is the (i+512)-th bit of H*/
      /*t3 is the (i+768)-th bit of H*/
      /*t4 is the (i+128)-th bit of H*/
      /*t5 is the (i+256+128)-th bit of H*/
      /*t6 is the (i+512+128)-th bit of H*/
      /*t7 is the (i+768+128)-th bit of H*/

     for (i = 0; i < 128; i++) {
            t = 7 - (i & 7) ;
            u = i >> 3;
            t0 = (state->H[u] >> t ) & 1;
            t1 = (state->H[u+32] >> t ) & 1;
            t2 = (state->H[u+64] >> t ) & 1;
            t3 = (state->H[u+96] >> t ) & 1;
            t4 = (state->H[u+16] >> t ) & 1;
            t5 = (state->H[u+48] >> t ) & 1;
            t6 = (state->H[u+80] >> t ) & 1;
            t7 = (state->H[u+112] >> t ) & 1;
            tem[i] = (t0 << 7) | (t1 << 6) | (t2 << 5) | (t3 << 4) | (t4 << 3) | (t5 << 2) | (t6 << 1) | (t7 << 0);
      }

      memcpy(state->H, tem, 128);
}

/* de-group at the end of E_8:  it is the inverse of E8_initialgroup

  In this function, eight bytes are processed in each "for" loop,
  It can be implemented to process one byte in each "for" loop.
*/
void E8_finaldegroup(hashState *state) {
      unsigned char i,t;
      unsigned char t0,t1,t2,t3,t4,t5,t6,t7;
      unsigned char tem[128];

      memset(tem,0,128);

      for (i = 0; i < 128; i++) {
            t0 = (state->H[i] >> 7) & 1;
            t1 = (state->H[i] >> 6) & 1;
            t2 = (state->H[i] >> 5) & 1;
            t3 = (state->H[i] >> 4) & 1;
            t4 = (state->H[i] >> 3) & 1;
            t5 = (state->H[i] >> 2) & 1;
            t6 = (state->H[i] >> 1) & 1;
            t7 = (state->H[i] >> 0) & 1;

            t = 7 - (i & 7);
            tem[i>>3] |= t0 << t;
            tem[(i + 256)>>3] |= t1 << t;
            tem[(i + 512)>>3] |= t2 << t;
            tem[(i + 768)>>3] |= t3 << t;
            tem[(i + 128)>>3] |= t4 << t;
            tem[(i + 256 + 128)>>3] |= t5 << t;
            tem[(i + 512 + 128)>>3] |= t6 << t;
            tem[(i + 768 + 128)>>3] |= t7 << t;
      }
      memcpy(state->H, tem, 128);
}

/* Bijective function E8 */
void E8(hashState *state)
{
      unsigned char i,j;
      unsigned char u,t,t0,t1,t2,t3,t4,t5,t6,t7;

      /*initial group at the begining of E_8: group the H bits into 4-bit elements.*/
      E8_initialgroup(state);

      /* 42 rounds */
      for (i = 0; i < 42; i++) {
            R8(state);
            update_roundconstant(state);
      }

      /*de-group at the end of E_8:  decompose the 4-bit elements*/
      E8_finaldegroup(state);
}

/* compression function F8 */
void F8(hashState *state)
{
      unsigned char i;

      /*initialize the round constant*/
      memcpy(state->roundconstant, roundconstant_zero, 32);

      /*xor the message with the first half of H*/
      for (i = 0; i < 64; i++) state->H[i] ^= state->buffer[i];

      /* Bijective function E8 */
      E8(state);

      /* xor the message with the last half of H */
      for (i = 0; i < 64; i++) state->H[i+64] ^= state->buffer[i];
}


HashReturn Init(hashState *state, int hashbitlen)
{
      state->databitlen = 0;
      state->datasize_in_buffer = 0;

      state->hashbitlen = hashbitlen;

      memset(state->buffer,0,64) ;
      memset(state->H,0,128) ;

      /*initialize the initial hash value of JH*/
      state->H[1] = hashbitlen & 0xff;
      state->H[0] = (hashbitlen >> 8) & 0xff;

      F8(state);

      return(SUCCESS);
}


/*hash each 512-bit message block, except the last partial block*/
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen)
{
      DataLength index; /*the starting address of the data to be compressed*/

      state->databitlen += databitlen;
      index = 0;

      /*if there is remaining data in the buffer, fill it to a full message block first*/
      /*we assume that the size of the data in the buffer is the multiple of 8 bits if it is not at the end of a message*/

      /*There is data in the buffer, but the incoming data is insufficient for a full block*/
      if ( (state->datasize_in_buffer > 0 ) && (( state->datasize_in_buffer + databitlen) < 512)  ) {
            if ( (databitlen & 7) == 0 ) {
                 memcpy(state->buffer + (state->datasize_in_buffer >> 3), data, 64-(state->datasize_in_buffer >> 3)) ;
        }
            else memcpy(state->buffer + (state->datasize_in_buffer >> 3), data, 64-(state->datasize_in_buffer >> 3)+1) ;
            state->datasize_in_buffer += databitlen;
            databitlen = 0;
      }

      /*There is data in the buffer, and the incoming data is sufficient for a full block*/
      if ( (state->datasize_in_buffer > 0 ) && (( state->datasize_in_buffer + databitlen) >= 512)  ) {
          memcpy( state->buffer + (state->datasize_in_buffer >> 3), data, 64-(state->datasize_in_buffer >> 3) ) ;
          index = 64-(state->datasize_in_buffer >> 3);
          databitlen = databitlen - (512 - state->datasize_in_buffer);
          F8(state);
          state->datasize_in_buffer = 0;
      }

      /*hash the remaining full message blocks*/
      for ( ; databitlen >= 512; index = index+64, databitlen = databitlen - 512) {
            memcpy(state->buffer, data+index, 64);
            F8(state);
      }

      /*store the partial block into buffer, assume that -- if part of the last byte is not part of the message, then that part consists of 0 bits*/
      if ( databitlen > 0) {
            if ((databitlen & 7) == 0)
                  memcpy(state->buffer, data+index, (databitlen & 0x1ff) >> 3);
            else
                  memcpy(state->buffer, data+index, ((databitlen & 0x1ff) >> 3)+1);
            state->datasize_in_buffer = databitlen;
      }

      return(SUCCESS);
}

/*padding the message, truncate the hash value H and obtain the message digest*/
HashReturn Final(hashState *state, BitSequence *hashval)
{
      unsigned char i;

      if ( (state->databitlen & 0x1ff) == 0) {
            /*pad the message when databitlen is multiple of 512 bits, then process the padded block*/
            memset(state->buffer, 0, 64);
            state->buffer[0] = 0x80;
            for (i = 0; i < 8; i++) state->buffer[63-i] = (state->databitlen >> (i << 3)) & 0xff;
            F8(state);
      }
      else {
        /*set the rest of the bytes in the buffer to 0*/
      if ( (state->datasize_in_buffer & 7) == 0)
           for (i = (state->databitlen & 0x1ff) >> 3; i < 64; i++)  state->buffer[i] = 0;
            else
                 for (i = ((state->databitlen & 0x1ff) >> 3)+1; i < 64; i++)  state->buffer[i] = 0;

            /*pad and process the partial block*/
            state->buffer[((state->databitlen & 0x1ff) >> 3)] |= 1 << (7- (state->databitlen & 7));
            F8(state);
            memset(state->buffer, 0, 64);
            /*precess the last padded block*/
            for (i = 0; i < 8; i++) state->buffer[63-i] = (state->databitlen >> (i << 3)) & 0xff;
            F8(state);
      }

      /*trunacting the final hash value to generate the message digest*/
      switch (state->hashbitlen) {
            case 224:  memcpy(hashval,state->H+100,28);  break;
            case 256:  memcpy(hashval,state->H+96, 32);  break;
            case 384:  memcpy(hashval,state->H+80, 48);  break;
            case 512:  memcpy(hashval,state->H+64, 64);  break;
      }

      return(SUCCESS);
}

/*hash the message*/
HashReturn Hash(int hashbitlen, const BitSequence *data,DataLength databitlen, BitSequence *hashval)
{
      hashState state;

      if ( hashbitlen == 224 || hashbitlen == 256 || hashbitlen == 384 || hashbitlen == 512 ) {
            Init(&state, hashbitlen);
            Update(&state, data, databitlen);
            Final(&state, hashval);
            return SUCCESS;
      }
      else
            return(BAD_HASHLEN);
}

