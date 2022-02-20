#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
//We just need to add 2 more stress tests of our own

void freePtr(char *p[], int b){
	for(int i = 0; i < 120; i++){
		if(p[i] != NULL){
			free(p[i]);
			if(b) return;
		}
	}
}

//Specified in Section 4(1-3) of p1.pdf
int main(int argc, char** argv){
	for(int i = 0; i < 120; i++){
		char *ptr = malloc(sizeof(char));
		free(ptr);
	}

	char *p[120];
	for(int i = 0; i < 120; i++)
		p[i] = malloc(sizeof(char));
	freePtr(p, 0);	

	int malCount;
	int size = 0;
	while(malCount < 120){
		int r = rand() % 100000;
		if(r%2){
			int r1 = rand() % 121;
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
