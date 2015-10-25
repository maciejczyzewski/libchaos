# MD6

The MD6 Message-Digest Algorithm is a cryptographic hash function. It uses a Merkle tree-like structure to allow for immense parallel computation of hashes for very long inputs. Authors claim a performance of 28 cycles per byte for MD6-256 on an Intel Core 2 Duo and provable resistance against differential cryptanalysis.

## Informations

* __First published__ - 2008
* __Series__ - MD2, MD4, MD5, MD6
* __Digest sizes__ - 128 bits
* __Structure__ - Merkle tree

## Authors

Ronald Rivest, Benjamin Agre, Dan Bailey, Sarah Cheng, Christopher Crutchfield, Yevgeniy Dodis, Kermin Fleming, Asif Khan, Jayant Krishnamurthy, Yuncheng Lin, Leo Reyzin, Emily Shen, Jim Sukha, Eran Tromer, Yiqun Lisa Yin

## Implementations

- md6.h
- md6_compress.c
- md6_mode.c
- md6_nist.h
- md6_nist.c
- inttypes.h
- stdint.h

## Materials

- [The MD6 hash function A proposal to NIST for SHA-3](http://groups.csail.mit.edu/cis/md6/submitted-2008-10-27/Supporting_Documentation/md6_report.pdf)
- [MD6 website](http://groups.csail.mit.edu/cis/md6/)
