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
// static node *head = (node *)mem;
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


//	char *last = (char *)(mem + MSIZE);
	//ptr =(char *)(ptr + sizeof(node) + bsize);

	unsigned long int ptrAddress = (unsigned long int)ptr;
	unsigned long int last = (unsigned long int)(mem + MSIZE);
	ptrAddress = (ptrAddress + sizeof(node) + bsize);

	if(ptrAddress >= last /*|| (void *)ptr > (void *)last*/ )
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
	if(outOfBounds(ptr, ptr->bsize))
		return;
	node *nnode = next(ptr, ptr->bsize);
	
	while(nnode->available){
		ptr->bsize = ptr->bsize + sizeof(node) + nnode->bsize;
		if(outOfBounds(ptr, ptr->bsize))
			return;
		nnode = next(nnode, nnode->bsize);
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

	if (sizeof(node) + 4 < oldSize-(size + sizeof(node))){

		char *nna = (char *)ptr;
		nna = nna + sizeof(node) + size;
		node *newNode = (node *)nna;	
		newNode->available = 1;
		newNode->bsize = oldSize-(size + sizeof(node));
		coalesce(newNode);
	}
}

void *findAlignedAddress(void *ptr){
	unsigned long int p = (unsigned long int)ptr;
	unsigned long int i = 1;

	while((p % 4) != 0)
		p = p + i;

	return (void *)p;
}

void init(){
	char *p = mem;
	unsigned long int pv = (unsigned long int)p;	

	if((pv % 4) != 0)
		 p = (char *)findAlignedAddress(p);
	
	head = (node *)p;
	head->available = 1;

	pv = (unsigned long int)head;
	unsigned long int m = (unsigned long int)mem;

	head->bsize =  ( 4096 - ( sizeof(node *) + (pv -  m) ) );
}

int findSize4(int size){
	int p = size;
	int i = 1;

	while((p % 4) != 0)
		p = p + i;

	return p;
}

void *mymalloc(size_t size, char *file, int line){

	if(firstCall){
		firstCall = 0;
		init();
	//	head->available = 1;
	//	head->bsize = MSIZE - sizeof(node);		
	}

	if((size % 4) != 0)
		size = findSize4(size);

	node *ptr = head;

	while(1){

		//Find free node
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

void myfree(void *ptr, char *file, int line){	
	if(firstCall){
		printf("Error: calling free() with an address not obtained from malloc() in %s, at line %d.\n", __FILE__, __LINE__ );
		return;
	}
	
	char *p = (char *)ptr;
	unsigned long int pv= (unsigned long int)p; 
	unsigned long int first = (unsigned long int)mem;
	unsigned long int last = (unsigned long int)(mem + MSIZE);

	if(pv < first || pv >= last){
		printf("Error: calling free() with an address not obtained from malloc() in %s, at line %d.\n", __FILE__, __LINE__ );
		return;
	}
	
	node *curr = head;
	unsigned long int cv = (unsigned long int)curr;

	while(cv <= pv){
		char *test = (char *)(cv + sizeof(node));
		
		if(test == p){
				if(curr->available)
					printf("Error: attempting to free a previously freed pointer in %s, at line %d.\n", __FILE__, __LINE__); 					
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

	printf("Error: attempting to free address not at the start of the chunk in %s, at line %d.\n", __FILE__, __LINE__);
}

