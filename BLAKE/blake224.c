#include "blake.h"


void blake224_compress( state224 *S, const uint8_t *block )
{
  uint32_t v[16], m[16], i;
#define ROT(x,n) (((x)<<(32-n))|( (x)>>(n)))
#define G(a,b,c,d,e)          \
  v[a] += (m[sigma[i][e]] ^ u256[sigma[i][e+1]]) + v[b]; \
  v[d] = ROT( v[d] ^ v[a],16);        \
  v[c] += v[d];           \
  v[b] = ROT( v[b] ^ v[c],12);        \
  v[a] += (m[sigma[i][e+1]] ^ u256[sigma[i][e]])+v[b]; \
  v[d] = ROT( v[d] ^ v[a], 8);        \
  v[c] += v[d];           \
  v[b] = ROT( v[b] ^ v[c], 7);

  for( i = 0; i < 16; ++i )  m[i] = U8TO32_BIG( block + i * 4 );

  for( i = 0; i < 8; ++i )  v[i] = S->h[i];

  v[ 8] = S->s[0] ^ u256[0];
  v[ 9] = S->s[1] ^ u256[1];
  v[10] = S->s[2] ^ u256[2];
  v[11] = S->s[3] ^ u256[3];
  v[12] = u256[4];
  v[13] = u256[5];
  v[14] = u256[6];
  v[15] = u256[7];

  /* don't xor t when the block is only padding */
  if ( !S->nullt )
  {
    v[12] ^= S->t[0];
    v[13] ^= S->t[0];
    v[14] ^= S->t[1];
    v[15] ^= S->t[1];
  }

  for( i = 0; i < 14; ++i )
  {
    /* column step */
    G( 0,  4,  8, 12,  0 );
    G( 1,  5,  9, 13,  2 );
    G( 2,  6, 10, 14,  4 );
    G( 3,  7, 11, 15,  6 );
    /* diagonal step */
    G( 0,  5, 10, 15,  8 );
    G( 1,  6, 11, 12, 10 );
    G( 2,  7,  8, 13, 12 );
    G( 3,  4,  9, 14, 14 );
  }

  for( i = 0; i < 16; ++i )  S->h[i % 8] ^= v[i];

  for( i = 0; i < 8 ; ++i )  S->h[i] ^= S->s[i % 4];
}


void blake224_init( state224 *S )
{
  S->h[0] = 0xc1059ed8;
  S->h[1] = 0x367cd507;
  S->h[2] = 0x3070dd17;
  S->h[3] = 0xf70e5939;
  S->h[4] = 0xffc00b31;
  S->h[5] = 0x68581511;
  S->h[6] = 0x64f98fa7;
  S->h[7] = 0xbefa4fa4;
  S->t[0] = S->t[1] = S->buflen = S->nullt = 0;
  S->s[0] = S->s[1] = S->s[2] = S->s[3] = 0;
}


void blake224_update( state224 *S, const uint8_t *in, uint64_t inlen )
{
  int left = S->buflen;
  int fill = 64 - left;

  /* data left and data received fill a block  */
  if( left && ( inlen >= fill ) )
  {
    memcpy( ( void * ) ( S->buf + left ), ( void * ) in, fill );
    S->t[0] += 512;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake224_compress( S, S->buf );
    in += fill;
    inlen  -= fill;
    left = 0;
  }

  /* compress blocks of data received */
  while( inlen >= 64 )
  {
    S->t[0] += 512;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake224_compress( S, in );
    in += 64;
    inlen -= 64;
  }

  /* store any data left */
  if( inlen > 0 )
  {
    memcpy( ( void * ) ( S->buf + left ), \
            ( void * ) in, ( size_t ) inlen );
    S->buflen = left + ( int )inlen;
  }
  else S->buflen = 0;
}


