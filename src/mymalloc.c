#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mymalloc.h"

typedef struct metadata{
	int bsize;
	int available;	
}node;

typedef struct node{
	int data;
	int key;
	struct node *next;
};

static char mem[4096];

void *mymalloc(size_t size, char *file, int line){
	FILE *fp;
	fp = fopen(*file, "r+");

}

void free(void *ptr, char *file, int line){
	FILE *fp;
	char f = *file;
	fp = fopen(f, "r+");
	scanf("%s", fp);

}
