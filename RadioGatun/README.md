# RadioGatún

RadioGatún is a cryptographic hash primitive created by Guido Bertoni, Joan Daemen, Michaël Peeters, and Gilles Van Assche. It was first publicly presented at the NIST Second Cryptographic Hash Workshop, held in Santa Barbara, California, on August 24–25, 2006, as part of the NIST hash function competition.

## Informations

* __First published__ - 2006
* __Digest sizes__ - Up to 1216 bits
* __Structure__ - hash

## Authors

Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche

The most effective attack against the algorithm, one with a complexity of 2^11w, is given in the paper "Cryptanalysis of RadioGatun" by Thomas Fuhr and Thomas Peyrin. While more effective than the other attacks, this attack still does not break the security claim.

## Implementations

- common.cpp
- common.h
- fastradiogatun.c
- fastradiogatun.h
- fastradiogatun.macros
- radiogatun.h
- ref_main.cpp
- RG-inputstrings
- RG32-testvectors
- RG64-testvectors

## Materials

- [RadioGatún](http://radiogatun.noekeon.org/)

## Pseudocode

```
test_rg32 = 1 # Make this 0 if using this as a library

# Copyright (c) 2012-2013 Sam Trenholme
#
# TERMS
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# This software is provided 'as is' with no guarantees of correctness or
# fitness for purpose.

# This is a Python implementation of RadioGatun32.  It takes about 10
# seconds to set up the RNG, then it can output approximately
# 30,000 16-bit random numbers per second on a Core i5-2430M

# I would like to thank Lorenzo for his suggestion to use xrange to speed up
# the program

class RadioGatun32:
  def __init__(self, m):
    self.wordsize = 32
    self.millsize = 19
    self.beltrows = 3
    self.beltcol = 13
    self.beltfeed = 12
    self.mask = 0xffffffff # 32-bit
    self.index = 0
    (self.a, self.b) = self.seed(m)
  def mill(self,a):
    aa = []
    for g in xrange(self.millsize):
      aa.append(0)
    x = 0
    i = 0
    y = 0
    r = 0
    z = 0
    for i in xrange(self.millsize):
      y = (i * 7) % self.millsize
      r = ((i * (i + 1)) / 2) % self.wordsize
      x = a[y] ^ (a[ ((y + 1) % self.millsize) ] |
          (a[ ((y + 2) % self.millsize) ] ^ self.mask))
      aa[i] = ((x >> r) | (x << (self.wordsize - r))
              & self.mask)
    for i in xrange(self.millsize):
      y = i
      r = (i + 1) % self.millsize
      z = (i + 4) % self.millsize
      a[i] = aa[y] ^ aa[r] ^ aa[z]
    a[0] ^= 1
    return a
  def belt(self,a,b):
    q = []
    for g in xrange(self.beltrows):
      q.append(0)
    s = 0
    i = 0
    v = 0
    for s in xrange(self.beltrows):
      q[s] = b[((s * self.beltcol) + self.beltcol - 1)]
    i = self.beltcol - 1
    while i > 0:
      for s in xrange(self.beltrows):
        v = i - 1
        if v < 0:
          v = self.beltcol - 1
        b[((s * self.beltcol) + i)] = (
          b[((s * self.beltcol) + v)])
      i -= 1
    for s in xrange(self.beltrows):
      b[(s * self.beltcol)] = q[s]
    for i in xrange(self.beltfeed):
      s = (i + 1) + ((i % self.beltrows) * self.beltcol)
      b[s] ^= a[(i + 1)]
    a = self.mill(a)
    for i in xrange(self.beltrows):
      a[(i + self.beltcol)] ^= q[i]
    return (a, b)
  def seed(self,m):
    p = []
    for g in xrange(3):
      p.append(0)
    q = 0
    c = 0
    r = 0
    done = 0
    index = 0
    counter = 0
    a = []
    b = []
    for g in xrange(self.millsize):
      a.append(0)
    for g in xrange(self.beltrows * self.beltcol):
      b.append(0)
    for counter in xrange(16777218): # Infinite loop protection
      p[0] = p[1] = p[2] = 0
      for r in xrange(3):
        q = 0
        while q < self.wordsize:
          x = 0
          try:
            x = ord(m[index])
          except:
            x = 1
          index += 1
          if(index > len(m)):
            done = 1
            x = 1
          p[r] |= x << q
          if done == 1:
            for c in xrange(3):
              b[c * 13] ^= p[c]
              a[16 + c] ^= p[c]
            (a,b) = self.belt(a,b)
            for c in xrange(16):
              (a,b) = self.belt(a,b)
            return (a,b)
          q += 8
      for c in xrange(3):
        b[c * 13] ^= p[c]
        a[16 + c] ^= p[c]
      (a,b) = self.belt(a,b)
    return (a,b) # We should never get here
  def rng(self):
    if (self.index % 4) == 0:
      (self.a, self.b) = self.belt(self.a, self.b)
      self.index += 1
      return (((self.a[1] & 0xff) << 8) |
         ((self.a[1] & 0xff00) >> 8))
    self.index += 1
    if (self.index % 4) == 2:
      return(((self.a[1] & 0xff0000) >> 8) |
        ((self.a[1] & 0xff000000) >> 24))
    elif (self.index % 4) == 3:
      return(((self.a[2] & 0xff) << 8) |
        ((self.a[2] & 0xff00) >> 8))
    elif (self.index % 4) == 0:
      return(((self.a[2] & 0xff0000) >> 8) |
        ((self.a[2] & 0xff000000) >> 24))
    else: # Should never get here
      return -1

if test_rg32 == 1:
  q = RadioGatun32("12345678")
  for a in xrange(10001):
    print "%04x%04x%04x%04x" % (q.rng(),q.rng(),q.rng(),q.rng())
```