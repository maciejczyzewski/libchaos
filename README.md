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

- [Definitions](#definitions): bla bla bla bla bla
- [List of hash functions](#list-of-hash-functions): bla bla bla bla bla
  - [Cyclic redundancy checks](#cyclic-redundancy-checks): bla bla bla
  - [Checksums](#checksums): bla bla bla
  - [Non-cryptographic hash functions](#non-cryptographic-hash-functions): bla bla bla
  - [Cryptographic hash functions](#cryptographic-hash-functions): bla bla bla
- [Tools](#tools): bla bla bla
- [Libraries](#libraries): bla bla bla bla
- [Articles](#articles): bla bla bla

## Definitions

__Avalanche__ - a measure of how the output bits change based on each input bit. Ideally each input bit will affect each output bit with 1/2 probability.

Realistically, many hash functions do not achieve perfect avalanche and are still useable for many sets of keys.

__Cipher__ - an algorithm for performing encryption or decryption—a series of well-defined steps that can be followed as a procedure.

## List of hash functions

This is a list of hash functions, including cyclic redundancy checks, checksum functions, and cryptographic hash functions.

For each function are included: __description__, __implementations__ and __materials__.

### Cyclic redundancy checks

A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data.

### Checksums

A checksum or hash sum is a small-size datum from an arbitrary block of digital data for the purpose of detecting errors which may have been introduced during its transmission or storage.

### Non-cryptographic hash functions

A cryptographic hash function is a hash function which is considered practically impossible to invert, that is, to recreate the input data from its hash value alone.

### Cryptographic hash functions

A cryptographic hash function is a hash function which is considered practically impossible to invert, that is, to recreate the input data from its hash value alone.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Length</th>
    <th>Type</th>
    <th>Speed</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>
  <tr>
    <td>MD5</td>
    <td>128 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>255 MiB/Second</td>
    <td>Ronald Rivest</td>
    <td><a href="/MD5">more</a></td>
  </tr>
  <tr>
    <td>MD5</td>
    <td>128 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>Ronald Rivest</td>
    <td><a href="/MD5">more</a></td>
  </tr>
</table>

## Tools

### Smhasher

_Test your hash functions._

* __Authors__ - Smhasher team
* __Download__ - [https://code.google.com/p/smhasher/](https://code.google.com/p/smhasher/)

SMHasher is a test suite designed to test the distribution, collision, and performance properties of non-cryptographic hash functions - it aims to be the "DieHarder" of hash testing, and does a pretty good job of finding flaws with a number of popular hashes.

### Sage S-box MILP toolkit

_Extension of the Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)._

* __Authors__ - Laura Winnen
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/sage_sbox_milp.zip](http://www.ecrypt.eu.org/tools/uploads/sage_sbox_milp.zip)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis. Includes the implementation in Sage for AES, small AES, Present, Led, mCrypton, Klein and Enocoro.

### KeccakTools

_A set of documented C++ classes to help analyze Keccak-f._

* __Authors__ - Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche
* __Download__ - [http://keccak.noekeon.org/KeccakTools-3.3.zip](http://keccak.noekeon.org/KeccakTools-3.3.zip)
* __Documentation__ - [http://keccak.noekeon.org/KeccakTools-doc/](http://keccak.noekeon.org/KeccakTools-doc/)

KeccakTools is a set of C++ classes aimed at helping analyze the sponge function family Keccak.

### S-box MILP toolkit

_Toolkit for Counting Active S-boxes using Mixed-Integer Linear Programming (MILP)._

* __Authors__ - Nicky Mouha
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip](http://www.ecrypt.eu.org/tools/uploads/sbox-milp.zip)
* __Documentation__ - [http://www.cosic.esat.kuleuven.be/publications/article-2080.pdf](http://www.cosic.esat.kuleuven.be/publications/article-2080.pdf)

This toolkit can be used to prove the security of cryptographic ciphers against linear and differential cryptanalysis.

### Hashclash

_Framework for MD5 & SHA-1 Differential Path Construction and Chosen-Prefix Collisions for MD5._

* __Authors__ - Marc Stevens
* __Download__ - [http://code.google.com/p/hashclash/](http://code.google.com/p/hashclash/)

This framework contains tools for the constructions of differential paths for MD5 and SHA-1, including chosen-prefix collisions for MD5.

### ARX Toolkit

_The ARX toolkit is a set of tools to study ARX ciphers and hash functions._

* __Authors__ - Gaëtan Leurent
* __Download__ - [http://www.di.ens.fr/~leurent/arxtools.html](http://www.di.ens.fr/~leurent/arxtools.html)

The ARX toolkit was presented at the SHA-3 conference in March 2012 in Washington, DC.

### Information Set Decoding

_A tool for information set decoding._

* __Authors__ - ISD team
* __Download__ - [https://github.com/isd-dev/isd](https://github.com/isd-dev/isd)

This library, written in C++, is reasonably efficient at finding low weight codewords of a linear code using information set decoding.

### S-function Toolkit

_Toolkit for the differential cryptanalysis of S-functions._

* __Authors__ - Nicky Mouha, Vesselin Velichkov, Christophe De Cannière, Bart Preneel
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/s-function_toolkit_v2.zip](http://www.ecrypt.eu.org/tools/uploads/s-function_toolkit_v2.zip)
* __Documentation__ - [http://www.cosic.esat.kuleuven.be/publications/article-1473.pdf](http://www.cosic.esat.kuleuven.be/publications/article-1473.pdf)

An increasing number of cryptographic primitives use operations such as addition modulo 2^n, multiplication by a constant and bitwise Boolean functions as a source of non-linearity.

### CryptLogVer

_Toolkit for SAT-based attacks on cryptographic primitives._

* __Authors__ - Paweł Morawiecki, Marian Srebrny, Mateusz Srebrny
* __Download__ - [http://www.pawelmorawiecki.pl/cryptlogver](http://www.pawelmorawiecki.pl/cryptlogver)

CryptLogVer is a toolkit that can be used to mount SAT-based attacks on cryptographic primitives (block ciphers, stream ciphers, hash functions).

### Linear Hull Cryptanalysis of PRESENT

_A tool to compute linear hulls for PRESENT._

* __Authors__ - Bingsheng Zhang
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip](http://www.ecrypt.eu.org/tools/uploads/present-linear-hull.zip)

This tool computes linear hulls for the original PRESENT cipher. It confirms and even improves on the predicted bias (and the corresponding attack complexities) of conventional linear relations based on a single linear trail.

### Automated Algebraic Cryptanalysis

_A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers._

* __Authors__ - Paul Stankovski
* __Download__ - [http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1](http://www.eit.lth.se/index.php?id=260&uhpuid=dhs.pas&hpuid=584&L=1)

A simple tool for the automatic algebraic cryptanalysis of a large array of stream- and block ciphers. Three tests have been implemented and the best results have led to continued work on a computational cluster.

### SYMAES

_A Fully Symbolic Polynomial System Generator for AES-128._

* __Authors__ - Vesselin Velichkov, Vincent Rijmen, Bart Preneel
* __Download__ - [http://www.ecrypt.eu.org/tools/uploads/symaes-v1.1.tgz](http://www.ecrypt.eu.org/tools/uploads/symaes-v1.1.tgz)

SYMAES is a software tool that generates a system of polynomials in GF(2), corresponding to the round transformation and key schedule of the block cipher AES-128.

### Tools for Algebraic Cryptanalysis

_Tools for the algebraic cryptanalysis of cryptographic primitives._

* __Authors__ - Martin Albrecht
* __Download__ - [https://bitbucket.org/malb/research-snippets/src](https://bitbucket.org/malb/research-snippets/src)

Algebraic cryptanalysis of cryptographic primitives such as block ciphers, stream ciphers and hash functions usually proceeds in two steps.

### Grain of Salt

_An automated way to test stream ciphers through SAT solvers._

* __Authors__ - Mate Soos
* __Download__ - [http://planete.inrialpes.fr/~soos/GrainOfSalt/](http://planete.inrialpes.fr/~soos/GrainOfSalt/)

Grain of Salt is a tool developed to automatically test stream ciphers against standard SAT solver-based attacks. The tool takes as input a set of configuration options and the definition of each filter and feedback function of the stream cipher.

### Sage

_Free open-source mathematics software system, also containing many modules for cryptography._

* __Authors__ - William A. Stein
* __Download__ - [http://www.sagemath.org/](http://www.sagemath.org/)
* __Documentation__ - [http://www.sagemath.org/doc/reference/cryptography.html](http://www.sagemath.org/doc/reference/cryptography.html)

Sage is a free open-source mathematics software system licensed under the GPL. It combines the power of many existing open-source packages into a common Python-based interface. It also contains modules to perform both basic and more advanced computations in cryptography and cryptanalysis.

## Libraries

+ C
  + [libsodium](https://github.com/jedisct1/libsodium): a modern and easy-to-use crypto library.
  + [AESLib](https://github.com/DavyLandman/AESLib): arduino Library for AES Encryption (source based on avr-crypto-lib).
  + [crypto-algorithms](https://github.com/B-Con/crypto-algorithms): basic implementations of standard cryptography algorithms, like AES and SHA-1.
  + [sha3sum](https://github.com/maandree/sha3sum): SHA-3 and Keccak checksum utility.
  + [cryptlib](http://www.cryptlib.com/): an open source cross-platform software security toolkit library.
  + [Nettle](http://www.lysator.liu.se/~nisse/nettle/): a cryptographic library that is designed to fit easily in more or less any context.
+ C++
  + [Crypto++](http://www.cryptopp.com/): is a free and open source C++ class library of cryptographic algorithms and schemes.
  + [NaCl](http://nacl.cr.yp.to/): high-speed software library for network communication, encryption, decryption, signatures, etc.
+ Javascript
  + [jsHashes](https://github.com/h2non/jshashes): pure JavaScript implementation of the most extended hash algorithms.
  + [javascript-crypto-library](https://github.com/clipperz/javascript-crypto-library): provides web developers with an extensive and efficient set of cryptographic functions.
  + [cryptojs](https://github.com/gwjjeff/cryptojs): provide standard and secure cryptographic algorithms for NodeJS.
  + [hmacsha1.js](https://gist.github.com/macton/1743087): a JavaScript implementation of the Secure Hash Algorithm, SHA-1, as defined.
+ Clojure
  + [pandect](https://github.com/xsc/pandect): fast and easy-to-use Message Digest, Checksum and HMAC library for Clojure.
+ ActionScript
  + [as3corelib](https://github.com/mikechambers/as3corelib): an ActionScript 3 Library that contains a number of classes and utilities.
+ Ruby
  + [BozoCrack](https://github.com/juuso/BozoCrack): a silly & effective MD5 cracker in Ruby.
+ Objective-c
  + [CocoaSecurity](https://github.com/kelp404/CocoaSecurity): encrypt/Decrypt: AES. Hash: MD5, SHA(SHA1, SHA224, SHA256, SHA384, SHA512). Encode/Decode: Base64, Hex.
+ Rust
  + [rust-crypto](https://github.com/DaGenix/rust-crypto): a (mostly) pure-Rust implementation of various cryptographic algorithms.
+ Java
  + [Bouncy Castle](http://bouncycastle.org/): a collection of APIs used in cryptography. It includes APIs for both the Java and the C# programming languages.

## Articles

- [Cryptology for Beginners](http://www.mastermathmentor.com/mmm/content/files/crypt/Ciphering%20manual.pdf)
- [Differential Cryptanalysis of MD5](http://www.nada.kth.se/utbildning/grukth/exjobb/rapportlistor/2009/rapporter09/ekera_martin_09008.pdf)
- [Cryptography - Wikibooks](http://en.wikibooks.org/wiki/Cryptography)
- [Universal One-Way Hash Functions and their Cryptographic Applications](http://www.wisdom.weizmann.ac.il/~naor/PAPERS/uowhf.pdf)
- [Bloom filters, fast and simple](http://maciejczyzewski.me/2014/10/18/bloom-filters-fast-and-simple.html)
- [Lecture Notes on Cryptography](http://cseweb.ucsd.edu/~mihir/papers/gb.pdf)
- [Cryptograpy Lecture Notes](http://www1.cs.columbia.edu/~zeph/4261/lectures/)
- [Non-Cryptographic Hash Function Zoo](http://floodyberry.com/noncryptohashzoo/)

## Thanks

* Dongxia Bai, Tsinghua University in China
* Mike Sharov (@msharov)
