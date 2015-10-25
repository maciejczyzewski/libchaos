/*
* bbs.c
*
* Copyright 2000,2014 Bennett Samowich <bennett@foolean.org>
*
* bbs is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* bbs is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with bbs. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <openssl/bio.h>
#include <openssl/bn.h>

#define SIZE    512
#define COUNT   2048

#define FLAG_RAW    0x0
#define FLAG_HEX    0x1
#define FLAG_BIN    0x2
#define FLAG_ASC    0x4

/* function prototypes */
void usage();
static void cb(int, int, void*);

/* globals */
char *pname;
int  verbose;
int  debug;

/* structure for command-line options */
struct option opts[] = {
    { "ascii",      0,  0,  'a' },
    { "binary",     0,  0,  'b' },
    { "count",      1,  0,  'c' },
    { "help",       0,  0,  'h' },
    { "raw",        0,  0,  'r' },
    { "size",       1,  0,  's' },
    { "verbose",    0,  0,  'v' },
    { "hex",        0,  0,  'x' },
    { 0 }
};

/*
 * Implement the Blum, Blum, and Shub algorithm to generate a
 * random bit stream.
 *
 * From APPLIED CRYPTOGRAPHY 2nd Ed., Bruce Schneir (1996):
 *
 *  Find two large prime numbers, (p) and (q), which are congruent to
 *  3 modulo 4.  The product of these numbers, (n), is a blum integer.
 *  Choose another random integer, (x), which is relatively prime to
 *  (n).  Compute x[0] = (x^2) % n.  x[0] is the seed for the generator.
 *  now we can start computing bits.  The (i)th pseudo-random bit is the
 *  least significant bit of x[i] = (x[i-1] ^ 2) % n.
 *
 *  The security of this scheme rests on the difficulty of factoring
 *  (n).  You can make (n) public so that anyone can generate bits.
 *  However, unless a cryptanalyst can factor (n), he can never predict
 *  the output of the generator - not even with a statement like:
 *  "the next bit has a 51 percent change of being a 1".
 */
