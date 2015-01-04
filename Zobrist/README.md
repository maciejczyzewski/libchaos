# Zobrist hashing

Zobrist hashing is a hash function construction used in computer programs that play abstract board games, such as chess and Go, to implement transposition tables, a special kind of hash table that is indexed by a board position and used to avoid analyzing the same position more than once.

## Informations

* __First published__ - 1969
* __Digest sizes__ - variable
* __Structure__ - xor

## Authors

Albert Lindsey Zobrist

## Implementations

- zobrist.js

## Materials

- [A new hashing method with application for game playing](http://research.cs.wisc.edu/techreports/1970/TR88.pdf)
- [Zobrist keys](http://web.archive.org/web/20070822204038/http://www.seanet.com/~brucemo/topics/zobrist.htm)

## Pseudocode

Example pseudocode for the game of chess.

```
constant indices
       white_pawn := 1
       white_rook := 2
       # etc.
       black_king := 12

   function init_zobrist():
       # fill a table of random numbers/bitstrings
       table := a 2-d array of size 64×12
       for i from 1 to 64:  # loop over the board, represented as a linear array
           for j from 1 to 12:      # loop over the pieces
               table[i][j] = random_bitstring()

   function hash(board):
       h := 0
       for i from 1 to 64:      # loop over the board positions
           if board[i] != empty:
               j := the piece at board[i], as listed in the constant indices, above
               h := h XOR table[i][j]
       return h
```