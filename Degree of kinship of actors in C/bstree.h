#ifndef __BSTREE_H__
#define __BSTREE_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct g_node
{
	int indice;
	char name[100];
}g_node;


typedef struct BSTree{
	struct g_node key;
	int n;
	struct BSTree *left , *right;
}BSTree;




void init_BSTree(BSTree **tree);
void add(BSTree **tree, g_node val);
int search(BSTree *tree, char *val);
BSTree* low_left(BSTree *tree);
BSTree* delete(BSTree **tree,g_node val);
void free_tree(BSTree *tree);
void inOrder(BSTree *tree,FILE *fp);
#endif