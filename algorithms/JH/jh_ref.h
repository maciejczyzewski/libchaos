/* This program gives the reference implementation of JH.
   It implements the standard description of JH (not bitslice)
   The description given in this program is suitable for hardware implementation

   --------------------------------
   Comparing to the original reference implementation,
   two functions are added to make the porgram more readable.
   One function is E8_initialgroup() at the beginning of E8;
   another function is E8_finaldegroup() at the end of E8.

   --------------------------------

   Last Modified: January 16, 2011
*/



#include <string.h>

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

typedef struct {
      int hashbitlen;                     /*the message digest size*/
      unsigned long long databitlen;      /*the message size in bits*/
      unsigned long long datasize_in_buffer;  /*the size of the message remained in buffer; assumed to be multiple of 8bits except for the last partial block at the end of the message*/
      unsigned char H[128];               /*the hash value H; 128 bytes;*/
      unsigned char A[256];               /*the temporary round value; 256 4-bit elements*/
      unsigned char roundconstant[64];    /*round constant for one round; 64 4-bit elements*/
      unsigned char buffer[64];           /*the message block to be hashed; 64 bytes*/
} hashState;

/*The constant for the Round 0 of E8*/
const unsigned char roundconstant_zero[64] = {0x6,0xa,0x0,0x9,0xe,0x6,0x6,0x7,0xf,0x3,0xb,0xc,0xc,0x9,0x0,0x8,0xb,0x2,0xf,0xb,0x1,0x3,0x6,0x6,0xe,0xa,0x9,0x5,0x7,0xd,0x3,0xe,0x3,0xa,0xd,0xe,0xc,0x1,0x7,0x5,0x1,0x2,0x7,0x7,0x5,0x0,0x9,0x9,0xd,0xa,0x2,0xf,0x5,0x9,0x0,0xb,0x0,0x6,0x6,0x7,0x3,0x2,0x2,0xa};

/*The two Sboxes S0 and S1*/
unsigned char S[2][16] = {{9,0,4,11,13,12,3,15,1,10,2,6,7,5,8,14},{3,12,6,13,5,7,1,9,15,2,0,4,11,10,14,8}};

/*The linear transformation L, the MDS code*/
#define L(a, b) {                                                       \
      (b) ^= ( ( (a) << 1) ^ ( (a) >> 3) ^ (( (a) >> 2) & 2) ) & 0xf;   \
      (a) ^= ( ( (b) << 1) ^ ( (b) >> 3) ^ (( (b) >> 2) & 2) ) & 0xf;   \
}

void R8(hashState *state);                    /* The round function of E8 */
void update_roundconstant(hashState *state);  /* Update the round constant of E8 */
void E8_initialgroup(hashState *state);       /* Grouping the state into 4-bit elements at the beginning of E8 */
void E8_finaldegroup(hashState *state);       /* Inverse of the grouping at the end of E8 */
void E8(hashState *state);                    /* The bijective function E8 */
void F8(hashState *state);                    /* The compression function F8 */

/*The API functions*/
HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data,DataLength databitlen, BitSequence *hashval);

/*the round function of E8 */
void R8(hashState *state)
{
      unsigned int i;
      unsigned char tem[256],t;
      unsigned char roundconstant_expanded[256];  /*the round constant expanded into 256 1-bit element;*/

      /*expand the round constant into 256 one-bit element*/
      for (i = 0; i < 256; i++)  {
            roundconstant_expanded[i] = (state->roundconstant[i >> 2] >> (3 - (i & 3)) ) & 1;
      }

      /*S box layer, each constant bit selects one Sbox from S0 and S1*/
      for (i = 0; i < 256; i++) {
            tem[i] = S[roundconstant_expanded[i]][state->A[i]]; /*constant bits are used to determine which Sbox to use*/
      }

      /*MDS Layer*/
      for (i = 0; i < 256; i=i+2) L(tem[i], tem[i+1]);

      /*The following is the permuation layer P_8$

      /*initial swap Pi_8*/
      for ( i = 0; i < 256; i=i+4) {
            t = tem[i+2];
            tem[i+2] = tem[i+3];
            tem[i+3] = t;
      }

      /*permutation P'_8*/
      for (i = 0; i < 128; i=i+1) {
            state->A[i] = tem[i<<1];
            state->A[i+128] = tem[(i<<1)+1];
      }

      /*final swap Phi_8*/
      for ( i = 128; i < 256; i=i+2) {
            t = state->A[i];
            state->A[i] = state->A[i+1];
            state->A[i+1] = t;
      }
}


