#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shash.h"

typedef struct chunk {
    scell word[128];
} chunk;

typedef struct s_column {
    scell cell[8];
} s_column;

typedef struct s_row {
    scell cell[4];
} s_row;

typedef struct p_column {
    pcell cell[8];
} p_column;

typedef struct p_row {
    pcell cell[4];
} p_row;

typedef struct digest {
    unsigned char byte[64];
    int length;
} digest;

/*A lookup table for computing the inverse in GF 2^4*/
const __uint8_t xInverseTable[16] = {0,1,15,10,8,6,5,9,4,7,3,14,13,12,11,2};

/*A lookup table for computing the affine transform*/
const __uint8_t affineTransformTable[16] = {7,0,8,2,12,4,13,11,10,3,14,15,6,1,5,9}; 

/*s-hash generation tables*/
const scell sg128[4] = {0x01, 0x02, 0x04, 0x08};
const scell sg160[4] = {0x03, 0x02, 0x04, 0x08};
const scell sg192[4] = {0x03, 0x06, 0x04, 0x08};
const scell sg224[4] = {0x03, 0x06, 0x0c, 0x08};
const scell sg256[4] = {0x03, 0x07, 0x0c, 0x08};
const scell sg288[4] = {0x03, 0x07, 0x0e, 0x08};
const scell sg320[4] = {0x03, 0x07, 0x0e, 0x0c};
const scell sg352[4] = {0x07, 0x07, 0x0e, 0x0c};
const scell sg384[4] = {0x07, 0x0f, 0x0e, 0x0c};
const scell sg416[4] = {0x07, 0x0f, 0x0f, 0x0c};
const scell sg448[4] = {0x07, 0x0f, 0x0f, 0x0e};
const scell sg480[4] = {0x0f, 0x0f, 0x0f, 0x0e};
const scell sg512[4] = {0x0f, 0x0f, 0x0f, 0x0f};

/*Returns the last 4 bits of a byte*/
unsigned char get_last_word(unsigned char curChar){
    char fourOnes = 0x0f;
    return (curChar & fourOnes);
}
/*Returns the first 4 bits of a byte*/
unsigned char get_first_word(unsigned char curChar){
    return (curChar >> 4);
}

/*Fills the s prism with a 512 bit chunk of data*/
int fill_s_prism(s_prism *prism, chunk *chunk){
    int i, j, k;
    int counter = 0;
    
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                prism->cell[i][j][k] = chunk->word[counter];
                counter++;
            }
        }
    }
    return 0;
}
/*Initialization the h prism with zeros*/
void init_h_prism(h_prism *prism){
    int i, j, k;
    
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                prism->cell[i][j][k] = 0;
            }
        }
    }
    return;
}
/*Initializes the p prism with the numbers between 0 and 127*/
void init_p_prism(p_prism *prism){
    int i, j, k;
    int counter = 0;
    
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                prism->cell[i][j][k] = counter;
                counter++;
            }
        }
    }
    return;
}

/*Adds the length of the input data to the end of the last chunk*/
void add_length(chunk *curChunk, long int length){
    int i;
    __uint64_t fourOnes = 0x0f;
    for(i = 0; i < 16; i++){
        curChunk->word[112 + i] = (scell)(((fourOnes << (4*(15 - i))) & length) >> (4*(15 - i)));
    }
}

int s_set_ij_column(s_prism *prism, int i, int j, s_column column){
    int k;
    for(k = 0; k < 8; k++){
      prism->cell[i][j][k] = column.cell[k];
    }
    return 0;
}

int s_set_ik_row(s_prism *prism, int i, int k, s_row row){
    int j;
    for(j = 0; j < 4; j++){
      prism->cell[i][j][k] = row.cell[j];
    }
    return 0;
}

int s_set_jk_row(s_prism *prism, int j, int k, s_row row){
    int i;
    for(i = 0; i < 4; i++){
      prism->cell[i][j][k] = row.cell[i];
    }
    return 0;
}

