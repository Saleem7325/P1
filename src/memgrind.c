#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "bst.h"
#include "mymalloc.h"

extern tnode *root;

/*
* freePtr takes an array of char * and a boolean values indicating whethter to 
* free every pointer in the array or just one pointer
*/
void freePtr(char *p[], int b){
	for(int i = 0; i < 120; i++){
		if(p[i] != NULL){
			free(p[i]);
			p[i] = NULL;
			if(b) return;
		}
	}
}

/*
* setNull sets every index in p to NULL
*/
void setNULL(char *p[]){
	for(int i = 0; i < 120; i++)
		p[i] = NULL;
}

/*
* test1 mallocs a byte chunk then immeditely free it 120 times
*/
void test1(){
	for(int i = 0; i < 120; i++){
		char *ptr = malloc(sizeof(char));
		free(ptr);
	}
}

/*
* test2 mallocs 120 one byte chucks then frees all of them
*/
void test2(){
	char *p[120];
	for(int i = 0; i < 120; i++)
		p[i] = malloc(sizeof(char));
	freePtr(p, 0);
}

/*
* test3 randomly chooses between: allocating a one byte chunk and 
* storing the pointer returned in an array or deallocating one of
* pointers in the array if any. It repeats this process until
* mymalloc has been called 120 times, then all of the pointers
* are freed.
*/
void test3(){
	int malCount = 0;
	int size = 0;
	char *p[120];
	setNULL(p);

	while(malCount < 120){
		int r = rand() % 100000;
		if(r%2){
			int r1 = rand() % 120;
			if(p[r1] == NULL){
				p[r1] = malloc(sizeof(char));
				malCount++;
				size++;
			}
		}else if (size > 0){
			freePtr(p, 1);
			size--;
		}
	}
	freePtr(p, 0);
}

/*
* test4 inserts 128 random numbers into a bst which is the maximum numbers we can store using mymalloc and then
* deletes half of them randomly, then prints them inorder to check if any data got corrupted. After printing
* the node values to stdout the bst nodes are freed.
*/
void test4(){
	int p[64];
	int size = 0;
	for(int i = 0; i < 128; i++){
		int r = (rand() % 1000);
		insert(root, r);
		if(i%2){
			p[size] = r;	
			size++;
		}
	}
	
	for(int i = 0; i < 64; i++)
		delete(root, p[i]);

	inorderTraversal(root);
	freeAll(root);
	root = NULL;
}

/*
* test5 does two things:
* the first half shows that when a block is freed adjacent to unavailable blocks and 
* when malloc is called again it will retrieve the nearest block that has enough free memory for the requested size
* the second half tests that after there are no more available blocks left
* memory can no longer be allocated, but once a block is freed, 
* malloc can be called again if the chunk is big enough for what the client requested
*/
void test5(){
	char *p[10];
	for(int i = 0; i < 5; i++)
		p[i] = malloc(sizeof(char)* 16);

    	p[5] = malloc(sizeof(char)* 8);

    	for(int i = 6; i < 9 ; i ++)
		 p[i] = malloc(sizeof(char) * 8);
    
	free(p[5]); 
    	free(p[6]); 
	p[6] = NULL;
	p[5] = malloc(sizeof(char)* 24);  
    	p[9] = malloc(sizeof(char)); 

    	for(int i = 0; i < 10 ; i++)
		if(p[i]!= NULL) free(p[i]);
		
	// Second half of test
	char *pt[128];
	for(int i = 0; i < 128; i++)
		pt[i] = malloc(sizeof(char)*24);

	char *ptr = malloc(sizeof(char)); 
	if(ptr != NULL)
		exit(EXIT_FAILURE);

	int r = (rand() % 127);
	free(pt[r]);
	pt[r]= malloc(sizeof(char)*24); 

	for(int i = 0; i < 128; i++)
		free(pt[i]);
	
}

/*
* Computes the average of all the entries in nums and returns average
*/
double averageTime(double nums[]){
	double total = 0;
	for(int i = 0; i < 50; i++)
		total = total + nums[i];
	return (total/50);
}

/*
* runs test1, test2, test3, test4, and test5 50 times and times each run
* then prints the average of each test to stdout
*/
void runTests(){
	double test1times[50];
	double test2times[50];
	double test3times[50];
	double test4times[50];
	double test5times[50];	

	for(int i = 0; i < 50; i++){
		//Time test1
		struct timeval tv1;
		gettimeofday(&tv1, NULL);
		test1();
		struct timeval tv12;
		gettimeofday(&tv12, NULL);
		int time1 = tv12.tv_usec - tv1.tv_usec;
		test1times[i] = time1;		
		
		//Time test2
		struct timeval tv2;
		gettimeofday(&tv2, NULL);
		test2();
		struct timeval tv22;
		gettimeofday(&tv22, NULL);
		int time2 = tv22.tv_usec - tv2.tv_usec;
		test2times[i] = time2;		

		//Time test3
		struct timeval tv3;
		gettimeofday(&tv3, NULL);
		test3();
		struct timeval tv32;
		gettimeofday(&tv32, NULL);
		int time3 = tv32.tv_usec - tv3.tv_usec;
		test3times[i] = time3;

		//Time test4
		struct timeval tv4;
		gettimeofday(&tv4, NULL);
		test4();
		struct timeval tv42;
		gettimeofday(&tv42, NULL);
		int time4 = tv42.tv_usec - tv4.tv_usec;
		test4times[i] = time4;
	
		//Time test5
		struct timeval tv5;
		gettimeofday(&tv5, NULL);
		test5();
		struct timeval tv52;
		gettimeofday(&tv52, NULL);
		int time5 = tv52.tv_usec - tv5.tv_usec;
		test5times[i] = time5;
	}		
	printf("\nAverage time for test1 : %lf microseconds", averageTime(test1times));
	printf("\nAverage time for test2 : %lf microseconds", averageTime(test2times));	
	printf("\nAverage time for test3 : %lf microseconds", averageTime(test3times));
	printf("\nAverage time for test4 : %lf microseconds", averageTime(test4times));
	printf("\nAverage time for test5 : %lf microseconds\n", averageTime(test5times));
}

/*
* calls runTests() to test mymalloc/myfree
*/
int main(int argc, char** argv){
	runTests();
}
