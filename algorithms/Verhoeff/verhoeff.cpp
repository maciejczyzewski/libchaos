#include <iostream>

const int d[10][10] =
{
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    { 1, 2, 3, 4, 0, 6, 7, 8, 9, 5 },
    { 2, 3, 4, 0, 1, 7, 8, 9, 5, 6 },
    { 3, 4, 0, 1, 2, 8, 9, 5, 6, 7 },
    { 4, 0, 1, 2, 3, 9, 5, 6, 7, 8 },
    { 5, 9, 8, 7, 6, 0, 4, 3, 2, 1 },
    { 6, 5, 9, 8, 7, 1, 0, 4, 3, 2 },
    { 7, 6, 5, 9, 8, 2, 1, 0, 4, 3 },
    { 8, 7, 6, 5, 9, 3, 2, 1, 0, 4 },
    { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }
};

const int  p[8][10] =
{
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    { 1, 5, 7, 6, 2, 8, 3, 0, 9, 4 },
    { 5, 8, 0, 3, 7, 9, 6, 1, 4, 2 },
    { 8, 9, 1, 6, 0, 4, 3, 5, 2, 7 },
    { 9, 4, 5, 3, 1, 2, 6, 8, 7, 0 },
    { 4, 2, 8, 6, 5, 7, 3, 9, 0, 1 },
    { 2, 7, 9, 3, 8, 0, 6, 4, 1, 5 },
    { 7, 0, 4, 6, 9, 1, 3, 2, 5, 8 }
};

const int j[10] = { 0, 4, 3, 2, 1, 5, 6, 7, 8, 9 };

bool isValidVerhoeff(const char* const str, int len)
{
    int c = 0;
    int i = 0;
    while (len--)
        c = d[ c ][ p[i++%8][str[len]-'0'] ];
    return c == 0;
};

int getVerhoeffDigit(const char* const str, int len)
{
    int c = 0;
    int i = 0;
    while (len--)
        c = d[ c ][ p[++i%8][str[len]-'0'] ]; // We start with 1 (++i), cause the check digit is the 0.
    return j[c];
};

// Example.
int main( int argc, const char* argv[] )
{
    using namespace std;

    int r;
    r = isValidVerhoeff("1428570", 7);
    cout << r << "\n"; // 1 = true
    r = getVerhoeffDigit("142857", 6);
    cout << r; // 0 => the checksum digit.
};