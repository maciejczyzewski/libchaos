# Luhn algorithm

The Luhn algorithm or Luhn formula, also known as the "modulus 10" or "mod 10" algorithm, is a simple checksum formula used to validate a variety of identification numbers, such as credit card numbers, IMEI numbers, National Provider Identifier numbers in US and Canadian Social Insurance Numbers.

## Informations

* __First published__ - 1954
* __Digest sizes__ - 4 bits
* __Structure__ - sum

## Authors

Hans Peter Luhn

## Implementations

- luhn.c

## Materials

- [Check Digit Algorithm](https://wiki.openmrs.org/display/docs/Check+Digit+Algorithm)

## Pseudocode

The implementations below are in Python.

```python
def luhn_checksum(card_number):
    def digits_of(n):
        return [int(d) for d in str(n)]
    digits = digits_of(card_number)
    odd_digits = digits[-1::-2]
    even_digits = digits[-2::-2]
    checksum = 0
    checksum += sum(odd_digits)
    for d in even_digits:
        checksum += sum(digits_of(d*2))
    return checksum % 10

def calculate_luhn(partial_card_number):
    check_digit = luhn_checksum(int(partial_card_number) * 10)
    return check_digit if check_digit == 0 else 10 - check_digit

def is_luhn_valid(card_number):
    return luhn_checksum(card_number) == 0
```