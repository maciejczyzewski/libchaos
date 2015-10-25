# Fowler–Noll–Vo hash function

The basis of the FNV hash algorithm was taken from an idea sent as reviewer comments to the IEEE POSIX P1003.2 committee by Glenn Fowler and Phong Vo in 1991. In a subsequent ballot round, Landon Curt Noll improved on their algorithm.

## Informations

* __First published__ - 1991
* __Digest sizes__ - 32, 64, 128, 256, 512, or 1024 bits
* __Structure__ - xor/product or product/xor

## Authors

Glenn Fowler, Landon Curt Noll, Phong Vo

## Implementations

- fnv.h
- fnv32.c
- fnv64.c

## Materials

- [FNV Hash](http://www.isthe.com/chongo/tech/comp/fnv/index.html)

## Pseudocode

```
hash = FNV_offset_basis
for each octet_of_data to be hashed
  hash = hash × FNV_prime
  hash = hash XOR octet_of_data
return hash
```