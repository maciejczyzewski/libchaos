/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#include "fastradiogatun.h"
#include "fastradiogatun.macros"

void RadioGatun32_FastIterate(UINT32 *a, UINT32 *b, const UINT32* in, unsigned int Nr13Blocks)
{
    UINT32 local_a[19], local_b[13*3];
    UINT32 A[19];
    unsigned int INptr;
    unsigned int i;

    for(i=0; i<19; i++)
        local_a[i] = a[i];
    for(i=0; i<3*13; i++)
        local_b[i] = b[i];

    INptr = 0;
    for (i=0 ; i<Nr13Blocks ; i++) {
        multiroundmacro(local_a, local_b, 32)
    }

    for(i=0; i<19; i++)
        a[i] = local_a[i];
    for(i=0; i<3*13; i++)
        b[i] = local_b[i];
}

void RadioGatun64_FastIterate(UINT64 *a, UINT64 *b, const UINT64* in, unsigned int Nr13Blocks)
{
    UINT64 local_a[19], local_b[13*3];
    UINT64 A[19];
    unsigned int INptr;
    unsigned int i;

    for(i=0; i<19; i++)
        local_a[i] = a[i];
    for(i=0; i<3*13; i++)
        local_b[i] = b[i];

    INptr = 0;
    for (i=0 ; i<Nr13Blocks ; i++) {
        multiroundmacro(local_a, local_b, 64)
    }

    for(i=0; i<19; i++)
        a[i] = local_a[i];
    for(i=0; i<3*13; i++)
        b[i] = local_b[i];
}
