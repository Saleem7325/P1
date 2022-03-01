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

tnode *delete(tnode *begin, int num){
	if (begin == NULL) 
		return NULL;
	
	if(begin->data < num){
		if(begin->right == NULL) 
			return begin;
		else 
			begin->right = delete(begin->right, num);
	}else if(begin->data > num){
		if(begin->left == NULL) 
			return begin;
		else 
			begin->left = delete(begin->left, num);
	}else{
		if((begin->left == NULL) && (begin->right == NULL)){
			free(begin);
			return NULL;	
		}else if(begin->left == NULL){  
			tnode *p = begin->right;
			if(begin == root) 
				p = root;
			free(begin);
			return p;
		}else if(begin->right == NULL){
			tnode *p = begin->left;
			if(begin == root) 
				root = p;
			free(begin);
			return p;
		}else{
			tnode *p = begin->left;
			while(p->right != NULL)
				p = p->right;
			
			begin->data = p->data;
		
			if(begin->left == p)
				begin->left = delete(p, p->data);
			else{
				tnode *q = begin->left;
				while(q->right != p)	
					q = q->right;
				
				q -> right = delete(p, p->data);
			}
		}
	}
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
	int p[60];
	int size = 0;
	for(int i = 0; i < 120; i++){
		int r = (rand() % 1000);
		insert(root, r);
		if(i%2){
			p[size] = r;	
			size++;
		}
	}
	
	for(int i = 0; i < 60; i++)
		delete(root, p[i]);

	inorderTraversal(root);
	freeAll(root);
}
// this test case tests 2 things:
// the first half shows that when a block is freed adjacent to unavailable blocks and 
// when malloc is called again it will retrieve the nearest block that has enough free memory for the requested size
// the second half tests that after there are no more available blocks left
// memory can no longer be allocated, but once a block is freed, 
// malloc can be called again if the chunk is big enough for what the client requested
void test5(){
    char *p[10];
    for(int i = 0; i < 5; i++){
        p[i] = malloc(sizeof(char)* 16);
    }
    p[5] = malloc(sizeof(char)* 8);
    for(int i = 6; i < 9 ; i ++){
        p[i] = malloc(sizeof(char) * 8);
    }
    free(p[5]); 
    free(p[6]); 
	p[6] = NULL;
    p[5] = malloc(sizeof(char)* 24);  
    p[9] = malloc(sizeof(char)); 
    for(int i = 0; i < 10 ; i++){
		if(p[i]!= NULL) free(p[i]);
		
    }
	// second half of test case
	char *pt[128];
	for(int i = 0; i < 128; i++){
		pt[i] = malloc(sizeof(char)*24);
	}
	char *ptr = malloc(sizeof(char)); 
	if(ptr != NULL){
		free(ptr);
	}else{
		printf("not enough\n");
	}
	int r = (rand() % 127);
	free(pt[r]);
	pt[r]= malloc(sizeof(char)*24); 
	for(int i = 0; i < 128; i++){
		free(pt[i]);
	}
}


//Specified in Section 4(1-3) of p1.pdf
int main(int argc, char** argv){
	test1();
	test2();
	test3();
	test4();
	test5();
}
