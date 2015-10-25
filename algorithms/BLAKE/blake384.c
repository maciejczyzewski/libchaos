#include "blake.h"

void blake384_compress( state384 *S, const uint8_t *block )
{
  uint64_t v[16], m[16], i;
#define ROT(x,n) (((x)<<(64-n))|( (x)>>(n)))
#define G(a,b,c,d,e)          \
  v[a] += (m[sigma[i][e]] ^ u512[sigma[i][e+1]]) + v[b];\
  v[d] = ROT( v[d] ^ v[a],32);        \
  v[c] += v[d];           \
  v[b] = ROT( v[b] ^ v[c],25);        \
  v[a] += (m[sigma[i][e+1]] ^ u512[sigma[i][e]])+v[b];  \
  v[d] = ROT( v[d] ^ v[a],16);        \
  v[c] += v[d];           \
  v[b] = ROT( v[b] ^ v[c],11);

  for( i = 0; i < 16; ++i )  m[i] = U8TO64_BIG( block + i * 8 );

  for( i = 0; i < 8; ++i )  v[i] = S->h[i];

  v[ 8] = S->s[0] ^ u512[0];
  v[ 9] = S->s[1] ^ u512[1];
  v[10] = S->s[2] ^ u512[2];
  v[11] = S->s[3] ^ u512[3];
  v[12] =  u512[4];
  v[13] =  u512[5];
  v[14] =  u512[6];
  v[15] =  u512[7];

  /* don't xor t when the block is only padding */
  if ( !S->nullt )
  {
    v[12] ^= S->t[0];
    v[13] ^= S->t[0];
    v[14] ^= S->t[1];
    v[15] ^= S->t[1];
  }

  for( i = 0; i < 16; ++i )
  {
    /* column step */
    G( 0, 4, 8, 12, 0 );
    G( 1, 5, 9, 13, 2 );
    G( 2, 6, 10, 14, 4 );
    G( 3, 7, 11, 15, 6 );
    /* diagonal step */
    G( 0, 5, 10, 15, 8 );
    G( 1, 6, 11, 12, 10 );
    G( 2, 7, 8, 13, 12 );
    G( 3, 4, 9, 14, 14 );
  }

  for( i = 0; i < 16; ++i )  S->h[i % 8] ^= v[i];

  for( i = 0; i < 8 ; ++i )  S->h[i] ^= S->s[i % 4];
}


void blake384_init( state384 *S )
{
  S->h[0] = 0xcbbb9d5dc1059ed8ULL;
  S->h[1] = 0x629a292a367cd507ULL;
  S->h[2] = 0x9159015a3070dd17ULL;
  S->h[3] = 0x152fecd8f70e5939ULL;
  S->h[4] = 0x67332667ffc00b31ULL;
  S->h[5] = 0x8eb44a8768581511ULL;
  S->h[6] = 0xdb0c2e0d64f98fa7ULL;
  S->h[7] = 0x47b5481dbefa4fa4ULL;
  S->t[0] = S->t[1] = S->buflen = S->nullt = 0;
  S->s[0] = S->s[1] = S->s[2] = S->s[3] = 0;
}


