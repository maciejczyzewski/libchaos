#include "blake.h"

void blake512_compress( state512 *S, const uint8_t *block )
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


void blake512_init( state512 *S )
{
  S->h[0] = 0x6a09e667f3bcc908ULL;
  S->h[1] = 0xbb67ae8584caa73bULL;
  S->h[2] = 0x3c6ef372fe94f82bULL;
  S->h[3] = 0xa54ff53a5f1d36f1ULL;
  S->h[4] = 0x510e527fade682d1ULL;
  S->h[5] = 0x9b05688c2b3e6c1fULL;
  S->h[6] = 0x1f83d9abfb41bd6bULL;
  S->h[7] = 0x5be0cd19137e2179ULL;
  S->t[0] = S->t[1] = S->buflen = S->nullt = 0;
  S->s[0] = S->s[1] = S->s[2] = S->s[3] = 0;
}


void blake512_update( state512 *S, const uint8_t *in, uint64_t inlen )
{
  int left = S->buflen;
  int fill = 128 - left;

  /* data left and data received fill a block  */
  if( left && ( inlen >= fill ) )
  {
    memcpy( ( void * ) ( S->buf + left ), ( void * ) in, fill );
    S->t[0] += 1024;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake512_compress( S, S->buf );
    in += fill;
    inlen  -= fill;
    left = 0;
  }

  /* compress blocks of data received */
  while( inlen >= 128 )
  {
    S->t[0] += 1024;

    if ( S->t[0] == 0 ) S->t[1]++;

    blake512_compress( S, in );
    in += 128;
    inlen -= 128;
  }

  /* store any data left */
  if( inlen > 0 )
  {
    memcpy( ( void * ) ( S->buf + left ),   \
            ( void * ) in, ( size_t ) inlen );
    S->buflen = left + ( int )inlen;
  }
  else S->buflen = 0;
}


void blake512_final( state512 *S, uint8_t *out )
{
  uint8_t msglen[16], zo = 0x01, oo = 0x81;
  uint64_t lo = S->t[0] + ( S->buflen << 3 ), hi = S->t[1];

  /* support for hashing more than 2^32 bits */
  if ( lo < ( S->buflen << 3 ) ) hi++;

  U64TO8_BIG(  msglen + 0, hi );
  U64TO8_BIG(  msglen + 8, lo );

  if ( S->buflen == 111 )   /* one padding byte */
  {
    S->t[0] -= 8;
    blake512_update( S, &oo, 1 );
  }
  else
  {
    if ( S->buflen < 111 )  /* enough space to fill the block */
    {
      if ( !S->buflen ) S->nullt = 1;

      S->t[0] -= 888 - ( S->buflen << 3 );
      blake512_update( S, padding, 111 - S->buflen );
    }
    else   /* need 2 compressions */
    {
      S->t[0] -= 1024 - ( S->buflen << 3 );
      blake512_update( S, padding, 128 - S->buflen );
      S->t[0] -= 888;
      blake512_update( S, padding + 1, 111 );
      S->nullt = 1;
    }

    blake512_update( S, &zo, 1 );
    S->t[0] -= 8;
  }

  S->t[0] -= 128;
  blake512_update( S, msglen, 16 );
  U64TO8_BIG( out + 0, S->h[0] );
  U64TO8_BIG( out + 8, S->h[1] );
  U64TO8_BIG( out + 16, S->h[2] );
  U64TO8_BIG( out + 24, S->h[3] );
  U64TO8_BIG( out + 32, S->h[4] );
  U64TO8_BIG( out + 40, S->h[5] );
  U64TO8_BIG( out + 48, S->h[6] );
  U64TO8_BIG( out + 56, S->h[7] );
}


void blake512_hash( uint8_t *out, const uint8_t *in, uint64_t inlen )
{
  state512 S;
  blake512_init( &S );
  blake512_update( &S, in, inlen );
  blake512_final( &S, out );
}


void blake512_test()
{
  int i, v;
  uint8_t in[144], out[64];
  uint8_t test1[] =
  {
    0x97, 0x96, 0x15, 0x87, 0xf6, 0xd9, 0x70, 0xfa, 0xba, 0x6d, 0x24, 0x78, 0x04, 0x5d, 0xe6, 0xd1,
    0xfa, 0xbd, 0x09, 0xb6, 0x1a, 0xe5, 0x09, 0x32, 0x05, 0x4d, 0x52, 0xbc, 0x29, 0xd3, 0x1b, 0xe4,
    0xff, 0x91, 0x02, 0xb9, 0xf6, 0x9e, 0x2b, 0xbd, 0xb8, 0x3b, 0xe1, 0x3d, 0x4b, 0x9c, 0x06, 0x09,
    0x1e, 0x5f, 0xa0, 0xb4, 0x8b, 0xd0, 0x81, 0xb6, 0x34, 0x05, 0x8b, 0xe0, 0xec, 0x49, 0xbe, 0xb3
  };
  uint8_t test2[] =
  {
    0x31, 0x37, 0x17, 0xd6, 0x08, 0xe9, 0xcf, 0x75, 0x8d, 0xcb, 0x1e, 0xb0, 0xf0, 0xc3, 0xcf, 0x9f,
    0xC1, 0x50, 0xb2, 0xd5, 0x00, 0xfb, 0x33, 0xf5, 0x1c, 0x52, 0xaf, 0xc9, 0x9d, 0x35, 0x8a, 0x2f,
    0x13, 0x74, 0xb8, 0xa3, 0x8b, 0xba, 0x79, 0x74, 0xe7, 0xf6, 0xef, 0x79, 0xca, 0xb1, 0x6f, 0x22,
    0xCE, 0x1e, 0x64, 0x9d, 0x6e, 0x01, 0xad, 0x95, 0x89, 0xc2, 0x13, 0x04, 0x5d, 0x54, 0x5d, 0xde
  };
  memset( in, 0, 144 );
  blake512_hash( out, in, 1 );
  v = 0;

  for( i = 0; i < 64; ++i )
  {
    if ( out[i] != test1[i] ) v = 1;
  }

  if ( v ) printf( "test 1 error\n" );

  blake512_hash( out, in, 144 );
  v = 0;

  for( i = 0; i < 64; ++i )
  {
    if ( out[i] != test2[i] ) v = 1;
  }

  if ( v ) printf( "test 2 error\n" );
}

int main( int argc, char **argv )
{
#define BLOCK512 64
  FILE *fp;
  int i, j, bytesread;
  uint8_t in[BLOCK512], out[64];
  state512 S;
  blake512_test();

  for( i = 1; i < argc; ++i )
  {
    fp = fopen( *( argv + i ), "r" );

    if ( fp == NULL )
    {
      printf( "Error: unable to open %s\n", *( argv + i ) );
      return 1;
    }

    blake512_init( &S );

    while( 1 )
    {
      bytesread = fread( in, 1, BLOCK512, fp );

      if ( bytesread )
        blake512_update( &S, in, bytesread );
      else
        break;
    }

    blake512_final( &S, out );

    for( j = 0; j < 64; ++j )
      printf( "%02x", out[j] );

    printf( " %s\n", *( argv + i ) );
    fclose( fp );
  }

  return 0;
}
