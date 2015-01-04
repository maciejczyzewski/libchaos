# CRC

The SYSV checksum algorithm is commonly used, legacy checksum algorithms. It has been implemented in UNIX System V and is also available through the GNU sum command line utility.

## Informations

* __Digest sizes__ - 16 bits
* __Structure__ - CRC

## Cryptanalysis

- Re-arranging the same bytes in another order does not change the checksum.
- Increasing one byte and decreasing another byte by the same amount does not change the checksum.
- Adding or removing zero bytes does not change the checksum.

## Pseudocode

The main part of this algorithm is simply adding up all bytes in a 32-bit sum.
