#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bstree.h"
#include "queue.h"

#define MAX 30000

typedef struct graph {
	int nr_nodes;
	char **name;
	struct node **neighbours;
}graph;

typedef struct node
{
	int val;
	struct node* next;
}node;


/* Functie de initializare a grafului. Se stabilesc numarul de noduri,
se initializeaza listele de adiacenta si vectorul de string-uri. */
void initG (graph **g,int n )
{
	int i,j;
	(*g)=(graph *)malloc(sizeof(graph));
	(*g)->nr_nodes=n;
	(*g)->neighbours=(node **)malloc((n+1)*sizeof(node *));
	(*g)->name=(char **)malloc((n+1)*sizeof(char*));
	for(i=1;i<=n;i++)
	{
		(*g)->neighbours[i]=NULL;
		(*g)->name[i]=NULL;	
	}
}


/* Functie de adaugare a unui arc intre 2 noduri. Se considera un graf 
neorientat (specific temei) , deci se vor adauga nodurile in listele de 
adiacenta a celuilalt. */
void addArc(graph **g,int x, int y)
{
	node *nou1=(node*)malloc(sizeof(node));
	nou1->val=y;
	nou1->next=(*g)->neighbours[x];
	(*g)->neighbours[x]=nou1;
	node *nou2=(node*)malloc(sizeof(node));
	nou2->val=x;
	nou2->next=(*g)->neighbours[y];
	(*g)->neighbours[y]=nou2;
	
}


/* Functie care verifica daca exista un arc intre 2 noduri. Mai exact daca 
exista valoarea y in lista de adiacenta a lui x. */
int arc(graph *g,int x , int y)
{
	node * p;
	for(p=g->neighbours[x];p!=NULL;p=p->next)
		if(y==p->val)
			return 1;

	return 0;
}


/* Functie de construire a grafului , care primeste binary search tree-ul
construit pentru verificarea aparitiilor aceluiasi nume de mai multe ori,
din care se extrag indicii numelor din vector si se leaga intre ei in graf. */
void build_Graph( graph **g,int n,char **s,BSTree *tree)
{
	int i,j,a,b;
	if(n-1==1)
	{
		a=search(tree,s[1]);
		addArc(&(*g),a,a);
		return;
	}
		
	for(i=1;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			a=search(tree,s[i]);
			b=search(tree,s[j]);	
			if(arc(*g,a,b)==0)
			{
				addArc(&(*g),a,b);
				if((*g)->name[a]==NULL)
					(*g)->name[a]=strdup(s[i]);
				if((*g)->name[b]==NULL)
					(*g)->name[b]=strdup(s[j]);
			}
		}
	}
}


/* Functie recursiva de eliminare a unui nod din graf. Nefolosita in cadrul 
acestei teme, fiind mai mult pentru debugging. Se elimina valoarea nodului 
din lista de adiacenta. */ 
void eliminare(node **p,int v)
{
	node *q;
	if(*p==NULL)
		return;
	if(v==(*p)->val)
	{
		q=*p;
		*p=(*p)->next;
		free(q);
	}
	else
		eliminare(&(*p)->next,v);
}


/* Functie care elimina nodul din toate listele de adiacenta ale nodurilor,
prin apelarea functiei anterioare, si declara lista de adiacenta a nodului 
eliminat nula. */
void eliminareNod(graph **g,int v)
{
	int i;
	for(i=1;i<=(*g)->nr_nodes;i++)
		eliminare(&(*g)->neighbours[i],v);
	(*g)->neighbours[v]=NULL;
}


/* Functie pentru afisarea tuturor listelor de adiacenta a nodurilor din graf,
fiind mai mult pentru debugging. */
void show(graph *g)
{
	int i;
	node *p;
	printf("Graful reprezentat cu liste de adiacenta\n");
	for(i=1;i<=g->nr_nodes;i++)
	{
		printf("%d: ",i);
		p=g->neighbours[i];
		while(p!=NULL)
		{
			printf("%d ",p->val);
			p=p->next;
		}
		printf("\n");
	}
}



/* O parcurgere dfs , care numara fiecare apel al functiei. */
int dfs(graph *g ,int i,int visited[],int *val)
{
	node *p;
	(*val)++;
	p=g->neighbours[i];
	visited[i]=1;
	while(p!=NULL)
	{
		i=p->val;
		if(visited[i]==0)
			dfs(g,i,visited,val);
		p=p->next;
	}
}