s_column s_get_ij_column(s_prism *prism, int i, int j){
    s_column temp;
    int k;
    for(k = 0; k < 8; k++){
      temp.cell[k] = prism->cell[i][j][k];
    }
    return temp;
}

s_row s_get_ik_row(s_prism *prism, int i, int k){
    s_row temp;
    int j;
    for(j = 0; j < 4; j++){
      temp.cell[j] = prism->cell[i][j][k];
    }
    return temp;
}

s_row s_get_jk_row(s_prism *prism, int j, int k){
    s_row temp;
    int i;
    for(i = 0; i < 4; i++){
      temp.cell[i] = prism->cell[i][j][k];
    }
    return temp;
}

/*Calculates a four-point fourier transform*/
s_row four_point_DFT(s_row row){
    int a, b, c, d;
    s_row return_row;
    /* calculates the 2-point DFT operations */
    a = row.cell[0] + row.cell[2];
    b = row.cell[0] - row.cell[2];
    c = row.cell[1] + row.cell[3];
    d = row.cell[1] - row.cell[3];

    /* "% 17 + 17) % 17;" is used to ensure that the values being returned are positive */
    return_row.cell[0] = ((a + c) % 17 + 17) % 17;
    return_row.cell[1] = ((b + d * 4) % 17 + 17) % 17;
    return_row.cell[2] = ((a - c) % 17 + 17) % 17;
    return_row.cell[3] = ((b - d * 4) % 17 + 17) % 17;
    return return_row;
}
/*Calculates a eight-point fourier transform*/
s_column eight_point_DFT(s_column column){
    int temp[8];
    int temp2[8];
    s_column return_column;
   
    /* perform the 2-point DFT operations */
    temp2[0] = column.cell[0] + column.cell[4];
    temp2[1] = column.cell[0] - column.cell[4];
    temp2[2] = column.cell[2] + column.cell[6];
    temp2[3] = (column.cell[2] - column.cell[6]) * 4;
    temp2[4] = column.cell[1] + column.cell[5];
    temp2[5] = column.cell[1] - column.cell[5];
    temp2[6] = column.cell[3] + column.cell[7];
    temp2[7] = (column.cell[3] - column.cell[7]) * 4;
    
    /* perform the 4-point DFT on the first 4 elements of the temp */
    temp[0] = temp2[0] + temp2[2];
    temp[1] = temp2[1] + temp2[3];
    temp[2] = temp2[0] - temp2[2];
    temp[3] = temp2[1] - temp2[3];
    
    /* perform the 4-point DFT on the last 4 elements of the temp */
    temp[4] = temp2[4] + temp2[6];
    temp[5] = (temp2[5] + temp2[7]) * 2;
    temp[6] = (temp2[4] - temp2[6]) * 4;
    temp[7] = (temp2[5] - temp2[7]) * 8;
    
    /* perform the 8-point DFT operations */
    /* "% 17 + 17) % 17" is used to ensure that the values being returned are positive */
    return_column.cell[0] = ((temp[0] + temp[4]) % 17 + 17) % 17;
    return_column.cell[1] = ((temp[1] + temp[5]) % 17 + 17) % 17;
    return_column.cell[2] = ((temp[2] + temp[6]) % 17 + 17) % 17;
    return_column.cell[3] = ((temp[3] + temp[7]) % 17 + 17) % 17;
    return_column.cell[4] = ((temp[0] - temp[4]) % 17 + 17) % 17;
    return_column.cell[5] = ((temp[1] - temp[5]) % 17 + 17) % 17;
    return_column.cell[6] = ((temp[2] - temp[6]) % 17 + 17) % 17;
    return_column.cell[7] = ((temp[3] - temp[7]) % 17 + 17) % 17;
    return return_column;
}
/*Returns the least 2 significant bits of a four bit word*/
char get_word_sl(__uint8_t word){
    char temp = 0;
    char three = 0x03; /* 00000011 */
    temp = word & three;
    return temp;
}
/*Returns the most 2 significant bits of a four bit word*/
char get_word_sh(__uint8_t word){
    char temp = 0;
    char three = 0x03; /* 00000011 */
    temp = (word >> 2) & three;
    return temp;
}
/*Swaps the p prism based upon the first s prism*/
int initial_swap_control(s_prism *sPrism, p_prism *pPrism){
    int i;
    int j;
    int k;
    pcell temp;
    int sh, sl;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                /* get the most significant 2 bits of the 4 bit word at sPrism->cell[i][j][k] */
                sh = (int)get_word_sh(sPrism->cell[i][j][k]); 
                /* get the least significant 2 bits of the 4 bit word at sPrism->cell[i][j][k] */
                sl = (int)get_word_sl(sPrism->cell[i][j][k]);
                temp = pPrism->cell[i][j][k];
                pPrism->cell[i][j][k] = pPrism->cell[sh][sl][k];
                pPrism->cell[sh][sl][k] = temp;
            }
        }
    }
    return 0;
}
/*Performs the affine transform described by the affine transform table*/
void affine_transform(s_prism *prism){
    int i, j, k;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                if (prism->cell[i][j][k] > 15) {
                    fprintf(stderr, "affine transform lookup out of range: %x (%i, %i, %i)\n", prism->cell[i][j][k], i, j, k);
                }
                prism->cell[i][j][k] = affineTransformTable[(int)prism->cell[i][j][k]];
            }
        }
    }
    return;
}
/*Performs a swap on the p prism based upon the s prism*/
int k1_swap(s_prism *sPrism, p_prism *pPrism){
    char one = 0x01;
    char current = 0;
    pcell temp;
    int i;
    int j;
    int b;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
        /* xor the values from floor 0 and floor 4 in the s prism to perform the p prism swap */
        current = (sPrism->cell[i][j][0] ^ sPrism->cell[i][j][4]);
            for(b = 0; b < 4; b++){
                if(((current >> (b)) & one) == 0){
                    temp = pPrism->cell[i][j][b];
                    pPrism->cell[i][j][b] = pPrism->cell[i][j][7 - b];
                    pPrism->cell[i][j][7 - b] = temp;
                }
            } 
        }
    }
    return 0;
}
/*Performs a swap on the p prism based upon the s prism*/
int k2_swap(s_prism *sPrism, p_prism *pPrism){
    char one = 0x01;
    scell current;
    pcell temp;
    int i;
    int j;
    /* use floor 1 for swaping from planes 0 - 3 */
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            current = sPrism->cell[i][j][1];
            if((current & one) == 0){
                temp = pPrism->cell[i][j][0];
                pPrism->cell[i][j][0] = pPrism->cell[i][j][1];
                pPrism->cell[i][j][1] = temp;
            }
            if(((current >> 1) & one) == 0){
                temp = pPrism->cell[i][j][2];
                pPrism->cell[i][j][2] = pPrism->cell[i][j][3];
                pPrism->cell[i][j][3] = temp;
            }
            if(((current >> 2) & one) == 0){
                temp = pPrism->cell[i][j][1];
                pPrism->cell[i][j][1] = pPrism->cell[i][j][2];
                pPrism->cell[i][j][2] = temp;
            }
            if(((current >> 3) & one) == 0){
                temp = pPrism->cell[i][j][0];
                pPrism->cell[i][j][0] = pPrism->cell[i][j][3];
                pPrism->cell[i][j][3] = temp;
            }
        }
    }
    /* use floor 5 to swap the planes 4 - 7 */   
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            current = sPrism->cell[i][j][5];
            if((current & one) == 0){
                temp = pPrism->cell[i][j][4];
                pPrism->cell[i][j][4] = pPrism->cell[i][j][5];
                pPrism->cell[i][j][5] = temp;
            }
            if(((current >> 1) & one) == 0){
                temp = pPrism->cell[i][j][6];
                pPrism->cell[i][j][6] = pPrism->cell[i][j][7];
                pPrism->cell[i][j][7] = temp;
            }
            if(((current >> 2) & one) == 0){
                temp = pPrism->cell[i][j][5];
                pPrism->cell[i][j][5] = pPrism->cell[i][j][6];
                pPrism->cell[i][j][6] = temp;
            }
            if(((current >> 3) & one) == 0){
                temp = pPrism->cell[i][j][4];
                pPrism->cell[i][j][4] = pPrism->cell[i][j][7];
                pPrism->cell[i][j][7] = temp;
            }
        }
    }
    return 0;
}
/*Performs a swap on the p prism based upon the s prism*/
int j_swap(s_prism *sPrism, p_prism *pPrism){
    char one = 0x01;
    scell current;
    pcell temp;
    int i;
    int k;
    for(i = 0; i < 4; i++){
        for(k = 0; k < 8; k++){
        current = sPrism->cell[i][2][k];
            if(((current & one) ^ (k & one)) == 0){
                temp = pPrism->cell[i][0][k];
                pPrism->cell[i][0][k] = pPrism->cell[i][1][k];
                pPrism->cell[i][1][k] = temp;
            }
            if((((current >> 1) & one) ^ ((k >> 1) & one)) == 0) {
                temp = pPrism->cell[i][2][k];
                pPrism->cell[i][2][k] = pPrism->cell[i][3][k];
                pPrism->cell[i][3][k] = temp;
            }
            if((((current >> 2) & one) ^ ((k >> 2) & one)) == 0) {
                temp = pPrism->cell[i][1][k];
                pPrism->cell[i][1][k] = pPrism->cell[i][2][k];
                pPrism->cell[i][2][k] = temp;
            }
            if((((current >> 3) & one) ^ (i & one)) == 0) {
                temp = pPrism->cell[i][0][k];
                pPrism->cell[i][0][k] = pPrism->cell[i][3][k];
                pPrism->cell[i][3][k] = temp;
            }
        }
    }
    
    return 0;
}

