# Puzzle Encryption Algorithm

Puzzle is a block cipher algorithm based on transposition rather than substitution. It uses the following formula to reorder the plaintext :
FinalPosition = (InitialPosition * KeyBytes + OtherKeyBytes) modulo(BlockSize)

The idea behind this formula is like cutting every letter of a whole book and shuffling them together. The result is so big and mixed up that it is possible to recreate a completely different book from it. This makes reversing the process very hard without the password.

The algorithm has some interesting properties :
* The block size is variable and unknown from an attacker.
* The size of the key has no limit and is unknown from an attacker.
* The key size does not affect the algorithm speed (using a 256 bit key is the same as using a 1024 bit key).
* The algorithm is much faster than the average cryptographic function. Experimental test showed 600 Mo/s - 4 cycles/byte on an Intel Core 2 Duo P8600 2.40GHz and 1,2 Go/s - 2 cycles/byte on an Intel i5-3210M 2.50GHz. Both CPU had only 2 cores.

## Informations

* __First published__ - 2013
* __Structure__ - transposition

## Authors

Gregory Alvarez, Charles Berenguer

## Materials

- [Publication](http://eprint.iacr.org/2013/551.pdf)