void blake224_final( state224 *S, uint8_t *out )
{
  uint8_t msglen[8], zz = 0x00, oz = 0x80;
  uint32_t lo = S->t[0] + ( S->buflen << 3 ), hi = S->t[1];

  /* support for hashing more than 2^32 bits */
  if ( lo < ( S->buflen << 3 ) ) hi++;

  U32TO8_BIG(  msglen + 0, hi );
  U32TO8_BIG(  msglen + 4, lo );

  if ( S->buflen == 55 )   /* one padding byte */
  {
    S->t[0] -= 8;
    blake224_update( S, &oz, 1 );
  }
  else
  {
    if ( S->buflen < 55 )   /* enough space to fill the block  */
    {
      if ( !S->buflen ) S->nullt = 1;

      S->t[0] -= 440 - ( S->buflen << 3 );
      blake224_update( S, padding, 55 - S->buflen );
    }
    else   /* need 2 compressions */
    {
      S->t[0] -= 512 - ( S->buflen << 3 );
      blake224_update( S, padding, 64 - S->buflen );
      S->t[0] -= 440;
      blake224_update( S, padding + 1, 55 );
      S->nullt = 1;
    }

    blake224_update( S, &zz, 1 );
    S->t[0] -= 8;
  }

  S->t[0] -= 64;
  blake224_update( S, msglen, 8 );
  U32TO8_BIG( out + 0, S->h[0] );
  U32TO8_BIG( out + 4, S->h[1] );
  U32TO8_BIG( out + 8, S->h[2] );
  U32TO8_BIG( out + 12, S->h[3] );
  U32TO8_BIG( out + 16, S->h[4] );
  U32TO8_BIG( out + 20, S->h[5] );
  U32TO8_BIG( out + 24, S->h[6] );
}


void blake224_hash( uint8_t *out, const uint8_t *in, uint64_t inlen )
{
  state224 S;
  blake224_init( &S );
  blake224_update( &S, in, inlen );
  blake224_final( &S, out );
}


void blake224_test()
{
  int i, v;
  uint8_t in[72], out[28];
  uint8_t test1[] =
  {
    0x45, 0x04, 0xcb, 0x03, 0x14, 0xfb, 0x2a, 0x4f,
    0x7a, 0x69, 0x2e, 0x69, 0x6e, 0x48, 0x79, 0x12,
    0xfe, 0x3f, 0x24, 0x68, 0xfe, 0x31, 0x2c, 0x73,
    0xa5, 0x27, 0x8e, 0xc5
  };
  uint8_t test2[] =
  {
    0xf5, 0xaa, 0x00, 0xdd, 0x1c, 0xb8, 0x47, 0xe3,
    0x14, 0x03, 0x72, 0xaf, 0x7b, 0x5c, 0x46, 0xb4,
    0x88, 0x8d, 0x82, 0xc8, 0xc0, 0xa9, 0x17, 0x91,
    0x3c, 0xfb, 0x5d, 0x04
  };
  memset( in, 0, 72 );
  blake224_hash( out, in, 1 );
  v = 0;

  for( i = 0; i < 28; ++i )
  {
    if ( out[i] != test1[i] ) v = 1;
  }

  if ( v ) printf( "test 1 error\n" );

  blake224_hash( out, in, 72 );
  v = 0;

  for( i = 0; i < 28; ++i )
  {
    if ( out[i] != test2[i] ) v = 1;
  }

  if ( v ) printf( "test 2 error\n" );
}


int main( int argc, char **argv )
{
#define BLOCK224 64
  FILE *fp;
  int i, j, bytesread;
  uint8_t in[BLOCK224], out[28];
  state224 S;
  blake224_test();

  for( i = 1; i < argc; ++i )
  {
    fp = fopen( *( argv + i ), "r" );

    if ( fp == NULL )
    {
      printf( "Error: unable to open %s\n", *( argv + i ) );
      return 1;
    }

    blake224_init( &S );

    while( 1 )
    {
      bytesread = fread( in, 1, BLOCK224, fp );

      if ( bytesread )
        blake224_update( &S, in, bytesread );
      else
        break;
    }

    blake224_final( &S, out );

    for( j = 0; j < 28; ++j )
      printf( "%02x", out[j] );

    printf( " %s\n", *( argv + i ) );
    fclose( fp );
  }

  return 0;
}