/*Performs a swap on the p prism based upon the s prism*/
int i_swap(s_prism *sPrism, p_prism *pPrism){
    char one = 0x01;
    scell current;
    pcell temp;
    int j;
    int k;
    for(j = 0; j < 4; j++){
        for(k = 0; k < 8; k++){
            current = sPrism->cell[3][j][k];
            if(((current & one) ^ (k & one)) == 0){
                temp = pPrism->cell[0][j][k];
                pPrism->cell[0][j][k] = pPrism->cell[1][j][k];
                pPrism->cell[1][j][k] = temp;
            }
            if((((current >> 1) & one) ^ ((k >> 1) & one)) == 0){
                temp = pPrism->cell[2][j][k];
                pPrism->cell[2][j][k] = pPrism->cell[3][j][k];
                pPrism->cell[3][j][k] = temp;
            }
            if((((current >> 2) & one) ^ ((k >> 2) & one)) == 0){
                temp = pPrism->cell[1][j][k];
                pPrism->cell[1][j][k] = pPrism->cell[2][j][k];
                pPrism->cell[2][j][k] = temp;
            }
            if((((current >> 3) & one) ^ (j & one)) == 0){
                temp = pPrism->cell[0][j][k];
                pPrism->cell[0][j][k] = pPrism->cell[3][j][k];
                pPrism->cell[3][j][k] = temp;
            }
        }
    }
    return 0;
}
/*Performs a Non-Linear System Transform on the s prism based on the p and h prisms*/
void NLST(s_prism *sPrism, p_prism *pPrism, h_prism *hPrism){
  int i;
  int j;
  int k;
  char ph;
  scell stemp;
  pcell ptemp;
  scell sp;
  scell spp;
  scell h;
  scell pl;
  s_prism newsPrism;
  for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++){
          for(k = 0; k < 8; k++){
          		stemp = sPrism->cell[i][j][k];
				ptemp = pPrism->cell[i][j][k];
				assert(stemp < 17);
				/* 15 = 00001111 */
				sp = (stemp) & 15; /* get the first 4 bits of stemp */
				pl = (ptemp) & 15; /* get the first 4 bits of ptemp */
				ph = (((stemp & ((char) 16)) >> 1) | ((ptemp >> 4) & ((char) 7)));
				assert(ph < 16); /* concatonate together the fifth bit of stemp with the first 3 bits of ptemp */
				h = (hPrism->cell[i][j][k]);
				assert(h < 16);
				spp = (sPrism->cell[((int) ptemp/32)%4][((int) ptemp/8)%4][ptemp%8]) & 15;
				newsPrism.cell[i][j][k] =
				xInverseTable[(int)(sp ^ pl)]^xInverseTable[(int)(spp ^ ph)]^h;
				assert(newsPrism.cell[i][j][k] < 16);
          }
      }
  }
  memcpy(sPrism, &newsPrism, sizeof(scell) * 128);
  return;
}

