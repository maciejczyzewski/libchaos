### Implementations

<table width="100%">
  <tr>
    <th width="100%">Series</th>
    
    <th>PHP</th>
    <th>Javascript</th>
    <th>C++</th>
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
    
    <td>✓</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
    <td>✕</td>
  </tr>
</table>

### RTR0

This function is producing a 128-bit (16-byte) hash value. Variable is fixed-length output.

### Examples

Language used in this example is PHP.

```php
RTR0::hash('');
// string(32) "0997af115a5af21fcdb7d291cfce8a72"

RTR0::hash('Retter');
// string(32) "8ce505146e4bff2edbe8e02e01a3c065"

RTR0::hash('Testing');
// string(32) "d1c7a5399e2c26eb324a49f8a0d7c7de"

RTR0::hash('Festing');
// string(32) "7e3b6054e247cf107c24fb82faf0fdde"
```