/* O parcurgere dfs , care retine numele fiecarui actor din componenta conexa
intr-un arbore binar de cautare. */
int dfs_mod(graph *g ,int i,int visited[], BSTree **tree)
{
	node *p;
	g_node curent;
	strcpy(curent.name,g->name[i]);
	curent.indice=(*tree)->n+1;
	add(&(*tree),curent);
	p=g->neighbours[i];
	visited[i]=1;
	while(p!=NULL)
	{
		i=p->val;
		if(visited[i]==0)
			dfs_mod(g,i,visited,tree);
		p=p->next;
	}
}


/* O parcurge bfs folosind o coada. */
void bfs(graph *g, int v,int vazut[],int distanta[])
{
	int x,y;
	Queue *q;
	init_queue(&q);
	vazut[v]=1;
	distanta[v]=0;
	add_queue(q,v);
	while(isEmptyQueue(q)!=0)
	{
		x=pop_queue(q);
		for(y=1;y<=g->nr_nodes;y++)
		{				
			if(arc(g,x,y) && vazut[y]==0)
			{
				vazut[y]=1;
				distanta[y]=distanta[x]+1;
			
				add_queue(q,y);
			}
		}
	}
	free_queue(&q);
	free(q);
}


/* Functie de dealocare a memoriei din vectorul de string-uri. */
void free_strings( char **s,int n)
{
	int i;
	for(i=1;i<n;i++)
		free(s[i]);	
	free(s);
}


/* Functie care calculeaza productia maxima. Se foloseste de cele doua functii
de dfs , la prima numarand numarul aparitiilor si comparandu-l cu maximul , iar
la cea de-a doua construind bst-ul pentru sortare alfabetica. De asemenea, pentru 
reducerea timpului de executie am pus conditia ca doar in cazul in care nu 
exista un arc intre nodul curent si cel anterior se va apela dfs. */
void prod_max( graph *g,char fisier[])
{
	int *visited,i,val,ind,max=0;
	BSTree *tree;
	FILE *fp;
	init_BSTree(&tree);
	for(i=1;i<=g->nr_nodes;i++)
	{	
		if(i==1 || arc(g,i-1,i)==0)
		{
			val=0;
			visited=(int *)calloc(g->nr_nodes+1,sizeof(int));
			dfs(g,i,visited,&val);
			if(val>max)
			{
				max=val;
				ind=i;
			}
			free(visited);
		}
	}
	visited=(int *)calloc(g->nr_nodes+1,sizeof(int));
	dfs_mod(g,ind,visited,&tree);
	free(visited);
	fp=fopen(fisier,"w");
	fprintf(fp, "%d\n", max);
	inOrder(tree,fp);
	fclose(fp);
	free_tree(tree);
}


/* Functie care afla distanta de la un nod la alt nod. Se foloseste de bfs de 
la nodul de la care dorim sa calculam distanta, construind vectorul distanta ,
iar dupa afisand valoarea unde indicele corespunde cu nodul la care dorim sa 
aflam distanta. */ 
void grad_de_inrudire(graph *g,int  a,int  b,char fisier[])
{
	int *visited, *distanta;
	FILE *fp;
	visited=(int *)calloc(g->nr_nodes+1,sizeof(int));
	distanta=(int *)calloc(g->nr_nodes+1,sizeof(int));

	bfs(g,a,visited,distanta);

	fp=fopen(fisier,"w");

	if(distanta[b]==0)
		fprintf(fp,"-1\n");
	else
		fprintf(fp,"%d\n",distanta[b]);
	fclose(fp);

	free(visited);
	free(distanta);

}


/* Functie care calculeaza minimul dintre 2 intregi. */
int min(int a, int b)
{
	if(a<b)
		return a;
	else
		return b;
}


/* Functie asemanatoare cu dfs-ul , folosita de functia punti, care afla
daca exista o punte intre 2 noduri ale grafului. In caz afirmativ aceasta
creeaza un string compus din cele 2 nume asociate nodurilor ce constituie 
puntea si le introduc intr-un nou binary search tree. */
void dfsB(graph *g, int v, int timp,int idx[],int low[],int pi[], BSTree **tree)
{
	node *p;
	int i;
	g_node curent;
	char aux[100];
	idx[v]=timp;
	low[v]=timp;
	timp++;
	for(p=g->neighbours[v];p!=NULL;p=p->next)
	{
		if(p->val!=pi[v])
		{
			if(idx[p->val]==-1)
			{
				pi[p->val]=v;
				dfsB(g,p->val,timp,idx,low,pi,tree);
				low[v]=min(low[v],low[p->val]);
				if(low[p->val]>idx[v])
				{
					if(strcmp(g->name[v],g->name[p->val])<0)
					{
						strcpy(aux,g->name[v]);
						strcat(aux," ");
						strcat(aux,g->name[p->val]);
						strcpy(curent.name,aux);
						curent.indice=(*tree)->n+1;
						add(&(*tree),curent);	
					}
					else
					{
						strcpy(aux,g->name[p->val]);
						strcat(aux," ");
						strcat(aux,g->name[v]);
						strcpy(curent.name,aux);
						curent.indice=(*tree)->n+1;
						add(&(*tree),curent);	
					}
				}
			}
			else
			{
				low[v]=min(low[v],idx[p->val]);
			}
		}
	}



}


