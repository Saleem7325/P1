CC = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined

p1: mymalloc.h mymalloc.c bst.h bst.c memgrind.c
	$(CC) $(CFLAGS) -o $@ $^  	

clean:
	rm -rf *o *a *dylib *dSYM