/*The following function generates the next round constant from the current
  round constant;  R6 is used for generating round constants for E8, with
  the round constants of R6 being set as 0;
*/
void update_roundconstant(hashState *state) {
      int i;
      unsigned char tem[64],t;

      /*Sbox layer*/
      for (i = 0; i < 64; i++)   tem[i] = S[0][state->roundconstant[i]];

      /*MDS layer*/
      for (i = 0; i < 64; i=i+2) L(tem[i], tem[i+1]);

      /*The following is the permutation layer P_6 */

      /*initial swap Pi_6*/
      for ( i = 0; i < 64; i=i+4) {
            t = tem[i+2];
            tem[i+2] = tem[i+3];
            tem[i+3] = t;
      }

      /*permutation P'_6*/
      for ( i = 0; i < 32; i=i+1) {
            state->roundconstant[i]    = tem[i<<1];
            state->roundconstant[i+32] = tem[(i<<1)+1];
      }

      /*final swap Phi_6*/
      for ( i = 32; i < 64; i=i+2 ) {
            t = state->roundconstant[i];
            state->roundconstant[i] = state->roundconstant[i+1];
            state->roundconstant[i+1] = t;
      }
}

/*initial group at the begining of E_8: group the bits of H into 4-bit elements of A.
  After the grouping, the i-th, (i+256)-th, (i+512)-th, (i+768)-th bits of state->H
  become the i-th 4-bit element of state->A
*/
void E8_initialgroup(hashState *state) {
      unsigned int i;
      unsigned char t0,t1,t2,t3;
      unsigned char tem[256];

      /*t0 is the i-th bit of H, i = 0, 1, 2, 3, ... , 127*/
      /*t1 is the (i+256)-th bit of H*/
      /*t2 is the (i+512)-th bit of H*/
      /*t3 is the (i+768)-th bit of H*/
      for (i = 0; i < 256; i++) {
            t0 = (state->H[i>>3] >> (7 - (i & 7)) ) & 1;
            t1 = (state->H[(i+256)>>3] >> (7 - (i & 7)) ) & 1;
            t2 = (state->H[(i+ 512 )>>3] >> (7 - (i & 7)) ) & 1;
            t3 = (state->H[(i+ 768 )>>3] >> (7 - (i & 7)) ) & 1;
            tem[i] = (t0 << 3) | (t1 << 2) | (t2 << 1) | (t3 << 0);
      }
      /*padding the odd-th elements and even-th elements separately*/
      for (i = 0; i < 128; i++) {
            state->A[i << 1] = tem[i];
            state->A[(i << 1)+1] = tem[i+128];
      }
}

/*de-group at the end of E_8:  it is the inverse of E8_initialgroup
  The 256 4-bit elements in state->A are degouped into the 1024-bit state->H
*/
void E8_finaldegroup(hashState *state) {
      unsigned int i;
      unsigned char t0,t1,t2,t3;
      unsigned char tem[256];

      for (i = 0; i < 128; i++) {
            tem[i] = state->A[i << 1];
            tem[i+128] = state->A[(i << 1)+1];
      }

      for (i = 0; i < 128; i++) state->H[i] = 0;

      for (i = 0; i < 256; i++) {
            t0 = (tem[i] >> 3) & 1;
            t1 = (tem[i] >> 2) & 1;
            t2 = (tem[i] >> 1) & 1;
            t3 = (tem[i] >> 0) & 1;

            state->H[i>>3] |= t0 << (7 - (i & 7));
            state->H[(i + 256)>>3] |= t1 << (7 - (i & 7));
            state->H[(i + 512)>>3] |= t2 << (7 - (i & 7));
            state->H[(i + 768)>>3] |= t3 << (7 - (i & 7));
      }
}

