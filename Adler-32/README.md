# Adler-32

Compared to a cyclic redundancy check of the same length, it trades reliability for speed. Adler-32 is more reliable than Fletcher-16, and slightly less reliable than Fletcher-32.

## Informations

* __First published__ - 1995
* __Digest sizes__ - 32 bits
* __Structure__ - sum

## Authors

Mark Adler

## Cryptanalysis

Jonathan Stone discovered in 2001 that Adler-32 has a weakness for very short messages. He wrote "Briefly, the problem is that, for very short packets, Adler-32 is guaranteed to give poor coverage of the available bits. Don't take my word for it, ask Mark Adler.  :-)"

## Implementations

- adler16.c

## Materials

- [ZLIB Compressed Data Format Specification version 3.3](http://tools.ietf.org/html/rfc1950)

## Pseudocode

```
const int MOD_ADLER = 65521;

uint32_t adler32(unsigned char *data, size_t len) /* where data is the location of the data in physical memory and
                                                      len is the length of the data in bytes */
{
    uint32_t a = 1, b = 0;
    size_t index;

    /* Process each byte of the data in order */
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}
```