/* Functie care o apeleaza pe cea anterioara pentru aflarea puntilor. Dupa 
iesirea din dfsB se intoarce scriind in fisier continutul bst-ului in inorder,
pentru a afisate in ordine alfabetica. */ 
void punti(graph *g,char fisier[])
{
	int timp;
	int idx[g->nr_nodes];
	int pi[g->nr_nodes];
	int low[g->nr_nodes];
	int i;
	BSTree *tree;
	FILE *fp;
	init_BSTree(&tree);
	timp=0;
	for(i=1;i<=g->nr_nodes;i++)
	{
		idx[i]=-1;
		low[i]=MAX;
		pi[i]=0;
	}
	for(i=1;i<=g->nr_nodes;i++)
	{
		if(idx[i]==-1)
			dfsB(g,i,timp,idx,low,pi,&tree);
	}
	fp=fopen(fisier,"w");
	fprintf(fp, "%d\n",tree->n );
	inOrder(tree,fp);
	fclose(fp);
	free_tree(tree);
}


/* Functia main care primeste argumentele din linia de comanda. */
int main(int argc, char *argv[])
{	FILE *fp;
	graph *g;
	BSTree *tree;
	g_node curent;
	int n,m,i=1,i1,i2;
	char c[100],**s,**a;


/* Construim bst-ul pentru utilizarea ulterioara si aflarea tuturor numelor
distincte. Dupa construirea tree-ului initializam si graful. */
	init_BSTree(&tree);
	fp=fopen(argv[argc-2],"r");
	fgets(c,100,fp);
	n=atoi(c);
	while(n!=0)
	{
		fgets(c,100,fp);
		fgets(c,100,fp);
		m=atoi(c);
		while(m!=0)
		{
			fgets(c,100,fp);
			if(search(tree,c)==-1)
			{
				strcpy(curent.name,c);	
				curent.indice=tree->n+1;							
				add(&tree,curent);
			}
			m--;
		}
		n--;					
	}
	fclose(fp);

	initG(&g,tree->n);


/* Parcurgem iar fisierul retinand fiecare nume intr-un vector de string-uri
si folosind , dupa fiecare film , tree-ul si vectorul pentru a adauga nodurile
in graf cu ajutorul functiei de build. */
	fp=fopen(argv[argc-2],"r");
	fgets(c,100,fp);
	n=atoi(c);
	
	while(n!=0)
	{
		fgets(c,100,fp);
		fgets(c,100,fp);
		m=atoi(c);
		s=(char **)malloc((m+1)*sizeof(char *));

		while(m!=0)
		{
			fgets(c,100,fp);
			s[i]=(char*)malloc((strlen(c)+1)*sizeof(char));
			strcpy(s[i],c);
			i++;
			m--;
		}
		build_Graph(&g,i,s,tree);	
		free_strings(s,i); 
		n--;				
		i=1;
	}


/* Se citesc cele 2 string-uri in plus pentru cerinta 2 si se afla indicele
acestora pentru aflarea distantei dintre ele cu bfs-ul. */
	if(strcmp(argv[argc-3],"-c2")==0)
	{
		fgets(c,100,fp);
		i1=search(tree,c);																
		fgets(c,100,fp);
		i2=search(tree,c);
	}

	fclose(fp);
	free_tree(tree);


/* functia de productie maxima. */
	if(strcmp(argv[argc-3],"-c1")==0)
	{
		prod_max(g,argv[argc-1]);
	}


/* functia gradului de inrudire. */
	if(strcmp(argv[argc-3],"-c2")==0)
	{
		grad_de_inrudire(g,i1,i2,argv[argc-1]);
	}


/* functia de punti. */
	if(strcmp(argv[argc-3],"-c3")==0)
	{
		punti(g,argv[argc-1]);
	}


/* Dealocarea memoriei folosite. */
	for(i=1;i<=g->nr_nodes;i++)
		free(g->name[i]);
	free(g->name);
	for(i=1;i<=g->nr_nodes;i++) 													
	{
		node *p,*temp;
		p=g->neighbours[i];
		while(p!=NULL)
		{
			temp=p;
			p=p->next;
			free(temp);

		}
	}
	free(g->neighbours);
	free(g);


	return 0;
}
