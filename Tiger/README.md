# Tiger

In cryptography, Tiger is a cryptographic hash function designed by Ross Anderson and Eli Biham in 1995 for efficiency on 64-bit platforms. The size of a Tiger hash value is 192 bits. Truncated versions (known as Tiger/128 and Tiger/160) can be used for compatibility with protocols assuming a particular hash size. Unlike the SHA-2 family, no distinguishing initialization values are defined; they are simply prefixes of the full Tiger/192 hash value.

## Informations

* __First published__ - 1996
* __Digest sizes__ - 192 bits
* __Structure__ - Merkle-Damgård construction
* __Rounds__ - 24

## Authors

Ross Anderson and Eli Biham

## Cryptanalysis

John Kelsey and Stefan Lucks have found a collision-finding attack on 16-round Tiger with a time complexity equivalent to about 2^44 compression function invocations and another attack that finds pseudo-near collisions in 20-round Tiger with work less than that of 2^48 compression function invocations.

Florian Mendel et al. have improved upon these attacks by describing a collision attack spanning 19 rounds of Tiger, and a 22-round pseudo-near-collision attack. These attacks require a work effort equivalent to about 2^62 and 2^44 evaluations of the Tiger compression function, respectively.

## Implementations

- Makefile
- sboxes.c
- testtiger.c
- tiger.c

## Materials

- [Collisions and Near-Collisions for Reduced-Round Tiger](http://th.informatik.uni-mannheim.de/People/Lucks/papers/Tiger_FSE_v10.pdf)
- [Tiger: A Fast New Hash Function](http://www.cs.technion.ac.il/~biham/Reports/Tiger/tiger/tiger.html)
- [Tiger website](http://www.cs.technion.ac.il/~biham/Reports/Tiger/)