/* rotates elements counter-clockwise within the p prism */
void rubic_rot(p_prism *prism){
    int i, j, k;
    p_prism temp;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                if(k % 4 == 0){
                    temp.cell[i][j][k] = prism->cell[i][j][k];
                }
                else if(k % 4 == 1){
                    temp.cell[i][j][k] = prism->cell[3 - j][i][k];
                }
                else if(k % 4 == 2){
                    temp.cell[i][j][k] = prism->cell[j][i][k];
                }
                else if(k % 4 == 3){
                    temp.cell[i][j][k] = prism->cell[j][3 - i][k];
                }
            }
        }
    }
    memcpy(prism, &temp, sizeof(pcell) * 128);
    return;
}

/* The function which effectively performs the hash */
int compress(s_prism *sPrism, p_prism *pPrism, h_prism *hPrism){
    int i, j, k;
    
    affine_transform(sPrism);
    
    k1_swap(sPrism, pPrism);
    k2_swap(sPrism, pPrism);
    
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            s_set_ij_column(sPrism, i, j, eight_point_DFT(s_get_ij_column(sPrism, i, j)));
        }
    }
    
    j_swap(sPrism, pPrism);
    
    for (i = 0; i < 4; i++){
        for (k = 0; k < 8; k++){
            s_set_ik_row(sPrism, i, k, four_point_DFT(s_get_ik_row(sPrism, i, k)));
        }

    }
    
    i_swap(sPrism, pPrism);
    
    for (j = 0; j < 4; j++){
        for (k = 0; k < 8; k++){
            s_set_jk_row(sPrism, j, k, four_point_DFT(s_get_jk_row(sPrism, j, k)));
        }
    }
    
    NLST(sPrism, pPrism, hPrism);
    memcpy(hPrism, sPrism, sizeof(scell) * 128); /* copies the s prism over the h prism */
    rubic_rot(pPrism);
    
    return 0;
}

