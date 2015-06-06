# Naive Czyzewski Generator

A NCG is a randomization function, specified in 2015 by Maciej A. Czyzewski.
Generator has a arbitrary period 2^(n/2), and works on 32-bit input/output blocks.

## Informations

* __First published__ - June 2015
* __Digest sizes__ - arbitrary
* __Structure__ - state matrix

## Authors

Maciej Anthony Czyzewski

## Implementations

- src/ncg.c
- src/include/hash.c
- src/include/random.c

## Materials

- [NCG website](http://maciejczyzewski.me/2015/06/05/naive-czyzewski-generator.html)
- [NCG paper](http://maciejczyzewski.me/assets/files/NCG.pdf)