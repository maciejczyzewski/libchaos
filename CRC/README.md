# CRC

A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data.

## Informations

* __First published__ - 1961
* __Series__ - CRC-(number)
* __Digest sizes__ - 8, 16, 32, 64 bits
* __Structure__ - cyclic codes

## Authors

W. Wesley Peterson

## Implementations

- crc16.c
- crc32.c

## Materials

- [Reversing CRC – Theory and Practice](http://sar.informatik.hu-berlin.de/research/publications/SAR-PR-2006-05/SAR-PR-2006-05_.pdf)
- [The Great CRC Mystery](http://www.ciphersbyritter.com/ARTS/CRCMYST.HTM)
- [CRC Cyclic Redundancy Check Analysing and Correcting Errors](http://einstein.informatik.uni-oldenburg.de/papers/CRC-BitfilterEng.pdf)
- [Reverse-Engineering a CRC Algorithm](http://www.cosc.canterbury.ac.nz/greg.ewing/essays/CRC-Reverse-Engineering.html)

## Pseudocode

Since a CRC is fundamentally bit-oriented, the order that you assign to bits in bytes matters. You can do it big-endian.

```
function crc(bit array bitString[1..len], int polynomial) {
    shiftRegister := initial value // commonly all 0 bits or all 1 bits
    for i from 1 to len {
        if most significant bit of shiftRegister xor bitString[i] = 1 {
            shiftRegister := (shiftRegister left shift 1) xor polynomial
        } else {
            shiftRegister := (shiftRegister left shift 1)
        }
    }
    return shiftRegister
}
```