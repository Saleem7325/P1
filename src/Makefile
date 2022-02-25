CC = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined
FILES = mymalloc.c mymalloc.h memgrind.c 

final:
	$(CC) $(CFLAGS) $(FILES) -o p1  	

clean:
	rm -rf *o *a *dylib *dSYM
