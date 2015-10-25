<p align="center">
  <img src="https://raw.github.com/MaciejCzyzewski/retter/master/retter.png" width="128" alt="retter" /><br />
  retter, upholds cryptography<br />
  It's a collection of hash functions, ciphers, tools, libraries, and materials related to cryptography.
</p>

---

**NOTE:** If you see mistakes or would like to expand the list. Feel free to contribute to this project! It is still in the process of creating...

---

#### About

The purpose of this project is to promote and develop cryptography.

It includes the collection of known tools, libraries, articles, materials, hash functions, and ciphers.

Also it's a place for people who share an interest in cryptography and want to discuss about it.

__If you invent a new hash function or ciphers, or write an paper related to cryptography just add it here. That is the right place. See [contributing section in README.md](#contributing)__

#### Further reading:

- [Definitions](#definitions): Basic concepts.
- [List of hash functions](#list-of-hash-functions): The set of all known functions.
  - [Cyclic redundancy checks](#cyclic-redundancy-checks): List of cyclic redundancy checks functions.
  - [Checksums](#checksums): List of checksums functions.
  - [Non-cryptographic hash functions](#non-cryptographic-hash-functions): List of non-cryptographic hash functions.
  - [Cryptographic hash functions](#cryptographic-hash-functions): List of cryptographic hash functions.
  - [Message authentication codes](#message-authentication-codes): List of message authentication code functions.
- [List of ciphers](#list-of-ciphers): The set of all known ciphers.
  - [Symmetric key algorithms](#symmetric-key-algorithms): List of symmetric key algorithms.
  - [Asymmetric key algorithms](#asymmetric-key-algorithms): List of asymmetric key algorithms.
- [Tools](#tools): Collection of cryptanalysis apparatus.
- [Libraries](#libraries): List of security/crypto libraries.
- [Articles](#articles): Pages related to cryptography.
- [Contributing](#contributing): Explanation of how you can join the project.

## Definitions

### Hash functions

__Hash function__ - any function that can be used to map digital data of arbitrary size to digital data of fixed size, with slight differences in input data producing very big differences in output data.

_Used in: Database, Filters, Hash tables, Unique identifiers, Passwords, Diff utilities, and File system_

__Avalanche__ - a measure of how the output bits change based on each input bit. Ideally each input bit will affect each output bit with 1/2 probability.

_Realistically, many hash functions do not achieve perfect avalanche and are still useable for many sets of keys._

### Ciphers

__Cipher__ - an algorithm for performing encryption or decryption—a series of well-defined steps that can be followed as a procedure.

_There are 2 types of key algorithms: Symmetric and Asymmetric_

__Block cipher__ - a deterministic algorithm operating on fixed-length groups of bits, called blocks, with an unvarying transformation that is specified by a symmetric key.

_It can also be continuous stream of symbols. (stream ciphers)_

## List of hash functions

This is a list of hash functions, including cyclic redundancy checks, checksum functions, and cryptographic hash functions.

For each function are included: __description__, __implementations__ and __materials__.

### Cyclic redundancy checks

A __cyclic redundancy check__ (often __CRC__) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Length</th>
    <th>Type</th>
    <th>Published</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>

  <tr>
    <td>CRC</td>
    <td>8, 16, 32, 64 bits</td>
    <td>cyclic codes</td>
    <td>1961</td>
    <td>W. Wesley Peterson</td>
    <td><a href="/CRC">more [⬆]</a></td>
  </tr>
  <tr>
    <td>BSD</td>
    <td>16 bits</td>
    <td>CRC</td>
    <td>--</td>
    <td>--</td>
    <td><a href="/BSD">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SYSV</td>
    <td>16 bits</td>
    <td>CRC</td>
    <td>--</td>
    <td>--</td>
    <td><a href="/SYSV">more [⬆]</a></td>
  </tr>
</table>

### Checksums

A __checksum__ or __hash sum__ is a small-size datum from an arbitrary block of digital data for the purpose of detecting errors which may have been introduced during its transmission or storage.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Length</th>
    <th>Type</th>
    <th>Published</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>

  <tr>
    <td>Luhn algorithm</td>
    <td>4 bits</td>
    <td>sum</td>
    <td>1954</td>
    <td>Hans Peter Luhn</td>
    <td><a href="/Luhn">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Verhoeff algorithm</td>
    <td>4 bits</td>
    <td>sum</td>
    <td>1969</td>
    <td>Jacobus Verhoeff</td>
    <td><a href="/Verhoeff">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Fletcher</td>
    <td>4, 8, 16, 32 bits</td>
    <td>sum</td>
    <td>1970</td>
    <td>John Gould Fletcher</td>
    <td><a href="/Fletcher">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Adler-32</td>
    <td>32 bits</td>
    <td>sum</td>
    <td>1995</td>
    <td>Mark Adler</td>
    <td><a href="/Adler-32">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Damm algorithm</td>
    <td>1 decimal digit</td>
    <td>Quasigroup operation</td>
    <td>2004</td>
    <td>H. Michael Damm</td>
    <td><a href="/Damm">more [⬆]</a></td>
  </tr>
</table>

### Non-cryptographic hash functions

A __non-cryptographic hash function__ is a hash function which is sometimes possible to invert.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Length</th>
    <th>Type</th>
    <th>Published</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>

  <tr>
    <td>Zobrist hashing</td>
    <td>variable</td>
    <td>xor</td>
    <td>1969</td>
    <td>Albert Lindsey Zobrist</td>
    <td><a href="/Zobrist">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Pearson hashing</td>
    <td>8 bits</td>
    <td>xor/table</td>
    <td>1990</td>
    <td>Peter K. Pearson</td>
    <td><a href="/Pearson">more [⬆]</a></td>
  </tr>
  <tr>
    <td>FNV</td>
    <td>32, 64, 128, 256, 512, or 1024 bits</td>
    <td>xor/product or product/xor</td>
    <td>1991</td>
    <td>Glenn Fowler, Landon Curt Noll, Phong Vo</td>
    <td><a href="/FNV">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Jenkins</td>
    <td>32 or 64 bits</td>
    <td>xor/addition</td>
    <td>1997</td>
    <td>Bob Jenkins</td>
    <td><a href="/Jenkins">more [⬆]</a></td>
  </tr>
  <tr>
    <td>MurmurHash</td>
    <td>32, 64, or 128 bits</td>
    <td>product/rotation</td>
    <td>2008</td>
    <td>Austin Appleby</td>
    <td><a href="/MurmurHash">more [⬆]</a></td>
  </tr>
  <tr>
    <td>CityHash</td>
    <td>64, 128, or 256 bits</td>
    <td>product/rotation</td>
    <td>2010</td>
    <td>Geoff Pike, Jyrki Alakuijala</td>
    <td><a href="/CityHash">more [⬆]</a></td>
  </tr>
  <tr>
    <td>xxHash</td>
    <td>32, 64 bits</td>
    <td>product/rotation</td>
    <td>2012</td>
    <td>Yann Collet</td>
    <td><a href="/xxHash">more [⬆]</a></td>
  </tr>
</table>

### Cryptographic hash functions

A __cryptographic hash function__ is a hash function which is designed to be difficult to invert, that is, given an arbitrary hash value it should be hard to find an input that hashes to the same value.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Length</th>
    <th>Type</th>
    <th>Published</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>

  <tr>
    <td>MD2</td>
    <td>128 bits</td>
    <td>hash</td>
    <td>1989</td>
    <td>Ronald Rivest</td>
    <td><a href="/MD2">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Snefru</td>
    <td>128 or 256 bits</td>
    <td>hash</td>
    <td>1990</td>
    <td>Ralph Merkle</td>
    <td><a href="/Snefru">more [⬆]</a></td>
  </tr>
  <tr>
    <td>MD4</td>
    <td>128 bits</td>
    <td>hash</td>
    <td>1990</td>
    <td>Ronald Rivest</td>
    <td><a href="/MD4">more [⬆]</a></td>
  </tr>
  <tr>
    <td>HAVAL</td>
    <td>128 to 256 bits</td>
    <td>hash</td>
    <td>1992</td>
    <td>Yuliang Zheng, Josef Pieprzyk, Jennifer Seberry</td>
    <td><a href="/HAVAL">more [⬆]</a></td>
  </tr>
  <tr>
    <td>MD5</td>
    <td>128 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>1992</td>
    <td>Ronald Rivest</td>
    <td><a href="/MD5">more [⬆]</a></td>
  </tr>
  <tr>
    <td>GOST</td>
    <td>256 bits</td>
    <td>hash</td>
    <td>1994</td>
    <td>FAPSI</td>
    <td><a href="/GOST">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SHA-1</td>
    <td>160 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>1995</td>
    <td>National Security Agency</td>
    <td><a href="/SHA-1">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Tiger</td>
    <td>192 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>1996</td>
    <td>Ross Anderson, Eli Biham</td>
    <td><a href="/Tiger">more [⬆]</a></td>
  </tr>
  <tr>
    <td>RIPEMD</td>
    <td>128, 160, 256, 320 bits</td>
    <td>hash</td>
    <td>1996</td>
    <td>Hans Dobbertin, Antoon Bosselaers and Bart Preneel</td>
    <td><a href="/RIPEMD">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Whirlpool</td>
    <td>512 bits</td>
    <td>Miyaguchi-Preneel</td>
    <td>2000</td>
    <td>Vincent Rijmen, Paulo S. L. M. Barreto</td>
    <td><a href="/Whirlpool">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SHA-2</td>
    <td>224, 256, 384, or 512 bits</td>
    <td>Merkle-Damgård construction</td>
    <td>2001</td>
    <td>National Security Agency</td>
    <td><a href="/SHA-2">more [⬆]</a></td>
  </tr>
  <tr>
    <td>HAS-160</td>
    <td>160 bits</td>
    <td>hash</td>
    <td>2002</td>
    <td>Korean</td>
    <td><a href="/HAS-160">more [⬆]</a></td>
  </tr>
  <tr>
    <td>FSB</td>
    <td>160 to 512 bits</td>
    <td>hash</td>
    <td>2003</td>
    <td>Daniel Augot, Matthieu Finiasz, Nicolas Sendrier</td>
    <td><a href="/FSB">more [⬆]</a></td>
  </tr>
  <tr>
    <td>RadioGatún</td>
    <td>Up to 1216 bits</td>
    <td>hash</td>
    <td>2006</td>
    <td>Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche</td>
    <td><a href="/RadioGatún">more [⬆]</a></td>
  </tr>
  <tr>
    <td>BLAKE</td>
    <td>256 or 512 bits</td>
    <td>HAIFA structure</td>
    <td>2008</td>
    <td>Jean-Philippe Aumasson, Luca Henzen, Willi Meier, Raphael C.-W. Phan</td>
    <td><a href="/BLAKE">more [⬆]</a></td>
  </tr>
  <tr>
    <td>ECOH</td>
    <td>224 to 512 bits</td>
    <td>hash</td>
    <td>2008</td>
    <td>Daniel R. L. Brown, Matt Campagna, Rene Struik</td>
    <td><a href="/ECOH">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Grøstl</td>
    <td>256 to 512 bits</td>
    <td>hash</td>
    <td>2008</td>
    <td>Praveen Gauravaram, Lars Knudsen, Krystian Matusiewicz, Florian Mendel, Christian Rechberger, Martin Schläffer, Søren S. Thomsen</td>
    <td><a href="/Grøstl">more [⬆]</a></td>
  </tr>
  <tr>
    <td>JH</td>
    <td>512 bits</td>
    <td>hash</td>
    <td>2008</td>
    <td>Hongjun Wu</td>
    <td><a href="/JH">more [⬆]</a></td>
  </tr>
  <tr>
    <td>MD6</td>
    <td>128 bits</td>
    <td>Merkle tree NLFSR</td>
    <td>2008</td>
    <td>Ronald Rivest, Benjamin Agre, Dan Bailey, Sarah Cheng, Christopher Crutchfield, Yevgeniy Dodis, Kermin Fleming, Asif Khan, Jayant Krishnamurthy, Yuncheng Lin, Leo Reyzin, Emily Shen, Jim Sukha, Eran Tromer, Yiqun Lisa Yin</td>
    <td><a href="/MD6">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SWIFFT</td>
    <td>512 bits</td>
    <td>hash</td>
    <td>2008</td>
    <td>Vadim Lyubashevsky, Daniele Micciancio, Chris Peikert, Alon Rosen</td>
    <td><a href="/SWIFFT">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Spectral Hash</td>
    <td>512 bits</td>
    <td>Wide Pipe Merkle-Damgård construction</td>
    <td>2009</td>
    <td>Gokay Saldamlı, Cevahir Demirkıran, Megan Maguire, Carl Minden, Jacob Topper, Alex Troesch, Cody Walker, Çetin Kaya Koç</td>
    <td><a href="/Spectral">more [⬆]</a></td>
  </tr>
  <tr>
    <td>Skein</td>
    <td>arbitrary</td>
    <td>Unique Block Iteration</td>
    <td>2010</td>
    <td>Bruce Schneier, Niels Ferguson</td>
    <td><a href="/Skein">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SHA-3 (Keccak)</td>
    <td>arbitrary</td>
    <td>Sponge function</td>
    <td>2012</td>
    <td>Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche</td>
    <td><a href="/SHA-3">more [⬆]</a></td>
  </tr>
  <tr>
    <td>SipHash</td>
    <td>64 bits</td>
    <td>non-collision-resistant PRF</td>
    <td>2012</td>
    <td>Jean-Philippe Aumasson, Daniel J. Bernstein</td>
    <td><a href="/SipHash">more [⬆]</a></td>
  </tr>
</table>

### Message authentication codes

A __message authentication code__ (often __MAC__) is a short piece of information used to authenticate a message and to provide integrity and authenticity assurances on the message.

<table width="100%">
  <tr>
    <th>Name</th>
    <th>Published</th>
    <th>Authors</th>
    <th>Information</th>
  </tr>

  <tr>
    <td>HMAC</td>
    <td>1996</td>
    <td>Mihir Bellare, Ran Canetti, Hugo Krawczyk</td>
    <td><a href="http://tools.ietf.org/html/rfc2104">specification</a></td>
  </tr>
  <tr>
    <td>UMAC</td>
    <td>1999</td>
    <td>John Black, Shai Halevi, Hugo Krawczyk, Ted Krovetz, Phillip Rogaway</td>
    <td><a href="http://www.fastcrypto.org/vmac/draft-krovetz-vmac-01.txt">specification</a></td>
  </tr>
  <tr>
    <td>OMAC</td>
    <td>2002</td>
    <td>Tetsu Iwata, Kaoru Kurosawa</td>
    <td><a href="http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/omac/omac-spec.pdf">specification</a></td>
  </tr>
  <tr>
    <td>CMAC</td>
    <td>2003</td>
    <td>John Black, Phillip Rogaway</td>
    <td><a href="http://web.cs.ucdavis.edu/~rogaway/papers/3k.pdf">specification</a></td>
  </tr>
  <tr>
    <td>VMAC</td>
    <td>2007</td>
    <td>Ted Krovetz, Wei Dai</td>
    <td><a href="http://www.fastcrypto.org/vmac/draft-krovetz-vmac-01.txt">specification</a></td>
  </tr>
</table>

## List of ciphers

### Symmetric key algorithms

Not ready yet...

### Asymmetric key algorithms

Not ready yet...

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
  + [RHash](https://github.com/rhash/RHash): great utility for computing hash sums.
  + [cryptlib](http://www.cryptlib.com/): an open source cross-platform software security toolkit library.
  + [Nettle](http://www.lysator.liu.se/~nisse/nettle/): a cryptographic library that is designed to fit easily in more or less any context.
+ C++
  + [Crypto++](http://www.cryptopp.com/): is a free and open source C++ class library of cryptographic algorithms and schemes.
  + [HElib](https://github.com/shaih/HElib): an Implementation of homomorphic encryption.
  + [NaCl](http://nacl.cr.yp.to/): high-speed software library for network communication, encryption, decryption, signatures, etc.
+ C#
  + [Data.HashFunction](https://github.com/brandondahler/Data.HashFunction): Common interface library to non-cryptographic hash functions with numerous implementations.
+ Python
  + [pycrypto](https://www.dlitz.net/software/pycrypto/): Widely used package containing various cryptographic modules and functions
  + [Py2 Standard Library Cryptographic Services](https://docs.python.org/2/library/crypto.html): A set of utilities included with Python 2. Attention! Some modules are deprecated (`md5` and `sha`).
  + [Py3 Standard Library Cryptographic Services](https://docs.python.org/3.4/library/crypto.html): A set of utilities included with Python 3. Some API changes (`md5` and `sha` were moved inside the `hashlib` module).
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
  + [RNCryptor](https://github.com/RNCryptor/RNCryptor): CCCryptor (AES encryption) wrappers for iOS and Mac.
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
- [What is the difference between a Hash Function and a Cryptographic Hash Function?](http://security.stackexchange.com/questions/11839/what-is-the-difference-between-a-hash-function-and-a-cryptographic-hash-function)
- [Whats the difference between MAC vs hash](http://security.stackexchange.com/questions/1873/whats-the-difference-between-mac-vs-hash?rq=1)
- [Dr. Dobbs article](http://www.burtleburtle.net/bob/hash/doobs.html)
- [Attacks on Hash Functions and Applications](https://marc-stevens.nl/research/papers/PhD%20Thesis%20Marc%20Stevens%20-%20Attacks%20on%20Hash%20Functions%20and%20Applications.pdf)
- [Which hashing algorithm is best for uniqueness and speed?](http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed)

## Contributing

Please feel free to contribute to this project! Pull requests and feature requests welcome! :v:

### New hash function

Anyone can create a function. You can add to this list a function that is not yours but it already exists. To make your own, remember that the following conditions must be met.

* __Unique name__ - That which does not exist yet.
* __Informations__ - Date of creation, structure, first published, series, etc.
* __Authors__ - Everyone who worked on it.
* __Cryptanalysis__ -  The disadvantages, advantages.
* __Materials__ - Your research, documents.
* __Implementations__ - Example implementation (C, C++).
* __Pseudocode__ - Outline of the algorithm.

_Add it to the appropriate section in the README.md and create a new directory as have other functions. (ex. <a href="/Pearson">Pearson hashing</a>)_

## Thanks

* Dongxia Bai, Tsinghua University in China
* Mike Sharov ([@msharov](https://github.com/msharov))
* Others who have written tons of material!!!
