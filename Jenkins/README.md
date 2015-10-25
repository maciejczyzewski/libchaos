# Jenkins hash function

The Jenkins hash functions are a collection of (non-cryptographic) hash functions for multi-byte. They can be used also as checksums to detect accidental data corruption or detect identical records in a database.

## Informations

* __First published__ - 1997
* __Digest sizes__ - 32 or 64 bits
* __Structure__ - xor/addition

## Authors

Bob Jenkins

## Implementations

- jenkins.c
- jenkinstest.c

## Materials

- [Dr. Dobbs article](http://www.burtleburtle.net/bob/hash/doobs.html)

## Pseudocode

```
uint32_t jenkins_one_at_a_time_hash(char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
```