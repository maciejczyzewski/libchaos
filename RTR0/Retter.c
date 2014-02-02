/******************************************************************************/
/*                                  RETTER                                    */
/******************************************************************************/
/*                                                                            */
/*    RTR0 (Retter engine) static class for C                                 */
/*    implemented and designed by Maciej Czyżewski                            */
/*    specification from http://maciejczyzewski.github.io/                    */
/*                                                                            */
/*    © Copyright 2014 Developer's Network. All rights reserved.              */
/*    This is licensed under the Lesser General Public License (LGPL)         */
/*    This library is free software; you can redistribute it and/or           */
/*    modify it under the terms of the GNU Lesser General Public              */
/*    License as published by the Free Software Foundation; either            */
/*    version 2.1 of the License, or (at your option) any later version.      */
/*                                                                            */
/*    This library is distributed in the hope that it will be useful,         */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        */
/*    Lesser General Public License for more details.                         */
/*                                                                            */
/*    You should have received a copy of the GNU Lesser General Public        */
/*    License along with this library; if not, write to the Free Software     */
/*    Foundation, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA       */
/*                                                                            */
/**** Version 0.0.1 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
 
/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n, b, i)                           \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n, b, i)                           \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define RIGHTROTATE(x, c) (((x) >> (c)) | ((x) << (32 - (c))))

/*
 * Implementation of RTR0 cryptographic hash function
 * Example: RTR0((uint8_t*)message, length, result);
 */ 
void RTR0(const uint8_t *initial_message, size_t initial_length, uint8_t *result)
{
    /* Declaration of variables */
    size_t length, offset, i;

    /* Declaration of message */
    uint8_t *message = NULL; 

    /* Declaration of algorithm values */
    uint32_t words[16];
    uint32_t left, right, sand;
    uint32_t A = 0xf7537e82, B = 0xbd3af235, C = 0x2ad7d2bb, D = 0xeb86d391, S;
  
    /* Calculate new length */
    for (length = initial_length + 1; length % (512 / 8) != 448 / 8; length++)
        ;
 
    /* Prepare message */
    message = (uint8_t*) malloc(length + 8);

    /* Copy block of memory */
    memcpy(message, initial_message, initial_length);

    /* Append "1" bit */
    message[initial_length] = 0x80;

    /* Append "0" bits */
    for (offset = initial_length + 1; offset < length; offset++)
        message[offset] = 0;
 
    /* Append the len in bits at the end of the buffer */
    PUT_ULONG_LE(initial_length * 8, message + length, 0);

    /* Initial_len >> 29 == initial_len * 8 >> 32, but avoids overflow */
    PUT_ULONG_LE(initial_length >> 29, message + length + 4, 0);
    
    /* Process the message in successive 512-bit chunks */
    for(offset = 0; offset < length; offset += (512 / 8))
    { 
        for (i = 0; i <= 16; i++)
        {
            /* Get little endian */
            GET_ULONG_LE(words[i], message + offset + i * 4, 0);
        }

        for (i = 1; i <= 16; i++)
        {
            /* Combining depending with neighbour value */
            words[i]    ^= words[i-1] << 1 | i;
            words[i-1]  ^= i;

            /* Calculate sand for rotated values */
            sand = (LEFTROTATE(words[i], words[i-1]) ^ RIGHTROTATE(words[i-1], words[i]));

            /* Addition sand to the appropriate drawers */
            A += sand;
            B += sand;
            C += sand;
            D += sand;
        }

        /* Calculate checksum for final values */
        S = A << B ^ C >> D;
        S += words[16];

        /* Addition checksum to the appropriate drawers */
        A = A ^ S;
        B = B ^ S;
        C = C ^ S;
        D = D ^ S;
    }
 
    /* Releasing memory */
    free(message);
 
    /* Returns 128-bit (16-byte) final hash in table */
    PUT_ULONG_LE(A, result, 0);
    PUT_ULONG_LE(B, result + 4, 0);
    PUT_ULONG_LE(C, result + 8, 0);
    PUT_ULONG_LE(D, result + 12, 0);
}

/******************************************************************************/

/*
 * This section is only for tests. Does not belong to implementations
 */
int main(int argc, char **argv)
{
    /* Declaration of variables */
    char *message = argv[1];
    uint8_t result[16];
    size_t length, i;

    /* Rules (only for CLI) */
    if (argc < 2) {
        printf("usage: %s 'string'\n", argv[0]);
        return 1;
    }

    /* Generating hash */
    length = strlen(message);
    RTR0((uint8_t*)message, length, result);

    /* Output: Hash */
    for (i = 0; i < 16; i++)
        printf("%2.2x", result[i]);
    puts("");

    /* Output: Table of hex */
    for(i = 0; i < 16; i++)
    {
        printf("0x%02x%c ", result[i], (i < 15) ? ',' : ' ');
        if((i & 3) == 3)
            printf("\n");
    }
    
    /**
     Input:
        The quick brown fox jumps over the lazy dog
     Output:
        894a269d7cd9cde2c6feaa5098ff1991
        0x89, 0x4a, 0x26, 0x9d, 
        0x7c, 0xd9, 0xcd, 0xe2, 
        0xc6, 0xfe, 0xaa, 0x50, 
        0x98, 0xff, 0x19, 0x91 
    **/

    /* End */
    return 0;
}