void mark_bits(p_prism *pPrism, s_prism *markingPrism, int digestSize){
    
    scell curSg[4];
    int i, j, k;
    int lesserBits;
    if (digestSize == 512){
        memcpy(&curSg, &sg512, sizeof(curSg));
    }
    else if (digestSize == 480){
        memcpy(&curSg, &sg480, sizeof(curSg));
    }
    else if (digestSize == 448){
        memcpy(&curSg, &sg448, sizeof(curSg));
    }
    else if (digestSize == 416){
        memcpy(&curSg, &sg416, sizeof(curSg));
    } 
    else if (digestSize == 384) {
        memcpy(&curSg, &sg384, sizeof(curSg));
    }
    else if (digestSize == 352) {
        memcpy(&curSg, &sg352, sizeof(curSg));
    }
    else if (digestSize == 320) {
        memcpy(&curSg, &sg320, sizeof(curSg));
    }
    else if (digestSize == 288) {
        memcpy(&curSg, &sg288, sizeof(curSg));
    } 
    else if (digestSize == 256) {
        memcpy(&curSg, &sg256, sizeof(curSg));
    } 
    else if (digestSize == 224) {
        memcpy(&curSg, &sg224, sizeof(curSg));
    } 
    else if (digestSize == 192) {
        memcpy(&curSg, &sg256, sizeof(curSg));
    }
    else if (digestSize == 160) {
        memcpy(&curSg, &sg256, sizeof(curSg));
    }
    else if (digestSize == 128) {
        memcpy(&curSg, &sg128, sizeof(curSg));
    } 
    else {
        fprintf(stderr, "incompatible digest size\n");
        return;
    }
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                lesserBits = get_word_sl(pPrism->cell[i][j][k]);
                markingPrism->cell[i][j][k] = curSg[lesserBits];
            }
        }
    }
    return;
}

