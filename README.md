<p align="center">
  <img src="https://raw.github.com/MaciejCzyzewski/retter/master/retter.png" width="128" alt="retter" /><br />
  retter, upholds cryptography<br />
  It's a collection of the most important things.
</p>

---

**NOTE:** retter is not yet finished!!!

---

#### About

BLA BLA BLA BLA BLA BLA BLA

#### Further reading:

- [List of hash functions](#list of hash functions): bla bla bla bla bla
  - [Cyclic redundancy checks](#cyclic redundancy checks): bla bla bla
  - [Checksums](#checksums): bla bla bla
  - [Non-cryptographic hash functions](#non-cryptographic hash functions): bla bla bla
  - [Cryptographic hash functions](#cryptographic hash functions): bla bla bla
- [Tools](#tools): bla bla bla
- [Libraries](#libraries): bla bla bla bla
- [Articles](#articles): bla bla bla

## List of hash functions

This is a list of hash functions, including cyclic redundancy checks, checksum functions, and cryptographic hash functions.

### Cyclic redundancy checks

### Checksums

### Non-cryptographic hash functions

### Cryptographic hash functions

<table width="100%">
  <tr>
    <th width="100%">Name</th>
    <th>Length</th>

    <th>C</th>
    <th>Javascript</th>
    <th>Python</th>
    <th>Ruby</th>
    <th>PHP</th>
  </tr>
  <tr>
    <td>MD5</td>
    <td>128 bits</td>

    <td>✓</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
  </tr>
  <tr>
    <td>MD66</td>
    <td>128 bits</td>

    <td>✕</td>
    <td>✕</td>
    <td>✓</td>
    <td>✕</td>
    <td>✕</td>
  </tr>
</table>

## Tools

### Smhasher

Test your hash functions

* __Authors__ - Laura Winnen
* __Download__ - [https://code.google.com/p/smhasher/](https://code.google.com/p/smhasher/)

SMHasher is a test suite designed to test the distribution, collision, and performance properties of non-cryptographic hash functions - it aims to be the "DieHarder" of hash testing, and does a pretty good job of finding flaws with a number of popular hashes.

### Sage S-box MILP toolkit

Extension of the Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)

* __Authors__ - Laura Winnen
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/sage_sbox_milp.zip](http://www.ecrypt.eu.org/tools/uploads/sage_sbox_milp.zip)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. The toolkit generates a Mixed-Integer Linear Programming problem which counts the minimum number of (linearly or differentially) active S-boxes for a given cipher and solves this using a MILP solver in Sage.

The toolkit includes the implementation in Sage for AES, small AES, Present, Led, mCrypton, Klein and Enocoro.

### KeccakTools

A set of documented C++ classes to help analyze Keccak-f

* __Authors__ - Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche
* __Download__ - [http://keccak.noekeon.org/KeccakTools-3.3.zip](http://keccak.noekeon.org/KeccakTools-3.3.zip)
* __Documentation__ - [http://keccak.noekeon.org/KeccakTools-doc/](http://keccak.noekeon.org/KeccakTools-doc/)

KeccakTools is a set of C++ classes aimed at helping analyze the sponge function family Keccak. Version 3.3 is a major update, as it adds important classes and methods related to differential and linear cryptanalysis. These classes and methods were used to obtain the results reported in the paper Differential propagation anaylsis of Keccak presented at FSE 2012 (also available as ePrint 2012/163).

### S-box MILP toolkit

Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)

* __Authors__ - Nicky Mouha
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip](http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip)
* __Documentation__ - [http://www.cosic.esat.kuleuven.be/publications/article-2080.pdf](http://www.cosic.esat.kuleuven.be/publications/article-2080.pdf)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. The toolkit generates a Mixed-Integer Linear Programming (MILP) problem which counts the minimum number of (linearly or differentially) active S-boxes for a given cipher. Currently, AES and xAES are implemented (both in the single-key and related-key setting), as well as Enocoro-128v2 (in the related-key setting). The technique is very general, and can be adapted to other ciphers with little effort.

### Hashclash

Framework for MD5 & SHA-1 Differential Path Construction and Chosen-Prefix Collisions for MD5

* __Authors__ - Marc Stevens
* __Download__ - [http://code.google.com/p/hashclash/](http://code.google.com/p/hashclash/)

This framework contains tools for the constructions of differential paths for MD5 and SHA-1, including chosen-prefix collisions for MD5.

### ARX Toolkit

The ARX toolkit is a set of tools to study ARX ciphers and hash functions

* __Authors__ - Gaëtan Leurent
* __Download__ - [http://www.di.ens.fr/~leurent/arxtools.html](http://www.di.ens.fr/~leurent/arxtools.html)

The ARX toolkit was presented at the SHA-3 conference in March 2012 in Washington, DC.

### Information Set Decoding

A tool for information set decoding

* __Authors__ - ISD team
* __Download__ - [https://github.com/isd-dev/isd](https://github.com/isd-dev/isd)

This library, written in C++, is reasonably efficient at finding low weight codewords of a linear code using information set decoding.

### S-function Toolkit

Toolkit for the differential cryptanalysis of S-functions

* __Authors__ - Nicky Mouha, Vesselin Velichkov, Christophe De Cannière, Bart Preneel
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/s-function_toolkit_v2.zip](http://www.ecrypt.eu.org/tools/uploads/s-function_toolkit_v2.zip)
* __Documentation__ - [http://www.cosic.esat.kuleuven.be/publications/article-1473.pdf](http://www.cosic.esat.kuleuven.be/publications/article-1473.pdf)

An increasing number of cryptographic primitives use operations such as addition modulo 2^n, multiplication by a constant and bitwise Boolean functions as a source of non-linearity. In NIST’s SHA-3 competition, this applies to 6 out of the 14 second-round candidates. We generalize such constructions by introducing the concept of S-functions.

### CryptLogVer

Toolkit for SAT-based attacks on cryptographic primitives

* __Authors__ - Paweł Morawiecki, Marian Srebrny, Mateusz Srebrny
* __Download__ - [http://www.pawelmorawiecki.pl/cryptlogver](http://www.pawelmorawiecki.pl/cryptlogver)

CryptLogVer is a toolkit that can be used to mount SAT-based attacks on cryptographic primitives (block ciphers, stream ciphers, hash functions). The main advantage of CryptLogVer is that it greatly simplifies the creation of CNF out of a given crypto primitive. CNF formulas coding crypto primitives are usually very challenging for a SAT solver. Therefore CryptLogVer could be also helpful for SAT community in providing hard CNF instances.

### Linear Hull Cryptanalysis of PRESENT

A tool to compute linear hulls for PRESENT

* __Authors__ - Bingsheng Zhang
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip](http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip)

This tool computes linear hulls for the original PRESENT cipher. It confirms and even improves on the predicted bias (and the corresponding attack complexities) of conventional linear relations based on a single linear trail.

### Automated Algebraic Cryptanalysis

A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers

* __Authors__ - Paul Stankovski
* __Download__ - [http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1](http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1)

A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers. Three tests have been implemented and the best results have led to continued work on a computational cluster. Our best results show nonrandomness in Trivium up to 1070 rounds (out of 1152), and in the full Grain-128 with 256 rounds.

### SYMAES

A Fully Symbolic Polynomial System Generator for AES-128

* __Authors__ - Vesselin Velichkov, Vincent Rijmen, Bart Preneel
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/symaes-v1.1.tgz](http://www.ecrypt.eu.org/tools/uploads/symaes-v1.1.tgz)

SYMAES is a software tool that generates a system of polynomials in GF(2), corresponding to the round transformation and key schedule of the block cipher AES-128. Test vectors were added in v1.1.

### Tools for Algebraic Cryptanalysis

Tools for the algebraic cryptanalysis of cryptographic primitives

* __Authors__ - Martin Albrecht
* __Download__ - [http://bitbucket.org/malb/algebraic_attacks](http://bitbucket.org/malb/algebraic_attacks)

Algebraic cryptanalysis of cryptographic primitives such as block ciphers, stream ciphers and hash functions usually proceeds in two steps. (A) The algorithm is expressed as a system of multi-variate equations F over some field (usually F2). (B) The system F is solved using some technique such as Grëbner basis algorithms, SAT solvers or mixed integer programming solvers. We provide scripts and tools for the mathematics software Sage to construct polynomial systems of equations for various block ciphers and conversion routines from algebraic normal form (ANF) to conjunctive normal form (CNF) and mixed integer programmes.

### Hybrid Approach

Hybrid Approach for Solving Multivariate Polynomial Systems over Finite Fields

* __Authors__ - Luk Bettale, Jean-Charles Faugère, Ludovic Perret
* __Download__ - [http://www-polsys.lip6.fr/~bettale/hybrid/](http://www-polsys.lip6.fr/~bettale/hybrid/)

The Hybrid Approach is an improved approach to solve multivariate systems over finite fields. Our approach is a tradeoff between exhaustive search and Gröbner bases Techniques. The efficiency depends on the choice of the tradeoff. Typically, our work permits to refine the parameters to be chosen for multivariate schemes. We present here an implementation of the hybrid approach, and the tools to compute its complexity.

### Grain of Salt

An automated way to test stream ciphers through SAT solvers

* __Authors__ - Mate Soos
* __Download__ - [http://planete.inrialpes.fr/~soos/GrainOfSalt/](http://planete.inrialpes.fr/~soos/GrainOfSalt/)

Grain of Salt is a tool developed to automatically test stream ciphers against standard SAT solver-based attacks. The tool takes as input a set of configuration options and the definition of each filter and feedback function of the stream cipher. It outputs a problem in the language of SAT solvers describing the cipher. The tool can automatically generate SAT problem instances for Crypto-1, HiTag2, Grain, Bivium-B and Trivium. In addition, through a simple text-based interface it can be extended to generate problems for any stream cipher that employs shift registers, feedback and filter functions to carry out its work.

### Sage

Free open-source mathematics software system, also containing many modules for cryptography

* __Authors__ - William A. Stein
* __Download__ - [http://www.sagemath.org/](http://www.sagemath.org/)
* __Documentation__ - [http://www.sagemath.org/doc/reference/cryptography.html](http://www.sagemath.org/doc/reference/cryptography.html)

Sage is a free open-source mathematics software system licensed under the GPL. It combines the power of many existing open-source packages into a common Python-based interface. It also contains modules to perform both basic and more advanced computations in cryptography and cryptanalysis.

## Libraries

+ C
  + [https://github.com/jedisct1/libsodium](libsodium): a modern and easy-to-use crypto library.
  + [https://github.com/DavyLandman/AESLib](AESLib): arduino Library for AES Encryption (source based on avr-crypto-lib).
  + [https://github.com/B-Con/crypto-algorithms](crypto-algorithms): basic implementations of standard cryptography algorithms, like AES and SHA-1.
+ Javascript
  + [jsHashes](https://github.com/h2non/jshashes): pure JavaScript implementation of the most extended hash algorithms.
  + [javascript-crypto-library](https://github.com/clipperz/javascript-crypto-library): provides web developers with an extensive and efficient set of cryptographic functions.
  + [cryptojs](https://github.com/gwjjeff/cryptojs): provide standard and secure cryptographic algorithms for NodeJS.

## Articles

- [Cryptology for Beginners](http://www.mastermathmentor.com/mmm/content/files/crypt/Ciphering%20manual.pdf)
- [Differential Cryptanalysis of MD5](http://www.nada.kth.se/utbildning/grukth/exjobb/rapportlistor/2009/rapporter09/ekera_martin_09008.pdf)
- [Cryptography - Wikibooks](http://en.wikibooks.org/wiki/Cryptography)

## Thanks

* Dongxia Bai, Tsinghua University in China
* Mike Sharov (@msharov)
