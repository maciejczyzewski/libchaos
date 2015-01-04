# MD5

The MD4 Message-Digest Algorithm is a cryptographic hash function. The digest length is 128 bits. The algorithm has influenced later designs, such as the MD5, SHA-1 and RIPEMD algorithms.

## Informations

* __First published__ - October 1990
* __Series__ - MD2, MD4, MD5, MD6
* __Digest sizes__ - 128 bits
* __Structure__ - Merkle–Damgård construction
* __Rounds__ - 3

## Authors

Ronald Rivest

## Cryptanalysis

The security of MD4 has been severely compromised. The first full collision attack against MD4 was published in 1995 and several newer attacks have been published since then.

As of 2007, an attack can generate collisions in less than 2 hash operations. A theoretical preimage attack also exists.

## Implementations

- md4.c

## Materials

- [The MD4 Message-Digest Algorithm](http://tools.ietf.org/html/rfc1320)
- [Fast MD4 Collision Generator](http://www.bishopfox.com/resources/tools/other-free-tools/md4md5-collision-code/)

## Pseudocode

```
/* Process each 16-word block. */
For i = 0 to N/16-1 do

  /* Copy block i into X. */
  For j = 0 to 15 do
    Set X[j] to M[i*16+j].
  end /* of loop on j */

  /* Save A as AA, B as BB, C as CC, and D as DD. */
  AA = A
  BB = B
  CC = C
  DD = D

  /* Round 1. */
  /* Let [abcd k s] denote the operation
       a = (a + F(b,c,d) + X[k]) <<< s. */
  /* Do the following 16 operations. */
  [ABCD  0  3]  [DABC  1  7]  [CDAB  2 11]  [BCDA  3 19]
  [ABCD  4  3]  [DABC  5  7]  [CDAB  6 11]  [BCDA  7 19]
  [ABCD  8  3]  [DABC  9  7]  [CDAB 10 11]  [BCDA 11 19]
  [ABCD 12  3]  [DABC 13  7]  [CDAB 14 11]  [BCDA 15 19]

  /* Round 2. */
  /* Let [abcd k s] denote the operation
       a = (a + G(b,c,d) + X[k] + 5A827999) <<< s. */

  /* Do the following 16 operations. */
  [ABCD  0  3]  [DABC  4  5]  [CDAB  8  9]  [BCDA 12 13]
  [ABCD  1  3]  [DABC  5  5]  [CDAB  9  9]  [BCDA 13 13]
  [ABCD  2  3]  [DABC  6  5]  [CDAB 10  9]  [BCDA 14 13]
  [ABCD  3  3]  [DABC  7  5]  [CDAB 11  9]  [BCDA 15 13]

  /* Round 3. */
  /* Let [abcd k s] denote the operation
       a = (a + H(b,c,d) + X[k] + 6ED9EBA1) <<< s. */
  /* Do the following 16 operations. */
  [ABCD  0  3]  [DABC  8  9]  [CDAB  4 11]  [BCDA 12 15]
  [ABCD  2  3]  [DABC 10  9]  [CDAB  6 11]  [BCDA 14 15]
  [ABCD  1  3]  [DABC  9  9]  [CDAB  5 11]  [BCDA 13 15]
  [ABCD  3  3]  [DABC 11  9]  [CDAB  7 11]  [BCDA 15 15]

  /* Then perform the following additions. (That is, increment each
     of the four registers by the value it had before this block
     was started.) */
  A = A + AA
  B = B + BB
  C = C + CC
  D = D + DD

end /* of loop on i */
```