digest* extract_bits(h_prism *hPrism, s_prism *markingPrism, digest *hashDigest){
    int i, j, k, l;
    int bitNum = 0;
    int byteNum = 0;
        for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 8; k++){
                for (l = 0; l < 4; l++) {
                    if ((0x01 << l) & markingPrism->cell[i][j][k]){
                    	hashDigest->byte[byteNum] = hashDigest->byte[byteNum] | (((hPrism->cell[i][j][k] >> l) & 0x01) << (7 - bitNum));
                    	bitNum += 1;
                    	if (bitNum >= 8) {
                    		byteNum += 1;
                    		bitNum = 0;
                    	}
                       	if (byteNum > 64) {
                    		fprintf(stderr, "Error: writing too much to digest\n");
                    		return hashDigest;
                    	}                    	
                    }
                }
            }
        }
    }
    return hashDigest;
}

void make_digest(digest *hashDigest, p_prism *pPrism, h_prism *hPrism){
    s_prism markingPrism;
    mark_bits(pPrism, &markingPrism, hashDigest->length);
    extract_bits(hPrism, &markingPrism, hashDigest);
}

HashReturn Init(hashState *state, int hbitlen) {
    state->hashbitlen = hbitlen;
    init_p_prism(&state->pPrism); /* sets the p prism values from 0 and 127 */
    init_h_prism(&state->hPrism); /* initializes all of the h prism to zero */
    state->mesagelen = 0;
    state->remainderbitlen = 0;
    state->started = 0;
    if((hbitlen == 512) || (hbitlen == 480) || (hbitlen == 448) || (hbitlen == 416) || (hbitlen == 384) || (hbitlen == 352) || (hbitlen == 320) || (hbitlen == 288) || (hbitlen == 256) || (hbitlen == 224) || (hbitlen == 192) || (hbitlen == 160) || (hbitlen == 128)){
        return SUCCESS;
    }
    else{
        return BAD_HASHBITLEN;
    }
}

HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen){
    int numchunks = (databitlen + state->remainderbitlen) / 512;
    chunk currentbitsequence;
    int i;
    int n = 0;
    int currentbyte = 0; /* keeps track of the current byte withing bitsequence */
    if(state->remainderbitlen != 0){ /* handles any bits in state's remainder */
        for(n; n < (state->remainderbitlen / 4); n++){
            currentbitsequence.word[n] = get_first_word(state->remainder[currentbyte]);
            n++;
            currentbitsequence.word[n] = get_last_word(state->remainder[currentbyte]);
            currentbyte++;
        }
        currentbyte = 0;
     }
    for(i = 0; i < numchunks; i++){
        for(n; n < 128; n++){ /* fill a chunk */
            currentbitsequence.word[n] = get_first_word(data[currentbyte]);
            n++;
            currentbitsequence.word[n] = get_last_word(data[currentbyte]);
            currentbyte++;
        }
        n = 0;
        state->mesagelen += 512;
        /* prepair s prism, and compress */
        fill_s_prism(&state->sPrism, &currentbitsequence);
        if (state->started == 0){
        	state->started = 1;
        	initial_swap_control(&state->sPrism, &state->pPrism);
        }
        compress(&state->sPrism, &state->pPrism, &state->hPrism);
    }
    /* sets up the state for use with update again or final */
    state->remainderbitlen = (databitlen + state->remainderbitlen) % 512;
    memcpy(&state->remainder, &data[currentbyte], (state->remainderbitlen / 8) + (((state->remainderbitlen % 8) != 0) ? 1 : 0) );
    return SUCCESS;
}

