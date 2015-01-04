CC=gcc
CFLAGS=-Wall 
SRC=siphash24.c main.c
BIN=siphash24_test siphash24_debug
BIN_DOUBLE=siphash24_test_double siphash24_debug_double

all:		        $(BIN)

siphash24_test:	        $(SRC)
		        $(CC) $(CFLAGS) $^ -o $@

siphash24_debug:        $(SRC) 
		        $(CC) $(CFLAGS) $^ -o $@ -DDEBUG

double:		        $(BIN_DOUBLE)

siphash24_test_double:  $(SRC) 
		        $(CC) $(CFLAGS) $^ -o $@ -DDOUBLE

siphash24_debug_double: $(SRC) 
		        $(CC) $(CFLAGS) $^ -o $@ -DDOUBLE -DDEBUG

clean:	      
		        rm -f *.o $(BIN) $(BIN_DOUBLE)

dist:		        clean
		        cd ..; \
		        tar zcf SipHash-`date +%Y%m%d%H%M`.tgz SipHash/*

