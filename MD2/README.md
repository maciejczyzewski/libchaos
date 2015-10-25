# MD2

The MD2 Message-Digest Algorithm is a cryptographic hash function. The algorithm is optimized for 8-bit computers. Although MD2 is no longer considered secure, even as of 2014, it remains in use in public key infrastructures as part of certificates generated with MD2 and RSA.

## Informations

* __First published__ - August 1989
* __Series__ - MD2, MD4, MD5, MD6
* __Digest sizes__ - 128 bits
* __Structure__ - hash
* __Rounds__ - 18

## Authors

Ronald Rivest

## Cryptanalysis

Rogier and Chauvaud (1997) described collisions of MD2's compression function, although they were unable to extend the attack to the full MD2.

In 2009, security updates were issued disabling MD2 in OpenSSL, GnuTLS, and Network Security Services.

## Implementations

- md2.c

## Materials

- [The MD2 Message-Digest Algorithm](http://tools.ietf.org/html/rfc1319)
- [How is the MD2 hash function S-table constructed from Pi?](https://crypto.stackexchange.com/questions/11935/how-is-the-md2-hash-function-s-table-constructed-from-pi)
