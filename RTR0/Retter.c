/******************************************************************************/
/*                                  RETTER                                    */
/******************************************************************************/
/*                                                                            */
/*    RTR0 (Retter engine) static class for C                                 */
/*    implemented and designed by Maciej A. Czyzewski                         */
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
 * 64-bit integer manipulation macros (little endian)
 */
#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n, b, i)                           \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] << 8  )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif
 
#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n, b, i)                           \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 8  );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif
 
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (64 - (c))))
#define RIGHTROTATE(x, c) (((x) >> (c)) | ((x) << (64 - (c))))
 
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
    uint64_t words[16];
    uint64_t left, right, sand;
    uint64_t A = 0xf7537e82, B = 0xbd3af235, C = 0x2ad7d2bb, D = 0xeb86d391, E = 0xd76aa478, S;
 
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
        for (i = 0; i < 16; i++)
        {
            /* Get little endian */
            GET_ULONG_LE(words[i], message + offset + i * 4, 0);
        }
 
        for (i = 1; i < 16; i++)
        {
            /* Combining depending with neighbour value */
            words[i] ^= words[i - 1] << 1 | i;
    
            /* Calculate sand for rotated values */
            sand = LEFTROTATE(words[i], words[i - 1]) ^ RIGHTROTATE(words[i - 1], words[i]);
 
            /* Addition sand to the appropriate drawers */
            if( i % 4 == 0 )
                A += sand;
            if( i % 4 == 1 )
                B += sand;
            if( i % 4 == 2 )
                C += sand;
            if( i % 4 == 3 )
                D += sand;
 
            E += sand;
        }
 
        /* Calculate checksum for final values */
        S = A << B ^ C >> D;
        S += words[15] ^ E;
 
        /* Addition checksum to the appropriate drawers */
        A = A ^ S;
        B = B ^ S;
        C = C ^ S;
        D = D ^ S;
        E = E ^ S;
    }

    /* Releasing memory */
    free(message);
 
    /* Returns 160-bit (20-byte) final hash in table */
    PUT_ULONG_LE(A, result, 0);
    PUT_ULONG_LE(B, result + 4, 0);
    PUT_ULONG_LE(C, result + 8, 0);
    PUT_ULONG_LE(D, result + 12, 0);
    PUT_ULONG_LE(E, result + 16, 0);
}
