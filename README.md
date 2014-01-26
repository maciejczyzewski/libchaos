<p align="center">
 	<img src="https://raw.github.com/MaciejCzyzewski/Retter/gh-pages/Retter.png" alt="Retter"/>
</p>

Retter is engine of RTR0, cryptographic hash function.

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
  </tr>
  <tr>
    <td>RTR0</td>
    
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
  </tr>
</table>

### Examples

#### RTR0

```php
RTR0::hash('');
// string(32) "6b2dc960002528ece1c56ee7ada9a4d9"

RTR0::hash('Retter');
// string(32) "867e4445297098c5911659d5ef5b6ec3"

RTR0::hash('Testing');
// string(32) "179c43e081543a24adc732db167159c5"

RTR0::hash('Festing');
// string(32) "7dd924d8e3a15de431ce748a2a33b790"
```

### Attacks

Not yet found.
