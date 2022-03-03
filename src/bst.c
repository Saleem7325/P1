#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
#include "bst.h"

tnode *root = NULL;

/*
* insert method takes the root node and an int and inserts it
* in the correct place in the binary search tree
*/
tnode *insert(tnode *begin, int num){
	if(root == NULL){
		root = (tnode *)malloc(sizeof(tnode));
		root->data =num;
		root->left = NULL;
		root->right = NULL;
		return root;
	}else if(begin == NULL){		
		tnode *p  = (tnode *)malloc(sizeof(tnode));
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

/*
* delete take the root node and a number to delete and deletes that
* number from the bst if it exsists. In the case of a node having two
* children and needing to be deleted, that node is replaced with the 
* right most node in the left subtree and the right most node in the 
* left subtree is deleted.. 
*/
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
				root = p;
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
				
				q->right = delete(p, p->data);
			}
		}
	}
	return begin;
}

/*
* Prints every number in the bst in order
*/
void inorderTraversal(tnode *begin){
	if(begin == NULL) return;
	inorderTraversal(begin->left);
	printf("\n%d\n", begin->data);
	inorderTraversal(begin->right);
}

/*
* freeAll takes the root of a bst and frees all the tnodes
* of the tree through post order traversal
*/
void freeAll(tnode *begin){
	if(begin == NULL) return;
	freeAll(begin->right);
	freeAll(begin->left);
	free(begin);
}
