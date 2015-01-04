# Fletcher

The Fletcher checksum is an algorithm for computing a position-dependent checksum. The objective of the Fletcher checksum was to provide error-detection properties approaching those of a cyclic redundancy check but with the lower computational effort associated with summation techniques.

## Informations

* __First published__ - 1970
* __Digest sizes__ - 4, 8, 16, 32 bits
* __Structure__ - sum

## Authors

John Gould Fletcher

## Cryptanalysis

The Fletcher checksum cannot distinguish between blocks of all 0 bits and blocks of all 1 bits. For example, if a 16-bit block in the data word changes from 0x0000 to 0xFFFF, the Fletcher-32 checksum remains the same. This also means a sequence of all 00 bytes has the same checksum as a sequence (of the same size) of all FF bytes.

## Implementations

- fletcher16.c
- fletcher32.c

## Materials

- [ISO Transport Protocol Specification](http://tools.ietf.org/html/rfc905)
- [TCP Alternate Checksum Options](http://tools.ietf.org/html/rfc1146)
- [The Effectiveness of Checksums for Embedded Control Networks](http://users.ece.cmu.edu/~koopman/pubs/maxino09_checksums.pdf)

## Pseudocode

An optimized implementation in the C programming language operates as follows.

```
uint16_t fletcher16( uint8_t const *data, size_t bytes )
{
        uint16_t sum1 = 0xff, sum2 = 0xff;

        while (bytes) {
                size_t tlen = bytes > 20 ? 20 : bytes;
                bytes -= tlen;
                do {
                        sum2 += sum1 += *data++;
                } while (--tlen);
                sum1 = (sum1 & 0xff) + (sum1 >> 8);
                sum2 = (sum2 & 0xff) + (sum2 >> 8);
        }
        /* Second reduction step to reduce sums to 8 bits */
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
        return sum2 << 8 | sum1;
}

uint32_t fletcher32( uint16_t const *data, size_t words )
{
        uint32_t sum1 = 0xffff, sum2 = 0xffff;

        while (words) {
                unsigned tlen = words > 359 ? 359 : words;
                words -= tlen;
                do {
                        sum2 += sum1 += *data++;
                } while (--tlen);
                sum1 = (sum1 & 0xffff) + (sum1 >> 16);
                sum2 = (sum2 & 0xffff) + (sum2 >> 16);
        }
        /* Second reduction step to reduce sums to 16 bits */
        sum1 = (sum1 & 0xffff) + (sum1 >> 16);
        sum2 = (sum2 & 0xffff) + (sum2 >> 16);
        return sum2 << 16 | sum1;
}
```