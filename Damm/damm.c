char Lookup(char* number)
{
    const char table[]=
        "0317598642""7092154863""4206871359""1750983426""6123045978"
        "3674209581""5869720134""8945362017""9438617205""2581436790";
    char interim='0';
    char* p;
    for(p=number; *p!='\0'; ++p) {
        if((unsigned char)(*p-'0')>9)
            return '-'; //minus sign indicates an error: character is not a digit
        interim=table[(*p-'0')+(interim-'0')*10];
    }
    return interim;
}

char CalculateCheckDigit(char* numberWithoutCheckDigit)
{
    return Lookup(numberWithoutCheckDigit);
}

typedef int BOOL;
BOOL IsCheckDigitValid(char* numberWithCheckDigit)
{
    return Lookup(numberWithCheckDigit)=='0';
}