/* RadioGatún reference code
 * Public domain
 * For more information on RadioGatún, please refer to 
 * http://radiogatun.noekeon.org/
*/
#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <iostream>
#include <sstream>

#define BINHEX_USES_DOT

#define getbit(a,i) 		( ((a) >> (i)) & 1 )
#define setbit(bit,i) 		( ((bit) & 1) << (i) )

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

#if defined(_MSC_VER)
#define ROL32(x, y) _lrotl(x, y)
#define ROR32(x, y) _lrotr(x, y)
#else
#define ROL32(x, y) (((x) << (y)) | ((x) >> (32-(y))))
#define ROR32(x, y) (((x) >> (y)) | ((x) << (32-(y))))
#endif

#define ROL64(x, y) (((x) << (y)) | ((x) >> (64-(y))))
#define ROR64(x, y) (((x) >> (y)) | ((x) << (64-(y))))

#define ROL19(x, y) ((((x) << (y)) | (((x) & 0x7FFFF) >> (19-(y)))) & 0x7FFFF)
#define ROR19(x, y) (((((x) & 0x7FFFF) >> (y)) | ((x) << (19-(y)))) & 0x7FFFF)

template<class T>
inline T ROL(T x, int y)
{
    int y_mod = ((y % (sizeof(T)*8)) + (sizeof(T)*8)) % (sizeof(T)*8);
    return T(x << y_mod) | T(x >> (sizeof(T)*8-y_mod));
}

template<UINT32>
inline UINT32 ROL(UINT32 x, int y)
{
    int y_mod = ((y % 32) + 32) % 32;
    return ROL32(x, y_mod);
}

template<UINT64>
inline UINT64 ROL(UINT64 x, int y)
{
    int y_mod = ((y % 64) + 64) % 64;
    return ROL64(x, y);
}

template<class T>
inline T ROR(T x, int y)
{
    int y_mod = ((y % (sizeof(T)*8)) + (sizeof(T)*8)) % (sizeof(T)*8);
    return T(x >> y_mod) | T(x << (sizeof(T)*8-y_mod));
}

template<UINT32>
inline UINT32 ROR(UINT32 x, int y)
{
    int y_mod = ((y % 32) + 32) % 32;
    return ROR32(x, y_mod);
}

template<UINT64>
inline UINT64 ROR(UINT64 x, int y)
{
    int y_mod = ((y % 64) + 64) % 64;
    return ROR64(x, y);
}

using namespace std;

void Assert(bool condition, const string& message);
bool isLittleEndian();

extern const char _hex[];

//######################################################################################################################
// Helper Functions
//######################################################################################################################

int 						pause();
inline char 				hex(int nibble);
inline int 					htoi(char x);
template<class T> unsigned 	hammingweight(T i);
inline string 				xorstring(string s1, string s2, bool leftalign=true);

template<class T> T 		hextoint_BE(string h);
template<class T> T 		hextoint_LE(string h);
inline string 				hextostring_BE(const string& h);
inline string 				hextostring_LE(const string& h);
inline string 				hextonibblestring_LE(const string& h);
inline string 				hextonibblestring_BE(const string& h);

template<class T> string 	inttobin_BE(T i, unsigned int size=0);
template<class T> string 	inttobin_LE(T i, unsigned int size=0);
template<class T> string 	inttohex_BE(T i, unsigned int size=0);
template<class T> string 	inttohex_LE(T i, unsigned int size=0);

inline string 				stringtobin_BE(const string& s, unsigned int size=0);
inline string 				stringtobin_LE(const string& s, unsigned int size=0);
inline string 				stringtohex_BE(const string& s, unsigned int size=0);
inline string 				stringtohex_LE(const string& s, unsigned int size=0);
inline string 				stringtonibblestring_LE(const string& s);
inline string 				stringtonibblestring_BE(const string& s);

inline string 				nibblestringtobin_LE(const string& ns);
inline string 				nibblestringtobin_BE(const string& ns);
inline string 				nibblestringtohex_LE(const string& ns);
inline string 				nibblestringtohex_BE(const string& ns);
inline string 				nibblestringtostring_LE(const string& ns);			//! invertible only if x.length is even
inline string 				nibblestringtostring_BE(const string& ns);			//! invertible only if x.length is even


//######################################################################################################################
// Definitions
//######################################################################################################################

extern const char* htob_BE[];
extern const char* htob_LE[];

#if defined(BINHEX_USES_DOT)
	#define binhex_zero '.'
	#define binhex_one  'X'
#else
	#define binhex_zero '0'
	#define binhex_one  '1'
#endif

//----------------------------------------------------------------------------------------------------------------------
char hex(int nibble)
//----------------------------------------------------------------------------------------------------------------------
{
	return _hex[nibble];
}

