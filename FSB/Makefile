LFLAGS=
CFLAGS=-Wall -O3

all: fsb

fsb.o: fsb.c fsb.h pi.h Whirlpool.h nessie.h
	gcc $(CFLAGS) fsb.c -c

main.o: main.c fsb.h
	gcc $(CFLAGS) main.c -c

fsb: main.o fsb.o
	gcc $(LFLAGS) main.o fsb.o -o fsb

clean:
	rm fsb main.o fsb.o
