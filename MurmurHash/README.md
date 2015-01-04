# MurmurHash

MurmurHash is a non-cryptographic hash function suitable for general hash-based lookup.

## Informations

* __First published__ - 2008
* __Digest sizes__ - 32, 64, or 128 bits
* __Structure__ - product/rotation

## Authors

Austin Appleby

## Implementations

- murmurhash3_32.c

## Materials

- [MurmurHash first announcement](http://tanjent.livejournal.com/756623.html)
- [MurmurHash1](https://code.google.com/p/smhasher/wiki/MurmurHash1)
- [MurmurHash2](https://code.google.com/p/smhasher/wiki/MurmurHash2)
- [MurmurHash3](https://code.google.com/p/smhasher/wiki/MurmurHash3)

## Pseudocode

```
Murmur3_32(key, len, seed)
    // Note: In this version, all integer arithmetic is performed with unsigned 32 bit integers.
    //       In the case of overflow, the result is constrained by the application of modulo 2^{32} arithmetic.

    c1 \gets 0xcc9e2d51
    c2 \gets 0x1b873593
    r1 \gets 15
    r2 \gets 13
    m \gets 5
    n \gets 0xe6546b64

    hash \gets seed

    for each fourByteChunk of key
        k \gets fourByteChunk

        k \gets k * c1
        k \gets (k << r1) OR (k >> (32-r1))
        k \gets k * c2

        hash \gets hash XOR k
        hash \gets (hash << r2) OR (hash >> (32-r2))
        hash \gets hash * m + n

    with any remainingBytesInKey
        remainingBytes \gets SwapEndianOrderOf(remainingBytesInKey)
        // Note: Endian swapping is only necessary on big-endian machines.
        //       The purpose is to place the meaningful digits towards the low end of the value,
        //       so that these digits have the greatest potential to affect the low range digits
        //       in the subsequent multiplication.  Consider that locating the meaningful digits
        //       in the high range would produce a greater effect upon the high digits of the
        //       multiplication, and notably, that such high digits are likely to be discarded
        //       by the modulo arithmetic under overflow.  We don't want that.

        remainingBytes \gets remainingBytes * c1
        remainingBytes \gets (remainingBytes << r1) OR (remainingBytes >> (32 - r1))
        remainingBytes \gets remainingBytes * c2

        hash \gets hash XOR remainingBytes

    hash \gets hash XOR len

    hash \gets hash XOR (hash >> 16)
    hash \gets hash * 0x85ebca6b
    hash \gets hash XOR (hash >> 13)
    hash \gets hash * 0xc2b2ae35
    hash \gets hash XOR (hash >> 16)
```