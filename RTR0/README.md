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

#### Pseudocode

```
1) Prepare
    a) Unpack data to ascii chunks (Starting from 1)
    c) Create a drawers with sample values
        A = 0x428a2f98
        B = 0xefbe4786
        C = 0xbef9a3f7
        D = 0xab1c5ed5
    b) Addition of free bits
        chunks[0] = A // First
        chunks[]  = B // length - 2
        chunks[]  = C // length - 1
        chunks[]  = D // Last
    d) Addition 2 to length
2) Main loop
    (do this for i = 1 to <= length)
        a) Taking values ​​with neighbors
            a = chunks[i-1]
            b = chunks[i]
            c = chunks[i+1]
        b) Rotate values, insisting third value
            left = (a >> b % c) | (a << (sizeof(a) * 8 - b % c))
            right = (b >> a % c) | (b << (sizeof(b) * 8 - a % c))
        c) Calculate sand for rotated values
            sand = abs((left ^ right + a * b) + c)
        d) Create three distant situations
            round[0] = sand % 16
            round[1] = (left + right) % 16
            round[2] = (left - right) % 16
        e) Addition of sand to the appropriate drawers
            (do this for j = 0 to < 3)
                1.e) Adding value to the respective drawers
                    // A: [0, 13, 2, 15]
                    if( (round[j] == 0)  || (round[j] == 13) 
                     || (round[j] == 2)  || (round[j] == 15) ){ A += sand >> 1 }
                    // B: [4, 9, 6, 11]
                    if( (round[j] == 4)  || (round[j] == 9)  
                     || (round[j] == 6)  || (round[j] == 11) ){ B += sand >> 1 }
                    // C: [8, 5, 10, 7]
                    if( (round[j] == 8)  || (round[j] == 5)
                     || (round[j] == 10) || (round[j] == 7)  ){ C += sand >> 1 }
                    // D: [12, 1, 14, 3]
                    if( (round[j] == 12) || (round[j] == 1)
                     || (round[j] == 14) || (round[j] == 3)  ){ D += sand >> 1 }
3) Final hash
    a) Calculate checksum for final values
        S = A + B + C + D
    b) Creating four small hashes with checksum
        h[0] = sprintf('%08x', A ^ S)
        h[1] = sprintf('%08x', B ^ S)
        h[2] = sprintf('%08x', C ^ S)
        h[3] = sprintf('%08x', D ^ S)
    c) Returns 128-bit (16-byte) final hash
        return h[0] . h[1] . h[2] . h[3]
```

### Examples

Language used in this example is PHP.

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
