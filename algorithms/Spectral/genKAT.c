
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "shash.h"

#define	MAX_MARKER_LEN		50
#define	SUBMITTER_INFO_LEN	128

typedef enum { KAT_SUCCESS = 0, KAT_FILE_OPEN_ERROR = 1, KAT_HEADER_ERROR = 2, KAT_DATA_ERROR = 3, KAT_HASH_ERROR = 4 } STATUS_CODES;

STATUS_CODES	genShortMsg(int hashbitlen);
STATUS_CODES	genLongMsg(int hashbitlen);
STATUS_CODES	genExtremelyLongMsg(int hashbitlen);
STATUS_CODES	genMonteCarlo(int hashbitlen);
int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, BitSequence *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, BitSequence *A, int L);

STATUS_CODES
main()
{
	int		i, ret_val,  bitlens[4] = { 224, 256, 384, 512 };

	for ( i=0; i<4; i++ ) {
		if ( (ret_val = genShortMsg(bitlens[i])) != KAT_SUCCESS )
			return ret_val;
		if ( (ret_val = genLongMsg(bitlens[i])) != KAT_SUCCESS )
			return ret_val;
		if ( (ret_val = genExtremelyLongMsg(bitlens[i])) != KAT_SUCCESS )
			return ret_val;
		if ( (ret_val = genMonteCarlo(bitlens[i])) != KAT_SUCCESS )
			return ret_val;
	}
	
	return KAT_SUCCESS;
}

STATUS_CODES
genShortMsg(int hashbitlen)
{
	char		fn[32], line[SUBMITTER_INFO_LEN];
	int			msglen, msgbytelen, done;
	BitSequence	Msg[256], MD[64];
	FILE		*fp_in, *fp_out;
	
	if ( (fp_in = fopen("ShortMsgKAT.txt", "r")) == NULL ) {
		printf("Couldn't open <ShortMsgKAT.txt> for read\n");
		return KAT_FILE_OPEN_ERROR;
	}
	
	sprintf(fn, "ShortMsgKAT_%d.txt", hashbitlen);
	if ( (fp_out = fopen(fn, "w")) == NULL ) {
		printf("Couldn't open <%s> for write\n", fn);
		return KAT_FILE_OPEN_ERROR;
	}
	fprintf(fp_out, "# %s\n", fn);
	if ( FindMarker(fp_in, "# Algorithm Name:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Algorithm Name:%s\n", line);
	}
	else {
		printf("genShortMsg: Couldn't read Algorithm Name\n");
		return KAT_HEADER_ERROR;
	}
	if ( FindMarker(fp_in, "# Principal Submitter:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Principal Submitter:%s\n", line);
	}
	else {
		printf("genShortMsg: Couldn't read Principal Submitter\n");
		return KAT_HEADER_ERROR;
	}
	
	done = 0;
	do {
		if ( FindMarker(fp_in, "Len = ") )
			fscanf(fp_in, "%d", &msglen);
		else {
			done = 1;
			break;
		}
		msgbytelen = (msglen+7)/8;

		if ( !ReadHex(fp_in, Msg, msgbytelen, "Msg = ") ) {
			printf("ERROR: unable to read 'Msg' from <ShortMsgKAT.txt>\n");
			return KAT_DATA_ERROR;
		}
		Hash(hashbitlen, Msg, msglen, MD);
		fprintf(fp_out, "\nLen = %d\n", msglen);
		fprintBstr(fp_out, "Msg = ", Msg, msgbytelen);
		fprintBstr(fp_out, "MD = ", MD, hashbitlen/8);
	} while ( !done );
	printf("finished ShortMsgKAT for <%d>\n", hashbitlen);
	
	fclose(fp_in);
	fclose(fp_out);
	
	return KAT_SUCCESS;
}