HashReturn Final(hashState *state, BitSequence *hashval){
   int i;
   int currentbyte = 0;
   __uint8_t leftoverbits = (state->remainderbitlen % 8);
   __uint8_t mask;
   chunk currentbitsequence;
   digest finaldigest;
   memset(&finaldigest, 0, sizeof(digest));
   state->mesagelen += state->remainderbitlen;
   if(state->remainderbitlen != 0){ /* if there are remaining bits to be processed... */
       for(i = 0; i < (state->remainderbitlen / 8) * 2; i++){
           currentbitsequence.word[i] = get_first_word(state->remainder[currentbyte]);
           i++;
           currentbitsequence.word[i] = get_last_word(state->remainder[currentbyte]);
           currentbyte++;
       }
       if(leftoverbits != 0){ /* if the remaining bits isn't a whole byte... */
           mask = (255 >> leftoverbits) ^ 255; /* makes sure to only take the amount of remainin bits and 0 the rest */
           state->remainder[currentbyte] = state->remainder[currentbyte] & mask;
           state->remainder[currentbyte] = state->remainder[currentbyte] | (128 >> leftoverbits); /* make the bit following the last actual bit a 1 */
           currentbitsequence.word[i] = get_first_word(state->remainder[currentbyte]);
           i++;
           currentbitsequence.word[i] = get_last_word(state->remainder[currentbyte]);
       }
       else{
           currentbitsequence.word[i] = 0x08;
           i++;
       }
       if(i < 112){
           for(i; i < 112; i++){
               currentbitsequence.word[i] = 0;
           }
           add_length(&currentbitsequence, state->mesagelen);
       }
       else{
           /* if i >= 112, pad with 0 until the end of the chunk */
           for(i; i < 128; i++){
               currentbitsequence.word[i] = 0;
           }
           fill_s_prism(&state->sPrism, &currentbitsequence);
           initial_swap_control(&state->sPrism, &state->pPrism);
           compress(&state->sPrism, &state->pPrism, &state->hPrism);
           for(i = 0; i < 112; i++){
               currentbitsequence.word[i] = 0;
           }
           add_length(&currentbitsequence, state->mesagelen);
       }
       fill_s_prism(&state->sPrism, &currentbitsequence);
       initial_swap_control(&state->sPrism, &state->pPrism);
       compress(&state->sPrism, &state->pPrism, &state->hPrism);
   }
   
   else{
        currentbitsequence.word[0] = 0x08;
        for(i = 1; i < 112; i++){
            currentbitsequence.word[i] = 0;
        }
        add_length(&currentbitsequence, state->mesagelen);
        fill_s_prism(&state->sPrism, &currentbitsequence);
        initial_swap_control(&state->sPrism, &state->pPrism);
        compress(&state->sPrism, &state->pPrism, &state->hPrism);
   }
   finaldigest.length = state->hashbitlen;
   make_digest(&finaldigest, &state->pPrism, &state->hPrism); /* compute the hash */
   memcpy(hashval, &finaldigest.byte[0], (state->hashbitlen / 8));
   return SUCCESS;
}

HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval){
	hashState state;
	if(Init(&state, hashbitlen) == BAD_HASHBITLEN){/* initializes state and checks to make sure hashbitlen is valid */
		return BAD_HASHBITLEN;
	}
	Update(&state, data, databitlen); /* handles the bulk of the compresses */
	Final(&state, hashval); /* handles leftover bits and calculates hash value */
	return SUCCESS;
}
