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
static node *head;

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

	unsigned long int ptrAddress = (unsigned long int)ptr;
	unsigned long int last = (unsigned long int)(mem + MSIZE);
	ptrAddress = (ptrAddress + sizeof(node) + bsize);

	if(ptrAddress >= last)
		return 1;
	return 0;	
}

//Prints to stdout if blocks are available/not avaialable and block size inorder.
void printMem(){
	if(firstCall)
		return;

	int size = 1;
	node *ptr = head;
	printf("\nNode: %d\nAvailable: %d\nBlock Size: %d\n", size, ptr->available, ptr->bsize);
		
	while(!outOfBounds(ptr, ptr->bsize)){
		ptr = next(ptr, ptr->bsize);
		size++;
		printf("\nNode: %d\nAvailable: %d\nBlock Size: %d\n", size, ptr->available, ptr->bsize);
	}	
}

//Checks wheter ptr is last block in mem if so it returns.
//Otherwise it adds the block size of all the contiguous free blocks adjacent to ptr in the linked list
//to ptr->bsize so now there is only one large free block instead of many contiguous free blocks.
//coalesce() should only be called with a node that is available.  
void coalesce(node *ptr){
	ptr = (node *)ptr;

	if(outOfBounds(ptr, ptr->bsize))
		return;
	node *nextNode = next(ptr, ptr->bsize);
	
	while(nextNode->available){
		ptr->bsize = ptr->bsize + sizeof(node) + nextNode->bsize;
		if(outOfBounds(ptr, ptr->bsize))
			return;
		nextNode = next(ptr, ptr->bsize);
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

	if (sizeof(node) + 8 < oldSize-(size + sizeof(node))){
		char *newNodeAddress = (char *)ptr;
		newNodeAddress = newNodeAddress + sizeof(node) + size;
		node *newNode = (node *)newNodeAddress;	
		newNode->available = 1;
		newNode->bsize = oldSize-(size + sizeof(node));
		coalesce(newNode);
	}else
		ptr->bsize = oldSize;
	
}

//Since the ilab require structs of 8 bytes to be in an address divisible by 8, this 
//method takes a pointer to an address and finds an address greater than the address given
//the given pointers address that is also divisble by 8. If the address given is already divisible by 8
//it returns that address.
void *findAlignedAddress(void *ptr){
	unsigned long int p = (unsigned long int)ptr;
	unsigned long int i = 1;

	while((p % 8) != 0)
		p = p + i;

	return (void *)p;
}

//This method initializes head to point to the first aligned address in mem and
//calculates the block size and stores it in head, and sets available to 1;
void init(){
	char *p = mem;
	unsigned long int pv = (unsigned long int)p;	

	if((pv % 8) != 0)
		 p = (char *)findAlignedAddress(p);
	
	head = (node *)p;
	head->available = 1;

	pv = (unsigned long int)head;
	unsigned long int m = (unsigned long int)mem;

	head->bsize =  ( 4096 - ( sizeof(node *) + (pv -  m) ) );
}

//Since all of our addresses need to be divisisble by 8, I extended this property to all blocks
//in mem so that we dont have free pieces of memory between blocks and nodes due to node addresses 
//needing to be divisible by 8
int findSize8(int size){
	int p = size;
	int i = 1;

	while((p % 8) != 0)
		p = p + i;

	return p;
}

//mymalloc takes a requested size of bytes to store in mem, along with the file name and line number
//of the call to mymalloc. If the number of bytes requested is available in mem, mymalloc returns a void *
//to a block in mem of the size requested. If the number of bytes is not available mymalloc returns NULL.
void *mymalloc(size_t size, char *file, int line){
	if(firstCall){
		firstCall = 0;
		init();
	}

	if((size % 8) != 0)
		size = findSize8(size);

	node *ptr = head;

	while(1){

		while(!ptr->available){
			if(outOfBounds(ptr, ptr->bsize))
				return NULL;

			ptr = next(ptr, ptr->bsize);	
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

		}else if(ptr->bsize == size){

			ptr->available = 0;
			void *r = (char *)ptr + sizeof(node);
			return r;	

		}else
			return NULL;		
	}	

}

//myfree takes a pointer, the file name, and line number of the call. If the pointer is NULL
//myfree prints an error message reporting a NULL pointer, if the pointer is not within the bounds
//of mem, myfree prints an error mesage reporting the pointer is not from mymalloc, if the pointer
//is not at the beginnning of a block but within the bounds of mem, myfree prints an error reporting
//that the pointer is not at the beginning of a block, if the pointer contains an address of a free 
//block myfree prints an error message reporting a myfree call on a free pointer. If none of these
// happens to be the case myfree free the block pointed to by ptr and colesces it.
void myfree(void *ptr, char *file, int line){	
	if(ptr == NULL){
		printf("Error: calling free() with a NULL pointer in %s, at line %d.\n", file, line);
		return;
	}

	if(firstCall){
		printf("Error: calling free() with an address not obtained from malloc() in %s, at line %d.\n", file, line);
		return;
	}
	
	char *p = (char *)ptr;
	unsigned long int pv= (unsigned long int)p; 
	unsigned long int first = (unsigned long int)mem;
	unsigned long int last = (unsigned long int)(mem + MSIZE);

	if(pv < first || pv >= last){
		printf("Error: calling free() with an address not obtained from malloc() in %s, at line %d.\n", file, line );
		return;
	}
	
	node *curr = head;
	unsigned long int cv = (unsigned long int)curr;

	while(cv <= pv){
		char *test = (char *)(cv + sizeof(node));
		
		if(test == p){
				if(curr->available)
					printf("Error: attempting to free a previously freed pointer in %s, at line %d.\n", file, line); 					
				else
					curr->available = 1;
				coalesce(curr);
				return;
		}
		
		if(!outOfBounds(curr, curr->bsize)){
			curr = (node *)next(curr, curr->bsize);
			cv = (unsigned long int)curr;
		}else
			break;
	}

	printf("Error: attempting to free address not at the start of the chunk in %s, at line %d.\n", file, line);
}

