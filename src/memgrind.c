#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
//#include "mymalloc.c"
//We just need to add 2 more stress tests of our own

void freePtr(char *p[], int b){
	for(int i = 0; i < 120; i++){
		if(p[i] != NULL){
			free(p[i]);
			if(b) return;
		}
	}
}

void setNULL(char *p[]){
	for(int i = 0; i < 120; i++)
		p[i] = NULL;
}

void test1(){
	for(int i = 0; i < 120; i++){
		char *ptr = malloc(sizeof(char));
		free(ptr);
	}
}

void test2(){
	char *p[120];
	for(int i = 0; i < 120; i++)
		p[i] = malloc(sizeof(char));
	freePtr(p, 0);
}

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

//Specified in Section 4(1-3) of p1.pdf
int main(int argc, char** argv){
	test1();
	test2();
	test3();

/*	int *p = (int *)malloc(sizeof(int) * 10);
//	double *ptr = (double *)malloc(sizeof(double)*10);	

	for(int i = 0; i < 10; i++){
		p[i] = i * (rand() % 100);
//		ptr[i] = 1.0;
	}

	for(int i = 0; i < 10; i++)
		printf("\np[%d] = %d\n", i, p[i]); 

	for(int i = 0; i < 10; i++)
		printf("\nptr[%d] = %f\n", i, ptr[i]);*/	

}
