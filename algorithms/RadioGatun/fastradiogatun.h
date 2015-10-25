/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#ifndef _FASTRADIOGATUN_H_
#define _FASTRADIOGATUN_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int UINT32;
typedef unsigned long long UINT64;

void RadioGatun32_FastIterate(UINT32 *a, UINT32 *b, const UINT32* in, unsigned int Nr13Blocks);
void RadioGatun64_FastIterate(UINT64 *a, UINT64 *b, const UINT64* in, unsigned int Nr13Blocks);

#ifdef __cplusplus
}
#endif

#endif
