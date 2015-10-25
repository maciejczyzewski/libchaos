
B224 = blake224
B256 = blake256
B384 = blake384
B512 = blake512
BINS = $(B224) $(B256) $(B384) $(B512)
CFLAGS = -Wall -O3 -fomit-frame-pointer

all:			$(BINS)
			@echo "Checking test vectors"	
			./$(B224)
			./$(B256)
			./$(B384)
			./$(B512)

clean:
			rm -f $(BINS) 




