
#include "bstree.h"


/* Functie de initializare a bst-ului. */
void init_BSTree(BSTree **tree)
{
	(*tree)=(BSTree *)calloc(1,sizeof(BSTree));
	(*tree)->n=0;
}


/* Functie de comparare a 2 string-uri , eliminand '\n' in cazul in care
exista intr-un string. */
int comparare(char *a , char *b)
{
	if(a[strlen(a)-1]=='\n')
		*(a+strlen(a)-1)='\0';
	if(b[strlen(b)-1]=='\n')
		*(b+strlen(b)-1)='\0';
	return strcmp(a,b);
}


/* Functie recursiva de adaugare a unui nod in arbore. */
void add(BSTree **tree, g_node val)
{
	
	BSTree *new=(BSTree*)malloc(sizeof(BSTree));
	new->key=val;
	new->left=new->right=NULL;

	if((*tree)==NULL)
	{
		(*tree)=new;
		return ;
	}

	(*tree)->n++;

	if(comparare(val.name,(*tree)->key.name)<0)
		{
			free(new);
			add(&(*tree)->left,val);
		}
	else if(comparare(val.name,(*tree)->key.name)>0)
		{
			free(new);
			add(&(*tree)->right,val);
		}

	return ;
}


/* Functie recursiva de cautare a unui nod in arbore. */
int search(BSTree *tree, char *val)
{
	int p=-1;

	if(tree==NULL)
		return -1;

	if(comparare(val,tree->key.name)==0)
		return tree->key.indice;
	if(comparare(val,tree->key.name)<0)
		p=search(tree->left,val);
	if(p!=-1)
		return p;
	else
		return search(tree->right,val);

	return -1;
}


/* Functie care afla cel mai din stanga nod. Folosita pentru delete. */
BSTree* low_left(BSTree *tree)
{
	BSTree *current=tree ;

	while(current && current->left !=NULL)
  			current=current->left;
  	return current;
}


/* Functie recursiva de eliminare a unui nod din arbore. Nefolosita in tema ,
este doar pentru debugging. */
BSTree* delete(BSTree **tree,g_node val)
{ 
	BSTree *temp;
	(*tree)->n--;
	if(*tree==NULL)
		return *tree;
  	if(val.indice<(*tree)->key.indice)
  		(*tree)->left=delete(&(*tree)->left,val);
  	else if(val.indice>(*tree)->key.indice)
  		(*tree)->right=delete(&(*tree)->right,val);
  	else
  	{
  		if((*tree)->left==NULL)
  		{
  			temp=(*tree)->right;
  			free(*tree);
  			return temp;
  		}
  		else if((*tree)->right==NULL)
  		{
  			temp=(*tree)->left;
  			free(*tree);
  			return temp;
  		}
  		temp=low_left((*tree)->right);
  		
  		(*tree)->key=temp->key;
  		(*tree)->right=delete(&(*tree)->right,temp->key);
  	}
  	return (*tree);
}


/* Functie recursiva de eliberarea a memoriei nodurilor din arbore. */
void free_tree(BSTree *tree)
{ 
	if(tree==NULL)
	return;

	free_tree(tree->right);
	free_tree(tree->left);
	free(tree);
}


/* Functie recursiva ce afiseaza nodurile inorder. */ 
void inOrder(BSTree *tree,FILE *fp)
{if(tree==NULL) return;
	inOrder(tree->left,fp);
	if(tree->key.indice!=0)
	{
		if(tree->key.name[strlen(tree->key.name)-1]=='\n')
		fprintf(fp,"%s",tree->key.name );
	else
		fprintf(fp,"%s\n",tree->key.name );
	}
	inOrder(tree->right,fp);
}