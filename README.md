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

This function is producing a 160-bit (20-byte) hash value. Variable is fixed-length output. <a href="https://github.com/MaciejCzyzewski/Retter/tree/master/RTR0">More informations.</a>

```php
RTR0::hash('');
// string(40) "cf7a017ad356e74c7a6b31663d0eb6668f7fc011"

RTR0::hash('Retter');
// string(40) "e688dc562ac6cc1d995a73dc8dc53c313e7e30ae"

RTR0::hash('Testing');
// string(40) "701d77c5134db266faa0563f0c7b9a7b8c0d7b48"

RTR0::hash('Festing');
// string(40) "561e7edbb15e6ec0041c832fa43177c59e7f3969"
```

#### RTR1

The future successor...

### Attacks

Not yet found.

### Thanks

* Dongxia Bai, Tsinghua University in China
* Mike Sharov (@msharov)
