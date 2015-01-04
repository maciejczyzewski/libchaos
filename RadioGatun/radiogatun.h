/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#ifndef _RADIOGATUN_H_
#define _RADIOGATUN_H_
#include <algorithm>
#include <string>
#include <vector>
#include "common.h"
#include "fastradiogatun.h"

using namespace std;

namespace radiogatun
{

// Only 8-bit, 16-bit, 32-bit and 64-bit versions can be instantiated
// using the template class below

template<class W>
class RadioGatun {
public:
    bool symmetric;
    static const unsigned int MillSize = 19;
    static const unsigned int BeltWidth = 3;
    static const unsigned int BeltLength = 13;
    static const unsigned int NumberOfBlankIterations = 16;
    vector<W> a, b;
public:
    RadioGatun(bool _symmetric = false);
        // High-level methods
    void Hash(const string& input, string& output);
    void HashNoPadding(const string& input, string& output);
        // Low-level methods
    void RoundFunction();
    void InverseRoundFunction();
    void InputFunction(const vector<W>& in);
    void OutputFunction(vector<W>& out);
    void FastIterate(const W* in, int Nr13Blocks);
protected:
    void Hash(const string& input, string& output, bool padding);
    inline unsigned int index(unsigned int i, unsigned int j)
    {
        return i*BeltWidth+j;
    }
};

typedef RadioGatun<UINT8>  RadioGatun8;
typedef RadioGatun<UINT16> RadioGatun16;
typedef RadioGatun<UINT32> RadioGatun32;
typedef RadioGatun<UINT64> RadioGatun64;

template<class W>
RadioGatun<W>::RadioGatun(bool _symmetric)
    : symmetric(_symmetric), a(MillSize), b(BeltLength*BeltWidth)
{
    for(unsigned int i=0; i<MillSize; i++)
        a[i] = 0;
    for(unsigned int i=0; i<BeltLength; i++)
        for(unsigned int j=0; j<BeltWidth; j++)
            b[index(i,j)] = 0;
}

template<class W>
void RadioGatun<W>::Hash(const string& input, string& output, bool padding)
{
    vector<W> in(BeltWidth);
    vector<W> out;
    int NrFast13Rounds = 0;
    // following lines optimized for speed on Little Endian platforms
    if (!symmetric && isLittleEndian() && ((sizeof(W) == 4) || (sizeof(W) == 8))) {
       NrFast13Rounds = input.size()/(sizeof(W)*BeltWidth*BeltLength);
       FastIterate((W*)input.data(), NrFast13Rounds);
    }
    unsigned int p = NrFast13Rounds*sizeof(W)*BeltWidth*BeltLength;
    // following processing without speed optimization
    while(p+sizeof(W)*BeltWidth <= input.size()) {
        for(unsigned int q=0; q<BeltWidth; q++) {
            in[q] = 0;
            for(unsigned int w=0; w<sizeof(W); w++)
                in[q] |= W((unsigned char)(input[p+q*sizeof(W)+w])) << (8*w);
        }
        p += sizeof(W)*BeltWidth;
        InputFunction(in);
        RoundFunction();
    }
    if ((p < input.size()) || padding) {
        string last_block = input.substr(p);
        if (padding)
            last_block += (char)0x01;
        while(last_block.size() < sizeof(W)*BeltWidth)
            last_block += (char)0x00;
        for(unsigned int q=0; q<BeltWidth; q++) {
            in[q] = 0;
            for(unsigned int w=0; w<sizeof(W); w++)
                in[q] |= W((unsigned char)(last_block[q*sizeof(W)+w])) << (8*w);
        }
        InputFunction(in);
        RoundFunction();
    }
    for(int i=0; i<NumberOfBlankIterations; i++)
        RoundFunction();
    output.clear();
    while(output.size() < 32) {
        RoundFunction();
        OutputFunction(out);
        for(unsigned int q=0; q<out.size(); q++)
            for(unsigned int w=0; w<sizeof(W); w++)
                output += char((out[q] >> (8*w)) & 0xFF);
    }
}


template<class W>
void RadioGatun<W>::Hash(const string& input, string& output)
{
    Hash(input, output, true);
}

template<class W>
void RadioGatun<W>::HashNoPadding(const string& input, string& output)
{
    Hash(input, output, false);
}

template<class W>
void RadioGatun<W>::RoundFunction()
{
    // save the last stage of the belt in q
    vector<W> q(BeltWidth);
    for(unsigned int j=0; j<BeltWidth; j++)
        q[j] = b[index(BeltLength-1,j)];

    // rotate the belt
    // first shift the belt
    for(unsigned int i=BeltLength-1; i>0; i--)
        for(unsigned int j=0; j<BeltWidth; j++)
            b[index(i,j)] = b[index(i-1,j)];
    // then make it a rotation
    for(unsigned int j=0; j<BeltWidth; j++)
        b[index(0,j)] = q[j];

    // mill to belt feedforward
    for(unsigned int i=0; i<12; i++)
        b[index(i+1,i%BeltWidth)] ^= a[i+1];

    // run the mill
    vector<W> A(MillSize);
    // gamma
    for(unsigned int i=0; i<MillSize; i++)
        A[i] = a[i]^(a[(i+1)%MillSize]|(~a[(i+2)%MillSize]));
    // pi
    for(unsigned int i=0; i<MillSize; i++)
        a[i] = ROR(A[(7*i)%MillSize], i*(i+1)/2);
    // theta
    for(unsigned int i=0; i<MillSize; i++)
        A[i] = a[i]^a[(i+1)%MillSize]^a[(i+4)%MillSize];
    // iota: XOR a constant into a (symmetric version isomorphic to RadioGatun[1])
    if (!symmetric) A[0] ^= 1; else A[0] = ~A[0];
    // just copy A to a
    for(unsigned int i=0; i<MillSize; i++) a[i] = A[i];

    // belt to mill feedforward
    for(unsigned int j=0; j<BeltWidth; j++)
        a[j+13] ^= q[j];
}

template<class W>
void RadioGatun<W>::InputFunction(const vector<W>& in)
{
    // Inject input
    if (in.size() == BeltWidth) {
    // into the mill
       for(unsigned int j=0; j<BeltWidth; j++) a[j+16] ^= in[j];
    // into the belt
       for(unsigned int j=0; j<BeltWidth; j++) b[index(0,j)] ^= in[j];
    }
}

// The InverseRoundFunction() function is never called, but it illustrates
// the inverse of the round function explicitly.
template<class W>
void RadioGatun<W>::InverseRoundFunction()
{
    // save the first stage of the belt in q
    vector<W> q(BeltWidth);
    for(unsigned int j=0; j<BeltWidth; j++)
        q[j] = b[index(0,j)];

    // belt to mill feedforward
    for(unsigned int j=0; j<BeltWidth; j++)
        a[j+13] ^= q[j];

    // run the inverse mill
    vector<W> A(MillSize);
    // iota
    if (!symmetric) a[0] ^= 1; else a[0] = ~a[0];
    // inverse of theta (Caution! code only valid for MillSize==19)
    for(unsigned int i=0; i<MillSize; i++)
        A[i] = a[(i+ 3)%MillSize]^
               a[(i+ 4)%MillSize]^
               a[(i+ 5)%MillSize]^
               a[(i+ 6)%MillSize]^
               a[(i+ 8)%MillSize]^
               a[(i+10)%MillSize]^
               a[(i+11)%MillSize]^
               a[(i+14)%MillSize]^
               a[(i+18)%MillSize];
    // inverse of pi
    for(unsigned int i=0; i<MillSize; i++)
        a[(7*i)%MillSize] = ROR(A[i],-i*(i+1)/2);
    // inverse of gamma
    for (unsigned int i=0 ; i<MillSize; i++) A[i] = ~a[i]; // complement input
    for (unsigned int i=0 ; i<MillSize; i++) a[i] = A[i]; // inverse of Chi: sowing the seeds
    for (unsigned int j=0 ; j<3*(MillSize-1)/2; j++) { // inverse of Chi: leaping 3*(MillSize-1)/2 times
        unsigned int i = (j*(MillSize-2))%MillSize;
        a[i] = A[i] ^ (a[(i+2)%MillSize]&(~A[(i+1)%MillSize]));
    }
    // mill to belt feedforward
    for(unsigned int i=0; i<BeltLength; i++)
        b[index(i+1,i%BeltWidth)] ^= a[i+1];

    // rotate the belt inversely
    // first shift the belt
    for(unsigned int i=0; i<BeltLength-1; i++)
        for(unsigned int j=0; j<BeltWidth; j++)
            b[index(i,j)] = b[index(i+1,j)];
    // then make it a rotation
    for(unsigned int j=0; j<BeltWidth; j++)
        b[index(BeltLength-1,j)] = q[j];
}

template<class W>
void RadioGatun<W>::OutputFunction(vector<W>& out)
{
    out.resize(2);
    for(unsigned int j=0; j<out.size(); j++)
        out[j] = a[j+1];
}

template<class W>
void RadioGatun<W>::FastIterate(const W* in, int Nr13Blocks)
{
    Assert(!symmetric, "This function does not work in the symmetric case.");
    Assert(isLittleEndian(), "This function works only on a little endian platform.");
    if (sizeof(W) == 4) {
        UINT32 local_a[MillSize], local_b[BeltWidth*BeltLength];

        for(unsigned int i=0; i<MillSize; i++)
            local_a[i] = a[i];
        for(unsigned int i=0; i<BeltWidth*BeltLength; i++)
            local_b[i] = b[i];

        RadioGatun32_FastIterate(local_a, local_b, (UINT32*)in, Nr13Blocks);

        for(unsigned int i=0; i<MillSize; i++)
            a[i] = local_a[i];
        for(unsigned int i=0; i<BeltWidth*BeltLength; i++)
            b[i] = local_b[i];
    }
    else if (sizeof(W) == 8) {
        UINT64 local_a[MillSize], local_b[BeltWidth*BeltLength];

        for(unsigned int i=0; i<MillSize; i++)
            local_a[i] = a[i];
        for(unsigned int i=0; i<BeltWidth*BeltLength; i++)
            local_b[i] = b[i];

        RadioGatun64_FastIterate(local_a, local_b, (UINT64*)in, Nr13Blocks);

        for(unsigned int i=0; i<MillSize; i++)
            a[i] = local_a[i];
        for(unsigned int i=0; i<BeltWidth*BeltLength; i++)
            b[i] = local_b[i];
    }
    else
        Assert(false, "No fast iterate for this word length");
}

}

#endif
