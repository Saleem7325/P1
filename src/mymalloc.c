#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mymalloc.h"

typedef struct metadata{
	int bsize;
	int available;	
}node;

static char mem[4096];

void *mymalloc(size_t size, char *file, int line){

}

void free(void *ptr, char *file, int line){

}
