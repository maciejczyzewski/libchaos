<p align="center">
 	<img src="https://raw.github.com/MaciejCzyzewski/Retter/gh-pages/Retter.png" alt="Retter"/>
</p>

Retter is engine of RTR series, cryptographic hash function.

### Implementations

<table width="100%">
  <tr>
    <th width="100%">Series</th>
    
    <th>PHP</th>
    <th>Javascript</th>
    <th>C</th>
    <th>Python</th>
    <th>Ruby</th>
    <th>Go</th>
    <th>Perl</th>
    <th>Haskell</th>
    <th>D</th>
    <th>R</th>
  </tr>
  <tr>
    <td>RTR0</td>
    
    <td>✕</td>
    <td>✕</td>
    <td>✓</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
  </tr>
  <tr>
    <td>RTR1</td>
    
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>-</td>
  </tr>
</table>

### Series

#### RTR0

This function is producing a 320-bit (40-byte) hash value. Variable is fixed-length output. <a href="https://github.com/MaciejCzyzewski/Retter/tree/master/RTR0">More informations.</a>

```php
RTR0::hash('');
// string(80) "40a81bda28f1d49984b9146496a295962434ab47b321721a934bddd2be9beffb794000000088ce03"

RTR0::hash('Retter');
// string(80) "bf9a450867b0e43620e7bbef2bc766b909317522fb10be93e6242772697405e090b13ec83d9f333f"

RTR0::hash('Testing');
// string(80) "fab2556f28e9b50e811ae4e550c0c12a68778eab65ca59da5e83b6c400e7bdfefacf18c1702149d7"

RTR0::hash('Festing');
// string(80) "61876ea5a5cdeab4317ee8ae1f5ba270ccaa5b5ede3ca02756a9570529e4ebedfa598aafccaa3713"
```

#### RTR1

The future successor...

### Attacks

Not yet found.

### Thanks

* Dongxia Bai, Tsinghua University in China
* Mike Sharov (@msharov)
