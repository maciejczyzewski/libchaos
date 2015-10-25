#On all machines with gcc (including Alpha)
CC = gcc -O3
#On 32-bit machines
#CC = cc -O4
#On Alpha:
#CC = cc -O -migrate

all : testtiger

testtiger: testtiger.o tiger.o sboxes.o
	$(CC) -o testtiger testtiger.o tiger.o sboxes.o

testtiger.o: testtiger.c
	$(CC) -c testtiger.c

tiger.o: tiger.c
	$(CC) -c tiger.c

sboxes.o: sboxes.c
	$(CC) -c sboxes.c
