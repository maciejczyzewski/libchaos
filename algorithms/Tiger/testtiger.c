#include <time.h>

#ifdef i386
#define ITERATIONS 30
#else
#define ITERATIONS 500
#endif

typedef unsigned long long int word64;
typedef unsigned long word32;
typedef unsigned char byte;

void tiger(byte*, word64, word64*);

main()
{
  byte buffer[65536];
  long t1;
  long t2;
  double rate;
  int i;

  word64 res[3];

#define hash(str) tiger((byte*)str, strlen(str), res); \
  printf("Hash of \"%s\":\n\t%08X%08X %08X%08X %08X%08X\n", \
	 str, \
	 (word32)(res[0]>>32), \
	 (word32)(res[0]), \
	 (word32)(res[1]>>32), \
	 (word32)(res[1]), \
	 (word32)(res[2]>>32), \
	 (word32)(res[2]) );

  /* Hash of short strings */
  hash("");
  hash("abc");
  hash("Tiger");
  /* Hash of 512-bit strings */
  hash("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-");
  hash("ABCDEFGHIJKLMNOPQRSTUVWXYZ=abcdefghijklmnopqrstuvwxyz+0123456789");
  hash("Tiger - A Fast New Hash Function, by Ross Anderson and Eli Biham");
  /* Hash of two-block strings strings */
  hash("Tiger - A Fast New Hash Function, by Ross Anderson and Eli Biham, proceedings of Fast Software Encryption 3, Cambridge.");
  hash("Tiger - A Fast New Hash Function, by Ross Anderson and Eli Biham, proceedings of Fast Software Encryption 3, Cambridge, 1996.");
  hash("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-");

  /* Hash of a 64K byte string */
  for (i=0;i<65536;i++)
    buffer[i] = i&0xFF;

  tiger(buffer, 65536, res);
  printf("Hash of a 64K-byte string:\n\t%08X%08X %08X%08X %08X%08X\n",
	 (word32)(res[0]>>32),
	 (word32)(res[0]),
	 (word32)(res[1]>>32),
	 (word32)(res[1]),
	 (word32)(res[2]>>32),
	 (word32)(res[2]) );

  t1 = clock();
  for (i=0;i<ITERATIONS;i++)
    {
      tiger(buffer, 65536, res);
    }
  t2 = clock();

  rate = (double)CLOCKS_PER_SEC*(double)ITERATIONS*65556.0*8.0/
         ((double)(t2 - t1));
  printf("rate = %lf bit/s\n", rate);
}
