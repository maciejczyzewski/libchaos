# Skein

Skein is a cryptographic hash function and one of five finalists in the NIST hash function competition. Entered as a candidate to become the SHA-3 standard, the successor of SHA-1 and SHA-2, it ultimately lost to NIST hash candidate Keccak.

## Informations

* __First published__ - 2010
* __Digest sizes__ - arbitrary
* __Structure__ - Unique Block Iteration

## Authors

Bruce Schneier, Niels Ferguson

## Cryptanalysis

In October 2010, an attack that combines rotational cryptanalysis with the rebound attack was published. The attack finds rotational collisions for 53 of 72 rounds in Threefish-256, and 57 of 72 rounds in Threefish-512. It also affects the Skein hash function. This is a follow-up to the earlier attack published in February, which breaks 39 and 42 rounds respectively.

## Implementations

- brg_endian.h
- brg_types.h
- SHA3api_ref.c
- SHA3api_ref.h
- skein.c
- skein.h
- skein_block.c
- skein_debug.c
- skein_debug.h
- skein_port.h

## Materials

- [The Skein Hash Function Family](http://www.skein-hash.info/sites/default/files/skein1.3.pdf)
- [Skein website](http://www.skein-hash.info/)