/*bijective function E8 */
void E8(hashState *state)
{
      unsigned int i;
      unsigned char t0,t1,t2,t3;
      unsigned char tem[256];

      /*initialize the round constant*/
      for (i = 0; i < 64; i++) state->roundconstant[i] = roundconstant_zero[i];

      /*initial group at the begining of E_8: group the H value into 4-bit elements and store them in A */
      E8_initialgroup(state);

      /* 42 rounds */
      for (i = 0; i < 42; i++) {
            R8(state);
            update_roundconstant(state);
      }

      /*de-group at the end of E_8:  decompose the 4-bit elements of A into the 1024-bit H*/
      E8_finaldegroup(state);
}

/* compression function F8 */
void F8(hashState *state)
{
      unsigned int i;

      /*xor the message with the first half of H*/
      for (i = 0; i < 64; i++) state->H[i] ^= state->buffer[i];

      /* Bijective function E8 */
      E8(state);

      /* xor the message with the last half of H */
      for (i = 0; i < 64; i++) state->H[i+64] ^= state->buffer[i];
}

/*before hashing a message, initialize the hash state as H0 */
HashReturn Init(hashState *state, int hashbitlen)
{
      unsigned int i;

      state->databitlen = 0;
    state->datasize_in_buffer = 0;

      state->hashbitlen = hashbitlen;

      for (i = 0; i < 64; i++) state->buffer[i] = 0;
      for (i = 0; i < 128; i++) state->H[i] = 0;

      /*initialize the initial hash value of JH*/
      /*step 1: set H(-1) to the message digest size*/
      state->H[1] = hashbitlen & 0xff;
      state->H[0] = (hashbitlen >> 8) & 0xff;
    /*step 2: compute H0 from H(-1) with message M(0) being set as 0*/
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
      unsigned int i;

      if ( (state->databitlen & 0x1ff) == 0) {
            /*pad the message when databitlen is multiple of 512 bits, then process the padded block*/
            for (i = 0; i < 64; i++) state->buffer[i] = 0;
            state->buffer[0] = 0x80;
            state->buffer[63] = state->databitlen & 0xff;
            state->buffer[62] = (state->databitlen >> 8) & 0xff;
            state->buffer[61] = (state->databitlen >> 16) & 0xff;
            state->buffer[60] = (state->databitlen >> 24) & 0xff;
            state->buffer[59] = (state->databitlen >> 32) & 0xff;
            state->buffer[58] = (state->databitlen >> 40) & 0xff;
            state->buffer[57] = (state->databitlen >> 48) & 0xff;
            state->buffer[56] = (state->databitlen >> 56) & 0xff;
            F8(state);
      }
      else {
            /*set the rest of the bytes in the buffer to 0*/
            if ( (state->datasize_in_buffer & 7) == 0)
                   for (i = (state->databitlen & 0x1ff) >> 3; i < 64; i++)  state->buffer[i] = 0;
            else
                   for (i = ((state->databitlen & 0x1ff) >> 3)+1; i < 64; i++)  state->buffer[i] = 0;

            /*pad and process the partial block when databitlen is not multiple of 512 bits, then hash the padded blocks*/
            state->buffer[((state->databitlen & 0x1ff) >> 3)] |= 1 << (7- (state->databitlen & 7));
            F8(state);
            for (i = 0; i < 64; i++) state->buffer[i] = 0;
            state->buffer[63] = state->databitlen & 0xff;
            state->buffer[62] = (state->databitlen >> 8) & 0xff;
            state->buffer[61] = (state->databitlen >> 16) & 0xff;
            state->buffer[60] = (state->databitlen >> 24) & 0xff;
            state->buffer[59] = (state->databitlen >> 32) & 0xff;
            state->buffer[58] = (state->databitlen >> 40) & 0xff;
            state->buffer[57] = (state->databitlen >> 48) & 0xff;
            state->buffer[56] = (state->databitlen >> 56) & 0xff;
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


/* hash a message,
   three inputs: message digest size in bits (hashbitlen); message (data); message length in bits (databitlen)
   one output:   message digest (hashval)
*/
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