//----------------------------------------------------------------------------------------------------------------------
int htoi(char x)
//----------------------------------------------------------------------------------------------------------------------
{
	int isletter = (x & 0x40) >> 6; return (isletter | (isletter << 3)) + ( x & 0x0F );
}

//----------------------------------------------------------------------------------------------------------------------
template<class T> unsigned hammingweight(T i)
//----------------------------------------------------------------------------------------------------------------------
{
	//                             0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
	static const unsigned _hw[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	unsigned hw=0;
	while( i > 0 )
	{
		hw+=_hw[i & 0x0F];
		i>>=4;
	}
	return hw;
}

//----------------------------------------------------------------------------------------------------------------------
template<> inline unsigned hammingweight(string s)
//----------------------------------------------------------------------------------------------------------------------
{
	unsigned hw=0;
	for( unsigned i=0; i<s.length(); ++i)
	    hw+=hammingweight(s[i]);
	return hw;
}

//----------------------------------------------------------------------------------------------------------------------
string xorstring(string s1, string s2, bool leftalign)
//----------------------------------------------------------------------------------------------------------------------
{
	//We want s1.length() <= s2.length()
	if(s1.length()>s2.length()) swap(s1,s2);

	string s;
	unsigned ofs=leftalign ? 0 : s2.length()-s1.length();
	for(unsigned i=0; i<s1.length(); ++i)
	{
		s+=s1[i]^s2[i+ofs];
	}

	if(leftalign)
	{
    	return s+string(s2,s1.length());
	}
	else
	{
    	return string(s2,0,s2.length()-s1.length())+s;
	}
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
T hextoint_BE(string h)
//----------------------------------------------------------------------------------------------------------------------
{
	T i=0;

	for(string::const_iterator it=h.begin(); it<h.end(); ++it)
	{
		i=(i<<4)|htoi(*it);
	}

	return i;
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
T hextoint_LE(string h)
//----------------------------------------------------------------------------------------------------------------------
{
	T i=0;

	for(string::const_iterator it=h.end()-1; it>=h.begin(); --it)
	{
		i=(i<<4)|htoi(*it);
	}

	return i;
}

//----------------------------------------------------------------------------------------------------------------------
string hextostring_BE(const string& h)
//----------------------------------------------------------------------------------------------------------------------
{
    string x;
	for(unsigned int i=0; (i+1)<h.length(); i+=2) {
		x += (htoi(h[i]) << 4)+ htoi(h[i+1]);
	}
	return x;
}

//----------------------------------------------------------------------------------------------------------------------
string hextostring_LE(const string& h)
//----------------------------------------------------------------------------------------------------------------------
{
    string x;
	for(unsigned int i=0; (i+1)<h.length(); i+=2) {
		x += (htoi(h[i]) << 4)+ htoi(h[i+1]);
	}
	return x;
}

//----------------------------------------------------------------------------------------------------------------------
string hextonibblestring_BE(const string& h)
//----------------------------------------------------------------------------------------------------------------------
{
	string ns;

	for(string::const_iterator it=h.begin();it<h.end();++it)
	{
		ns+=htoi(*it);
	}

	return ns;
}

//----------------------------------------------------------------------------------------------------------------------
string hextonibblestring_LE(const string& h)
//----------------------------------------------------------------------------------------------------------------------
{
	string ns;

	for(string::const_iterator it=h.end()-1;it>=h.begin();--it)
	{
		ns+=htoi(*it);
	}

	return ns;
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
string inttobin_BE(T i, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
    string sb;
	while( i > 0 )
	{
        sb = htob_BE[i&0xF] + sb;
		i>>=4;
	}

    if(!size)
        return sb;
    if(size<sb.length())
        return string(sb,sb.length()-size,size);
    else
        return string(size-sb.length(),binhex_zero)+sb;
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
string inttobin_LE(T i, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
    string sb;
    while( i > 0 )
    {
        sb += htob_LE[i&0xF];
        i>>=4;
    }

    if(!size)
        return sb;
    if(size<sb.length())
        return string(sb,0,size);
    else
        return sb+string(size-sb.length(),binhex_zero);
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
string inttohex_BE(T i, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
    string sh;
	while( i > 0 )
	{
        sh = hex(i&0xF) + sh;
		i>>=4;
	}

    if(!size)
        return sh;
    if(size<sh.length())
        return string(sh,sh.length()-size,size);
    else
        return string(size-sh.length(),'0')+sh;
}

//----------------------------------------------------------------------------------------------------------------------
template<class T>
string inttohex_LE(T i, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
    string sh;
	while( i > 0 )
	{
        sh += hex(i&0xF);
		i>>=4;
	}

    if(!size)
        return sh;
    if(size<sh.length())
        return string(sh,0,size);
    else
        return sh+string(size-sh.length(),'0');
}

//----------------------------------------------------------------------------------------------------------------------
string stringtobin_BE(const string& s, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
	string sb;
	for(string::const_iterator it=s.begin();it<s.end();++it)
	{
		sb += htob_BE[((UINT8)*it)>>4];
		sb += htob_BE[*it&0xF];
	}

    if(!size)
        return sb;
    if(size<sb.length())
        return string(sb,sb.length()-size,size);
    else
        return string(size-sb.length(),binhex_zero)+sb;
}

//----------------------------------------------------------------------------------------------------------------------
string stringtobin_LE(const string& s, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
	string sb;
	for(string::const_iterator it=s.end()-1;it>=s.begin();--it)
	{
		sb += htob_BE[*it&0xF];
		sb += htob_BE[((UINT8)*it)>>4];
	}
    if(!size)
        return sb;
    if(size<sb.length())
        return string(sb,0,size);
    else
        return sb+string(size-sb.length(),binhex_zero);
}

//----------------------------------------------------------------------------------------------------------------------
string stringtohex_BE(const string& s, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
	string h;
	for(string::const_iterator it=s.begin();it<s.end();++it)
	{
		h += hex((*it & 0xF0) >> 4);
		h += hex((*it & 0x0F) >> 0);
	}

    if(!size)
        return h;
    if(size<h.length())
        return string(h,h.length()-size,size);
    else
        return string(size-h.length(),'0')+h;
}

//----------------------------------------------------------------------------------------------------------------------
string stringtohex_LE(const string& s, unsigned int size)
//----------------------------------------------------------------------------------------------------------------------
{
	string h;
	for(string::const_iterator it=s.end()-1;it>=s.begin();--it)
	{
		h += hex((*it & 0x0F) >> 0);
		h += hex((*it & 0xF0) >> 4);
	}
    if(!size)
        return h;
    if(size<h.length())
        return string(h,0,size);
    else
        return h+string(size-h.length(),'0');
}

//----------------------------------------------------------------------------------------------------------------------
string stringtonibblestring_BE(const string& s)
//----------------------------------------------------------------------------------------------------------------------
{
	string ns;

	for(string::const_iterator it=s.begin();it<s.end();++it)
	{
		ns+=UINT8(*it)>>4;
		ns+=*it & 0x0F;
	}

	return ns;
}

//----------------------------------------------------------------------------------------------------------------------
string stringtonibblestring_LE(const string& s)
//----------------------------------------------------------------------------------------------------------------------
{
	string ns;

	for(string::const_iterator it=s.end()-1;it>=s.begin();--it)
	{
		ns+=*it & 0x0F;
		ns+=UINT8(*it)>>4;
	}

	return ns;
}
//----------------------------------------------------------------------------------------------------------------------
string nibblestringtobin_BE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string sb;
	for(string::const_iterator it=ns.begin();it<ns.end();++it)
	{
        sb += htob_BE[*it&0xF];
    }
    return sb;
}

//----------------------------------------------------------------------------------------------------------------------
string nibblestringtobin_LE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string sb;
	for(string::const_iterator it=ns.end()-1;it>=ns.begin();--it)
	{
        sb += htob_LE[*it&0xF];
	}

	return sb;
}

//----------------------------------------------------------------------------------------------------------------------
string nibblestringtohex_BE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string sh;
	for(string::const_iterator it=ns.begin();it<ns.end();++it)
	{
        sh += hex(*it&0xF);
	}

	return sh;
}

//----------------------------------------------------------------------------------------------------------------------
string nibblestringtohex_LE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string sh;
	for(string::const_iterator it=ns.end()-1;it>=ns.begin();--it)
	{
        sh += hex(*it&0xF);
	}

	return sh;
}

//----------------------------------------------------------------------------------------------------------------------
string nibblestringtostring_BE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string s;
	for(string::const_iterator it=ns.begin();it<ns.end();)
	{
		UINT8 c = *(it++) << 4;
		if(it<ns.end())
		{
			c |= *(it++);
		}
        s += c;
	}

	return s;
}

//----------------------------------------------------------------------------------------------------------------------
string nibblestringtostring_LE(const string& ns)
//----------------------------------------------------------------------------------------------------------------------
{
    string s;
	for(string::const_iterator it=ns.end()-1;it>=ns.begin();)
	{
		UINT8 c = *(it--);
		if(it>=ns.begin())
		{
			c |= *(it--) << 4;
		}
        s += c;
	}

	return s;
}


#endif