STATUS_CODES
genLongMsg(int hashbitlen)
{
	char		fn[32], line[SUBMITTER_INFO_LEN];
	int			msglen, msgbytelen, done;
	BitSequence	Msg[4288], MD[64];
	FILE		*fp_in, *fp_out;
	
	if ( (fp_in = fopen("LongMsgKAT.txt", "r")) == NULL ) {
		printf("Couldn't open <LongMsgKAT.txt> for read\n");
		return KAT_FILE_OPEN_ERROR;
	}
	
	sprintf(fn, "LongMsgKAT_%d.txt", hashbitlen);
	if ( (fp_out = fopen(fn, "w")) == NULL ) {
		printf("Couldn't open <%s> for write\n", fn);
		return KAT_FILE_OPEN_ERROR;
	}
	fprintf(fp_out, "# %s\n", fn);
	if ( FindMarker(fp_in, "# Algorithm Name:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Algorithm Name:%s\n", line);
	}
	else {
		printf("genLongMsg: Couldn't read Algorithm Name\n");
		return KAT_HEADER_ERROR;
	}
	if ( FindMarker(fp_in, "# Principal Submitter:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Principal Submitter:%s\n\n", line);
	}
	else {
		printf("genLongMsg: Couldn't read Principal Submitter\n");
		return KAT_HEADER_ERROR;
	}
	
	done = 0;
	do {
		if ( FindMarker(fp_in, "Len = ") )
			fscanf(fp_in, "%d", &msglen);
		else
			break;
		msgbytelen = (msglen+7)/8;

		if ( !ReadHex(fp_in, Msg, msgbytelen, "Msg = ") ) {
			printf("ERROR: unable to read 'Msg' from <LongMsgKAT.txt>\n");
			return KAT_DATA_ERROR;
		}
		Hash(hashbitlen, Msg, msglen, MD);
		fprintf(fp_out, "Len = %d\n", msglen);
		fprintBstr(fp_out, "Msg = ", Msg, msgbytelen);
		fprintBstr(fp_out, "MD = ", MD, hashbitlen/8);
	} while ( !done );
	printf("finished LongMsgKAT for <%d>\n", hashbitlen);
	
	fclose(fp_in);
	fclose(fp_out);
	
	return KAT_SUCCESS;
}

STATUS_CODES
genExtremelyLongMsg(int hashbitlen)
{
	char		fn[32], line[SUBMITTER_INFO_LEN];
	BitSequence	Text[65], MD[64];
	int			i, repeat;
	FILE		*fp_in, *fp_out;
	hashState	state;
	HashReturn	retval;
	
	if ( (fp_in = fopen("ExtremelyLongMsgKAT.txt", "r")) == NULL ) {
		printf("Couldn't open <ExtremelyLongMsgKAT.txt> for read\n");
		return KAT_FILE_OPEN_ERROR;
	}
	
	sprintf(fn, "ExtremelyLongMsgKAT_%d.txt", hashbitlen);
	if ( (fp_out = fopen(fn, "w")) == NULL ) {
		printf("Couldn't open <%s> for write\n", fn);
		return KAT_FILE_OPEN_ERROR;
	}
	fprintf(fp_out, "# %s\n", fn);
	if ( FindMarker(fp_in, "# Algorithm Name:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Algorithm Name:%s\n", line);
	}
	else {
		printf("genExtremelyLongMsg: Couldn't read Algorithm Name\n");
		return KAT_HEADER_ERROR;
	}
	if ( FindMarker(fp_in, "# Principal Submitter:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Principal Submitter:%s\n\n", line);
	}
	else {
		printf("genExtremelyLongMsg: Couldn't read Principal Submitter\n");
		return KAT_HEADER_ERROR;
	}
	
	if ( FindMarker(fp_in, "Repeat = ") )
		fscanf(fp_in, "%d", &repeat);
	else {
		printf("ERROR: unable to read 'Repeat' from <ExtremelyLongMsgKAT.txt>\n");
		return KAT_DATA_ERROR;
	}
	
	if ( FindMarker(fp_in, "Text = ") )
		fscanf(fp_in, "%s", Text);
	else {
		printf("ERROR: unable to read 'Text' from <ExtremelyLongMsgKAT.txt>\n");
		return KAT_DATA_ERROR;
	}
	
//	memcpy(Text, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno", 64);
	
	if ( (retval = Init(&state, hashbitlen)) != KAT_SUCCESS ) {
		printf("Init returned <%d> in genExtremelyLongMsg\n", retval);
		return KAT_HASH_ERROR;
	}
	for ( i=0; i<repeat; i++ )
		if ( (retval = Update(&state, Text, 512)) != KAT_SUCCESS ) {
			printf("Update returned <%d> in genExtremelyLongMsg\n", retval);
			return KAT_HASH_ERROR;
		}
	if ( (retval = Final(&state, MD)) != KAT_SUCCESS ) {
		printf("Final returned <%d> in genExtremelyLongMsg\n", retval);
		return KAT_HASH_ERROR;
	}
	fprintf(fp_out, "Repeat = %d\n", repeat);
	fprintf(fp_out, "Text = %s\n", Text);
	fprintBstr(fp_out, "MD = ", MD, hashbitlen/8);
	printf("finished ExtremelyLongMsgKAT for <%d>\n", hashbitlen);
	
	fclose(fp_in);
	fclose(fp_out);
	
	return KAT_SUCCESS;
}

