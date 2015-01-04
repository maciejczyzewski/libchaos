# GOST

The GOST hash function, defined in the standards GOST R 34.11-94 and GOST 34.311-95, is a 256-bit cryptographic hash function. It was initially defined in the Russian national standard GOST R 34.11-94 Information Technology - Cryptographic Information Security - Hash Function. The equivalent standard used by other member-states of the CIS is GOST 34.311-95.

## Informations

* __First published__ - 1994
* __Digest sizes__ - 256 bits
* __Structure__ - hash
* __Rounds__ - 32

## Authors

FAPSI

## Cryptanalysis

In 2008, an attack was published that breaks the full-round GOST hash function. The paper presents a collision attack in 2^105 time, and first and second preimage attacks in 2^192 time (2^n time refers to the approximate number of times the algorithm was calculated in the attack).

## Implementations

- gostan.c
- gosthash.c
- gosthash.h
- gosttest.c
- Makefile

## Materials

- [GOST R 34.11-94: Hash Function Algorithm](http://tools.ietf.org/html/rfc5831)
- [The GOST Hash Function Ecrypt page](http://ehash.iaik.tugraz.at/wiki/GOST)