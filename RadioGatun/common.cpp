/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#include <algorithm>
#include "common.h"

#if defined(BINHEX_USES_DOT)
const char* htob_BE[] = {"....","...X","..X.","..XX",".X..",".X.X",".XX.",".XXX","X...","X..X","X.X.","X.XX","XX..","XX.X","XXX.","XXXX"};
const char* htob_LE[] = {"....","X...",".X..","XX..","..X.","X.X.",".XX.","XXX.","...X","X..X",".X.X","XX.X","..XX","X.XX",".XXX","XXXX"};
#else
const char* htob_BE[] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
const char* htob_LE[] = {"0000","1000","0100","1100","0010","1010","0110","1110","0001","1001","0101","1101","0011","1011","0111","1111"};
#endif

const char _hex[] = "0123456789ABCDEF";

//----------------------------------------------------------------------------------------------------------------------
int pause()
//----------------------------------------------------------------------------------------------------------------------
{
	char c;
	cout << "Press a key..." << endl; cin.get(c);
	return c;
}

void Assert(bool condition, const string& message)
{
     if(!condition) {
         cout << "ASSERTION FAILED: ";
         cout << message << endl;
         pause();
         abort();
     }
}

bool isLittleEndian()
{
    static UINT32 x = 0x04030201UL;
    static UINT64 y = 0x0807060504030201ULL;
    bool littleEndian = true;
    for(unsigned int i=0; i<4; i++)
        littleEndian = littleEndian && (int(((UINT8*)(&x))[i]) == (i+1));
    for(unsigned int i=0; i<8; i++)
        littleEndian = littleEndian && (int(((UINT8*)(&y))[i]) == (i+1));
    return littleEndian;
}