STATUS_CODES
genMonteCarlo(int hashbitlen)
{
	char		fn[32], line[SUBMITTER_INFO_LEN];
	BitSequence	Seed[128], Msg[128], MD[64], Temp[128];
	int			i, j, bytelen;
	FILE		*fp_in, *fp_out;
	
	if ( (fp_in = fopen("MonteCarlo.txt", "r")) == NULL ) {
		printf("Couldn't open <MonteCarlo.txt> for read\n");
		return KAT_FILE_OPEN_ERROR;
	}
	
	sprintf(fn, "MonteCarlo_%d.txt", hashbitlen);
	if ( (fp_out = fopen(fn, "w")) == NULL ) {
		printf("Couldn't open <%s> for write\n", fn);
		return KAT_FILE_OPEN_ERROR;
	}
	fprintf(fp_out, "# %s\n", fn);
	if ( FindMarker(fp_in, "# Algorithm Name:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Algorithm Name:%s\n", line);
	}
	else {
		printf("genMonteCarlo: Couldn't read Algorithm Name\n");
		return KAT_HEADER_ERROR;
	}
	if ( FindMarker(fp_in, "# Principal Submitter:") ) {
		fscanf(fp_in, "%[^\n]\n", line);
		fprintf(fp_out, "# Principal Submitter:%s\n\n", line);
	}
	else {
		printf("genMonteCarlo: Couldn't read Principal Submitter\n");
		return KAT_HEADER_ERROR;
	}
	
	if ( !ReadHex(fp_in, Seed, 128, "Seed = ") ) {
		printf("ERROR: unable to read 'Seed' from <MonteCarlo.txt>\n");
		return KAT_DATA_ERROR;
	}
	
	bytelen = hashbitlen / 8;
	memcpy(Msg, Seed, 128);
	fprintBstr(fp_out, "Seed = ", Seed, 128);
	for ( j=0; j<100; j++ ) {
		for ( i=0; i<1000; i++ ) {
			Hash(hashbitlen, Msg, 1024, MD);
			memcpy(Temp, Msg, 128-bytelen);
			memcpy(Msg, MD, bytelen);
			memcpy(Msg+bytelen, Temp, 128-bytelen);
		}
		fprintf(fp_out, "\nj = %d\n", j);
		fprintBstr(fp_out, "MD = ", MD, bytelen);
	}
	printf("finished MonteCarloKAT for <%d>\n", hashbitlen);

	fclose(fp_in);
	fclose(fp_out);
	
	return KAT_SUCCESS;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
	char	line[MAX_MARKER_LEN];
	int		i, len;

	len = (int)strlen(marker);
	if ( len > MAX_MARKER_LEN-1 )
		len = MAX_MARKER_LEN-1;

	for ( i=0; i<len; i++ )
		if ( (line[i] = fgetc(infile)) == EOF )
			return 0;
	line[len] = '\0';

	while ( 1 ) {
		if ( !strncmp(line, marker, len) )
			return 1;

		for ( i=0; i<len-1; i++ )
			line[i] = line[i+1];
		if ( (line[len-1] = fgetc(infile)) == EOF )
			return 0;
		line[len] = '\0';
	}

	// shouldn't get here
	return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, BitSequence *A, int Length, char *str)
{
	int			i, ch, started;
	BitSequence	ich;

	if ( Length == 0 ) {
		A[0] = 0x00;
		return 1;
	}
	memset(A, 0x00, Length);
	started = 0;
	if ( FindMarker(infile, str) )
		while ( (ch = fgetc(infile)) != EOF ) {
			if ( !isxdigit(ch) ) {
				if ( !started ) {
					if ( ch == '\n' )
						break;
					else
						continue;
				}
				else
					break;
			}
			started = 1;
			if ( (ch >= '0') && (ch <= '9') )
				ich = ch - '0';
			else if ( (ch >= 'A') && (ch <= 'F') )
				ich = ch - 'A' + 10;
			else if ( (ch >= 'a') && (ch <= 'f') )
				ich = ch - 'a' + 10;
			
			for ( i=0; i<Length-1; i++ )
				A[i] = (A[i] << 4) | (A[i+1] >> 4);
			A[Length-1] = (A[Length-1] << 4) | ich;
		}
	else
		return 0;

	return 1;
}

void
fprintBstr(FILE *fp, char *S, BitSequence *A, int L)
{
	int		i;

	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}