#ifdef _MSC_VER
    typedef unsigned char __uint8_t;
    typedef unsigned long long __uint64_t;
#endif

typedef enum {SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;
typedef unsigned char BitSequence;
typedef __uint64_t DataLength;

typedef __uint8_t pcell;
typedef __uint8_t scell;

typedef struct s_prism {
    scell cell[4][4][8];
} s_prism;

typedef struct h_prism {
    scell cell[4][4][8];
} h_prism;

typedef struct p_prism {
    pcell cell[4][4][8];
} p_prism;

typedef struct hashState {
    int hashbitlen;
    s_prism sPrism;
    p_prism pPrism;
    h_prism hPrism;    
    __uint8_t remainder[64];
    int remainderbitlen;
    __uint64_t mesagelen;
    int started;
} hashState;

HashReturn Init(hashState *state, int hbitlen);

HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen);

HashReturn Final(hashState *state, BitSequence *hashval);

HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);
