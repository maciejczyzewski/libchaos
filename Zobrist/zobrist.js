// pieces * colors * fields * 64/32
var zobrist = new Uint32Array(13 * 2 * 64 * 2)

for (var i=0; i<zobrist.length; i++)
    zobrist[i] = Math.random() * 4294967296;

var table = new Uint32Array(3 * tablesize);

function hash(hi, lo, piece, color, field) {
    hi ^= zobrist[piece * 128 + color * 64 + field];
    lo ^= zobrist[piece * 128 + color * 64 + field + 1];

    var i = lo % tablesize;
    if (table[i] == hi && table[i+1] == lo) {
        // collision
    } else {
        table[i] = hi; table[i+1] = lo;
        // do what you want with table[i+2]
    }
}