# Pearson hashing

Pearson hashing is a hash function designed for fast execution on processors with 8-bit registers.

## Informations

* __First published__ - 1990
* __Digest sizes__ - 8 bits
* __Structure__ - xor/table

## Authors

Peter K. Pearson

## Implementations

- pearson.c

## Materials

- [Fast Hashing of Variable Length Text Strings](http://epaperpress.com/vbhash/download/p677-pearson.pdf)

## Pseudocode

```
h := 0
for each c in C loop
  index := h xor c
  h := T[index]
end loop
return h
```