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

This function is producing a 128-bit (16-byte) hash value. Variable is fixed-length output. <a href="https://github.com/MaciejCzyzewski/Retter/tree/master/RTR0">More informations.</a>

```php
RTR0::hash('');
// string(32) "e26ad9d937e6309fbdc655edd3c784ad"

RTR0::hash('Retter');
// string(32) "2e82ec63e3f605ac65d6685e1fd7b99f"

RTR0::hash('Testing');
// string(32) "578cb8a598f193ef1ed0349944d367de"

RTR0::hash('Festing');
// string(32) "f764739940f09b42be10f7ace40f466d"
```

#### RTR1

The future successor...

### Attacks

Not yet found.
