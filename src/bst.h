#include <stddef.h>

typedef struct treeNode{
	int data;
	struct treeNode *left;
	struct treeNode *right;
}tnode;

tnode *insert(tnode *begin, int num);
tnode *delete(tnode *begin, int num);
void inorderTraversal(tnode *begin);
void freeAll(tnode *begin);
