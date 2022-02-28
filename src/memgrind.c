#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

typedef struct treeNode{
	int data;
	struct treeNode *left;
	struct treeNode *right;
}tnode;

tnode *root = NULL;

tnode *insert(tnode *begin, int num){
	if(root == NULL){
		root = malloc(sizeof(tnode));
		root->data =num;
		root->left = NULL;
		root->right = NULL;
		return root;
	}else if(begin == NULL){		
		tnode * p  = malloc(sizeof(tnode));
		p->data = num;
		p->left = NULL;
		p->right = NULL;
		return p;	
	}
	
	if(begin->data < num)
		begin-> right = insert(begin->right, num);
	else if(begin->data > num)
		begin->left = insert(begin->left, num);
			
	return begin;
}

void freeAll(tnode *begin){
	if(begin == NULL) return;
	freeAll(begin->right);
	freeAll(begin->left);
	free(begin);
}

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

void inorderTraversal(tnode *begin){
	if(begin == NULL) return;
	inorderTraversal(begin->left);
	printf("\n%d\n", begin->data);
	inorderTraversal(begin->right);
}

void test4(){
	for(int i = 0; i < 120; i++)
		insert(root, (rand() % 1000));
	
	inorderTraversal(root);
	freeAll(root);
}


//Specified in Section 4(1-3) of p1.pdf
int main(int argc, char** argv){
	test1();
	test2();
	test3();
	test4();
}
