#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MSIZE 4096

//This is a node in a linked list that we keep between each block in mem
//bsize tells us the number of bytes this block contains
//available is either 0 if block is in use  or 1 if block is not in use 
typedef struct metadata{
	int bsize;	
	int available;	
}node;

static char mem[MSIZE];
static node *head = (node *)mem;

//This is intially set to 1 to indicate to mymalloc 
//if the client has  called mymalloc before.
//After the first call to mymalloc this get changed to 0.
static int firstCall = 1;

//Returns the next node in the linked list
//Does not check if next node is within bounds
//typically used after testing with if next node 
//is within the array with outOfBounds() call
node *next(void *ptr, int bsize){
	ptr = (char *)ptr;
	ptr = ptr + sizeof(node) + bsize;
	ptr = (node *)ptr;
	return ptr;
}

//Checks whether a ptr address + a block size is within the array mem
//This accounts for the size of our metadata
int outOfBounds(void *ptr, int bsize){
	ptr = (char *)ptr;

	if(ptr + sizeof(node) + bsize >= mem + MSIZE)
		return 1;
	return 0;	
}


//Checks wheter ptr is last block in mem if so it returns.
//Otherwise it adds the block size of all the contiguous free blocks adjacent to ptr in the linked list
//to ptr->bsize so now there is only one large free block instead of many contiguous free blocks.
//coalesce() should only be called with a node that is available.  
void coalesce(node *ptr){
	if(outOfBounds(ptr, ptr->bsize))
		return;
	node *nnode = next(ptr, ptr->bsize);
	
	while(nnode->available){
		ptr->bsize = ptr->bsize + sizeof(node) + nnode->bsize;
		if(outOfBounds(ptr, ptr->bsize))
			return;
		nnode = next(ptr, ptr->bsize);
	}
	return;
}

//If we have a free block that is large enough to store the requested number of bytes by the client
//we change that blocks size to the size they request and return a pointer to the block.
//This function takes a ptr to the block that is now in use by the client
//and creates a new node in the linked list at the end of the block that is in use
//to indicate that we have room in memory for the next call to mymalloc()
void createNode(node *ptr, int oldSize){
	int size = ptr->bsize;
	char *nna = (char *)ptr;
	nna = nna + sizeof(node) + size;
	node *newNode = (node *)nna;	
	newNode->available = 1;
	newNode->bsize = oldSize-(size + sizeof(node));
	coalesce(newNode);
}

void *mymalloc(size_t size, char *file, int line){
	if(firstCall){
		firstCall = 0;
		head->available = 1;
		head->bsize = MSIZE - sizeof(node);		
	}

	node *ptr = head;
	while(1){
		while(!ptr->available){
			if(outOfBounds(ptr, ptr->bsize))
				return NULL;

			ptr = next(ptr, size);	
		}
		coalesce(ptr);
		if(ptr->bsize < size && (!outOfBounds(ptr, ptr->bsize)) )
			ptr = next(ptr, ptr->bsize);
		else if (ptr->bsize > size){
			int oldSize = ptr->bsize;
			ptr->bsize = size;	
			ptr->available = 0;
			createNode(ptr, oldSize);
			void *r = (char *)ptr + sizeof(node);
			return r;
		else if(ptr->bsize == size){
			ptr->available = 0;
			void *r = (char *)ptr + sizeof(node);
			return r;	
		}else
			return NULL;		
	}	

}

void myfree(void *ptr, char *file, int line){

}

int main(int argc, char **argv){
	printf("Size of node: %ld\n", sizeof(node));
}
