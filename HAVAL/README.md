# HAVAL

HAVAL is a cryptographic hash function. Unlike MD5, but like most modern cryptographic hash functions, HAVAL can produce hashes of different lengths. HAVAL can produce hashes in lengths of 128 bits, 160 bits, 192 bits, 224 bits, and 256 bits. HAVAL also allows users to specify the number of rounds (3, 4, or 5) to be used to generate the hash.

## Informations

* __First published__ - 1992
* __Digest sizes__ - 128 to 256 bits
* __Structure__ - hash
* __Rounds__ - 4

## Authors

Yuliang Zheng, Josef Pieprzyk, and Jennifer Seberry

## Cryptanalysis

Research has uncovered weaknesses which make further use of HAVAL (at least the variant with 128 bits and 3 passes with 2^6 operations) questionable. On 17 August 2004, collisions for HAVAL (128 bits, 3 passes) were announced by Xiaoyun Wang, Dengguo Feng, Xuejia Lai, and Hongbo Yu.

## Implementations

- [HAVAL source](http://labs.calyptix.com/haval-1.1.tar.gz)

## Materials

- [HAVAL — A One-Way Hashing Algorithm with Variable Length of Output](http://labs.calyptix.com/files/haval-paper.pdf)
- [Collisions for Hash Functions MD4, MD5, HAVAL-128 and RIPEMD](http://eprint.iacr.org/2004/199.pdf)
- [HAVAL website](http://labs.calyptix.com/haval.php)