void blake384_update( state384 *S, const uint8_t *in, uint64_t inlen )
{
  int left = S->buflen;
  int fill = 128 - left;

  /* data left and data received fill a block  */
  if( left && ( inlen >= fill ) )
  {
    memcpy( ( void * ) ( S->buf + left ), ( void * ) in, fill );
    S->t[0] += 1024;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake384_compress( S, S->buf );
    in += fill;
    inlen  -= fill;
    left = 0;
  }

  /* compress blocks of data received */
  while( inlen >= 128 )
  {
    S->t[0] += 1024;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake384_compress( S, in );
    in += 128;
    inlen -= 128;
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


void blake384_final( state384 *S, uint8_t *out )
{
  uint8_t msglen[16], zz = 0x00, oz = 0x80;
  uint64_t lo = S->t[0] + ( S->buflen << 3 ), hi = S->t[1];

  /* support for hashing more than 2^32 bits */
  if ( lo < ( S->buflen << 3 ) ) hi++;

  U64TO8_BIG(  msglen + 0, hi );
  U64TO8_BIG(  msglen + 8, lo );

  if ( S->buflen == 111 )   /* one padding byte */
  {
    S->t[0] -= 8;
    blake384_update( S, &oz, 1 );
  }
  else
  {
    if ( S->buflen < 111 )  /* enough space to fill the block */
    {
      if ( !S->buflen ) S->nullt = 1;

      S->t[0] -= 888 - ( S->buflen << 3 );
      blake384_update( S, padding, 111 - S->buflen );
    }
    else   /* need 2 compressions */
    {
      S->t[0] -= 1024 - ( S->buflen << 3 );
      blake384_update( S, padding, 128 - S->buflen );
      S->t[0] -= 888;
      blake384_update( S, padding + 1, 111 );
      S->nullt = 1;
    }

    blake384_update( S, &zz, 1 );
    S->t[0] -= 8;
  }

  S->t[0] -= 128;
  blake384_update( S, msglen, 16 );
  U64TO8_BIG( out + 0, S->h[0] );
  U64TO8_BIG( out + 8, S->h[1] );
  U64TO8_BIG( out + 16, S->h[2] );
  U64TO8_BIG( out + 24, S->h[3] );
  U64TO8_BIG( out + 32, S->h[4] );
  U64TO8_BIG( out + 40, S->h[5] );
}


void blake384_hash( uint8_t *out, const uint8_t *in, uint64_t inlen )
{
  state384 S;
  blake384_init( &S );
  blake384_update( &S, in, inlen );
  blake384_final( &S, out );
}


void blake384_test()
{
  int i, v;
  uint8_t in[144], out[48];
  uint8_t test1[] =
  {
    0x10, 0x28, 0x1f, 0x67, 0xe1, 0x35, 0xe9, 0x0a, 0xe8, 0xe8, 0x82, 0x25, 0x1a, 0x35, 0x55, 0x10,
    0xa7, 0x19, 0x36, 0x7a, 0xd7, 0x02, 0x27, 0xb1, 0x37, 0x34, 0x3e, 0x1b, 0xc1, 0x22, 0x01, 0x5c,
    0x29, 0x39, 0x1e, 0x85, 0x45, 0xb5, 0x27, 0x2d, 0x13, 0xa7, 0xc2, 0x87, 0x9d, 0xa3, 0xd8, 0x07
  };
  uint8_t test2[] =
  {
    0x0b, 0x98, 0x45, 0xdd, 0x42, 0x95, 0x66, 0xcd, 0xab, 0x77, 0x2b, 0xa1, 0x95, 0xd2, 0x71, 0xef,
    0xfe, 0x2d, 0x02, 0x11, 0xf1, 0x69, 0x91, 0xd7, 0x66, 0xba, 0x74, 0x94, 0x47, 0xc5, 0xcd, 0xe5,
    0x69, 0x78, 0x0b, 0x2d, 0xaa, 0x66, 0xc4, 0xb2, 0x24, 0xa2, 0xec, 0x2e, 0x5d, 0x09, 0x17, 0x4c
  };
  memset( in, 0, 144 );
  blake384_hash( out, in, 1 );
  v = 0;

  for( i = 0; i < 48; ++i )
  {
    if ( out[i] != test1[i] ) v = 1;
  }

  if ( v ) printf( "test 1 error\n" );

  blake384_hash( out, in, 144 );
  v = 0;

  for( i = 0; i < 48; ++i )
  {
    if ( out[i] != test2[i] ) v = 1;
  }

  if ( v ) printf( "test 2 error\n" );
}

int main( int argc, char **argv )
{
#define BLOCK384 64
  FILE *fp;
  int i, j, bytesread;
  uint8_t in[BLOCK384], out[48];
  state384 S;
  blake384_test();

  for( i = 1; i < argc; ++i )
  {
    fp = fopen( *( argv + i ), "r" );

    if ( fp == NULL )
    {
      printf( "Error: unable to open %s\n", *( argv + i ) );
      return 1;
    }

    blake384_init( &S );

    while( 1 )
    {
      bytesread = fread( in, 1, BLOCK384, fp );

      if ( bytesread )
        blake384_update( &S, in, bytesread );
      else
        break;
    }

    blake384_final( &S, out );

    for( j = 0; j < 48; ++j )
      printf( "%02x", out[j] );

    printf( " %s\n", *( argv + i ) );
    fclose( fp );
  }

  return 0;
}
