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
// string(40) "f58e6b727a62233b4203f551047a721144cfaa94"

RTR0::hash('Retter');
// string(40) "7377bc6d704488272c8297960b02626dafb3c9c1"

RTR0::hash('Testing');
// string(40) "4efaf06a90b5908e874992855525e33e2cf8885d"

RTR0::hash('Festing');
// string(40) "b6f05082b04bbaa72597777a4fc283f52ccbd19e"
```

#### RTR1

The future successor...

### Attacks

Not yet found.
