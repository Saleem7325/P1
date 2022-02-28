#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
//#include "mymalloc.c"
//We just need to add 2 more stress tests of our own

void freePtr(char *p[], int b){
	for(int i = 0; i < 120; i++){
		if(p[i] != NULL){
			free(p[i]);
			p[i] = NULL;
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
/*	test1();
	test2();
	test3();

	int *p = (int *)malloc(sizeof(int) * 10);
	double *ptr = (double *)malloc(sizeof(double)*10);	

	for(int i = 0; i < 10; i++){
		p[i] = i * (rand() % 100);
		ptr[i] = (rand() % 1000);
	}

	for(int i = 0; i < 10; i++)
		printf("\np[%d] = %d\n", i, p[i]); 

	for(int i = 0; i < 10; i++)
		printf("\nptr[%d] = %f\n", i, ptr[i]);	
	
	printMem();

	puts("\nFree ptr double");
	free(ptr);
	printMem();

	puts("\nFree p int");
	free(p);
	printMem();*/
	
/*	char *p[120];
	int *in[120];

	for(int i = 0; i < 120; i++){
		p[i] = malloc(sizeof(char));
		in[i] = malloc(sizeof(int));
	} 
	
	printMem();

	for(int i = 0; i < 120; i++){
		free(in[i]);
		free(p[i]);
	}

	printMem();*/

/*	int *a[120] = malloc(sizeof(int)*120);
	double *b[120]= malloc(sizeof(double)*120);
	char *c[120] = malloc(sizeof(char)*400);
	
	printMem();

	puts("\nfree c\n");
	free(c);	
	printMem();
	
	puts("\nfree b\n");
	free(b);
	printMem();

	puts("\nfree a\n");
	free(a);
	printMem();*/


}
