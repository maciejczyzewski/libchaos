# BSD

The BSD checksum algorithm is a commonly used, legacy checksum algorithm. It has been implemented in BSD and is also available through the GNU sum command line utility.

## Informations

* __Digest sizes__ - 16 bits
* __Structure__ - CRC

## Pseudocode

Below is the relevant part of the GNU sum source code (GPL licensed). It computes a 16-bit checksum by adding up all bytes (8-bit words) of the input data stream.

In order to avoid many of the weaknesses of simply adding the data, the checksum accumulator is circular rotated to the right by one bit at each step before the new char is added.

```c
FILE *fp;                     /* The file handle for input data */
int ch;                       /* Each character read. */
int checksum = 0;             /* The checksum mod 2^16. */

while ((ch = getc(fp)) != EOF) {
    checksum = (checksum >> 1) + ((checksum & 1) << 15);
    checksum += ch;
    checksum &= 0xffff;       /* Keep it within bounds. */
}
```