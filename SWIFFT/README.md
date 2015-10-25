# SWIFFT

In cryptography, SWIFFT is a collection of provably secure hash functions. It is based on the concept of the Fast Fourier Transform (FFT). SWIFFT is not the first hash function based on FFT, but it sets itself apart by providing a mathematical proof of its security. It also uses the LLL basis reduction algorithm.

## Informations

* __First published__ - 2008
* __Digest sizes__ - 512 bits
* __Structure__ - hash

## Authors

Vadim Lyubashevsky, Daniele Micciancio, Chris Peikert, Alon Rosen

## Cryptanalysis

Known working attacks are: Generalized Birthday Attack, which takes 2^106 operations and inversion attacks which takes 2^448 operations for a standard parameter choice. This is usually considered to be enough to render an attack by an adversary infeasible.

## Materials

- [SWIFFT: A Modest Proposal for FFT Hashing](http://www.eecs.harvard.edu/~alon/PAPERS/lattices/swifft.pdf)