int
main(argc, argv)
int  argc;
char *argv[];
{
    BIGNUM p, q, n, x1, r, x0;
    BN_CTX *ctx;
    unsigned char val;
    int c, bit, byte, count, size;
    char flag;

    /* set the defaults */
    pname   = argv[0];
    flag    = FLAG_RAW;
    size    = SIZE;
    count   = 0;
    debug   = 0;
    verbose = 0;

    while ((c = getopt_long(argc, argv, "abc:Dhrs:vx", opts, NULL)) != -1) {
        switch (c) {
            case 'a':   /* set ASCII output format */
                flag = FLAG_ASC;
                break;
            case 'b':   /* set BINARY output format */
                flag = FLAG_BIN;
                break;
            case 'c':   /* set the number of bytes to output */
                count = atoi(optarg);
                break;
            case 'D':   /* turn on debugging */
                debug = 1;
                break;
            case 'r':   /* set RAW output format */
                flag = FLAG_RAW;
                break;
            case 's':   /* set bit size to use */
                size = atoi(optarg);
                if (size == 0) {
                    size = SIZE;
                }
                break;
            case 'v':   /* turn on verbosity */
                verbose = 1;
                break;
            case 'x':   /* set HEX output format */
                flag = FLAG_HEX;
                break;
            case 'h':   /* help! */
                usage();
                exit(1);    /* should never get here */
                break;      /* ... nor here          */
            default:
                usage();
                exit(1);    /* should never get here */
        }   /* end switch */
    }   /* end while */

    if (debug) {
        (void)fprintf(stderr, "debug: flag = %i\n", flag);
    }

    ctx = BN_CTX_new();
    BN_init(&n);
    BN_init(&p);
    BN_init(&q);
    BN_init(&r);
    BN_init(&x0);
    BN_init(&x1);

    /*
     * find two large prime numbers, (p) and (q), whose
     * product, (n), is a blum integer.
     */
    BN_one(&n);
    if (verbose) {
        (void)fprintf(stderr, "Generating first prime number: ");
    }
    BN_generate_prime(&p, size, 0, NULL, NULL, cb, NULL);
    if (verbose) {
        (void)fprintf(stderr, "\nGenerating second prime number: ");
    }
    while ( BN_mod_word(&n, 4) != 3) {
        if (verbose) {
            (void)fprintf(stderr, "|");
        }
        BN_generate_prime(&q, size, 0, NULL, NULL, cb, NULL);
        BN_mul(&n, &p, &q, ctx);
    }
    BN_clear_free(&p);
    BN_clear_free(&q);
    if (verbose) {
        (void)fprintf(stderr, "\n");
    }

    /*
     * find a random larger integer, (x), that is
     * relatively prime to (n).
     */
    BN_zero(&r);
    while (! BN_is_one(&r)) {
        BN_rand(&x1, size, 0, 0);
        BN_gcd(&r, &n, &x1, ctx);
    }
    BN_clear_free(&r);

    /*
     * compute x[0] = (x ^ 2) mod n
     */
    BN_mod_mul(&x0, &x1, &x1, &n, ctx);

    /*
     * begin computing bits... the (i)th bit is
     * least-signficant-bit of x[i] = (x[i-1] ^ 2) mod n
     */
    bit  = 0;
    byte = 0;

    for ( ; ; ) {
        BN_mod_mul(&x1, &x0, &x0, &n, ctx);

        if (bit == 0) {
            val = 0;
        }

        if ( BN_is_bit_set(&x1, 0)) {
            val |= (1 << bit);
        }

        bit++;
        if (bit == 8) {
            if (debug) {
                (void)printf("debug: val = %i (0x%x)\n", val, val);
            }

            switch (flag) {
                case FLAG_RAW:
                    (void)printf ("%c", val);
                    break;
                case FLAG_HEX:
                    (void)printf ("%02x", val);
                    break;
                case FLAG_ASC:
                    (void)printf ("%03i", val);
                    break;
                case FLAG_BIN:
                    (void)printf ("%d", ((val & 0x80) >> 7));
                    (void)printf ("%d", ((val & 0x40) >> 6));
                    (void)printf ("%d", ((val & 0x20) >> 5));
                    (void)printf ("%d", ((val & 0x10) >> 4));
                    (void)printf ("%d", ((val & 0x08) >> 3));
                    (void)printf ("%d", ((val & 0x04) >> 2));
                    (void)printf ("%d", ((val & 0x02) >> 1));
                    (void)printf ("%d",  (val & 0x01));
                    break;
            }

            if (debug) {
                (void)printf("\n");
            }

            bit = 0;
            if (count) {
                if (++byte >= count) {
                    break;
                }
            }
        }

        BN_copy(&x0, &x1);
    }

    fflush(stdout);
    return(0);
}

void
usage()
{
    fprintf (stderr, "%s [OPTION]...\n\n", pname);
    fprintf (stderr, "Options:\n");
    fprintf (stderr, "-a, --ascii     Write output as 3-digit ASCII numbers (e.g. 029)\n");
    fprintf (stderr, "-b, --binary    Write output as stream of 1's and 0's\n");
    fprintf (stderr, "-c, --count=NUM Number of bytes to output\n");
    fprintf (stderr, "-h, --help      Print this message and exit\n");
    fprintf (stderr, "-r, --raw       Write output as RAW data (default)\n");
    fprintf (stderr, "-s, --size=SIZE Set the bit-size of the numerical fields (default: %i)\n", SIZE);
    fprintf (stderr, "-v, --verbose   Print progress messages\n");
    fprintf (stderr, "-x, --hex       Write output as 2-character hexadecimal (e.g. FE)\n");
    exit(1);
}

/*
 * callback routine for displaying progress
 * during prime number generation
 */
static void
cb(p, n, arg)
int  p;
int  n;
void *arg;
{
    char c = '*';

    if (! verbose) {
        return;
    }

    if (p == 0) { c = '.';  }
    if (p == 1) { c = '+';  }
    if (p == 2) { c = '*';  }
    if (p == 3) { c = '\n'; }

    (void)fprintf(stderr, "%c", c);
    (void)fflush(stderr);
}
