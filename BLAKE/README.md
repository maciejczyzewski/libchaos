# BLAKE

It is based on Dan Bernstein's ChaCha stream cipher, but a permuted copy of the input block, XORed with some round constants, is added before each ChaCha round.

## Informations

* __First published__ - 2008
* __Series__ - BLAKE, BLAKE2
* __Digest sizes__ - 224, 256, 384 or 512 bits
* __Structure__ - HAIFA structure
* __Rounds__ - 14 or 16

## Authors

Jean-Philippe Aumasson, Luca Henzen, Willi Meier, Raphael C.-W. Phan

## Implementations

- blake.h
- blake224.c
- blake256.c
- blake384.c
- blake512.c
- Makefile

## Materials

- [BLAKE website](https://131002.net/blake/)

## Pseudocode

```
σ[0] =  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
σ[1] = 14 10  4  8  9 15 13  6  1 12  0  2 11  7  5  3
σ[2] = 11  8 12  0  5  2 15 13 10 14  3  6  7  1  9  4
σ[3] =  7  9  3  1 13 12 11 14  2  6  5 10  4  0 15  8
σ[4] =  9  0  5  7  2  4 10 15 14  1 11 12  6  8  3 13
σ[5] =  2 12  6 10  0 11  8  3  4 13  7  5 15 14  1  9
σ[6] = 12  5  1 15 14 13  4 10  0  7  6  3  9  2  8 11
σ[7] = 13 11  7 14 12  1  3  9  5  0 15  4  8  6  2 10
σ[8] =  6 15 14  9 11  3  0  8 12  2 13  7  1  4 10  5
σ[9] = 10  2  8  4  7  6  1  5 15 11  9 14  3 12 13  0

j ← σ[r%10][2×i]            // Index computations
k ← σ[r%10][2×i+1]
a ← a + b + (m[j] ⊕ c[k])   // Step 1 (with input)
d ← (d ⊕ a) >>> 16
c ← c + d                   // Step 2 (no input)
b ← (b ⊕ c) >>> 12
a ← a + b + (m[k] ⊕ c[j])   // Step 3 (with input)
d ← (d ⊕ a) >>> 8
c ← c + d                   // Step 4 (no input)
b ← (b ⊕ c) >>> 7
```