# Damm algorithm

In error detection, the Damm algorithm is a check digit algorithm that detects all single-digit errors and all adjacent transposition errors.

## Informations

* __First published__ - 2004
* __Digest sizes__ - 1 decimal digit
* __Structure__ - Quasigroup operation

## Authors

H. Michael Damm

## Implementations

- damm.c

## Materials

- [Total anti-symmetrische Quasigruppen](http://archiv.ub.uni-marburg.de/diss/z2004/0516/pdf/dhmd.pdf)
- [DAMM tables](http://www.md-software.de/math/DAMM_Quasigruppen.txt)

## Pseudocode

```
"""Damm algorithm decimal check digit"""

# http://www.md-software.de/math/DAMM_Quasigruppen.txt
matrix = (
    (0, 3, 1, 7, 5, 9, 8, 6, 4, 2),
    (7, 0, 9, 2, 1, 5, 4, 8, 6, 3),
    (4, 2, 0, 6, 8, 7, 1, 3, 5, 9),
    (1, 7, 5, 0, 9, 8, 3, 4, 2, 6),
    (6, 1, 2, 3, 0, 4, 5, 9, 7, 8),
    (3, 6, 7, 4, 2, 0, 9, 5, 8, 1),
    (5, 8, 6, 9, 7, 2, 0, 1, 3, 4),
    (8, 9, 4, 5, 3, 6, 2, 0, 1, 7),
    (9, 4, 3, 8, 6, 1, 7, 2, 0, 5),
    (2, 5, 8, 1, 4, 3, 6, 7, 9, 0)
)


def encode(number):
    number = str(number)
    interim = 0

    for digit in number:
        interim = matrix[interim][int(digit)]

    return interim


def check(number):
    return encode(number) == 0


if __name__ == '__main__': # quick sanity checking
    assert encode(572) == 4
    assert check(5724)
    assert encode('43881234567') == 9
```