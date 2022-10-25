
#include "queue.h"


/* Functie de initializare a cozii. */
void init_queue(Queue **q)
{
  *q=(Queue *)malloc(sizeof(Queue));
  (*q)->len=0;
  (*q)->tail=NULL;
  (*q)->head=NULL;
}


/* Functie de eliberarea a memoriei alocate cozii. */
void free_queue(Queue** q)
{
  node_q *temp=NULL;

  while((*q)->head)
  {
    temp=(*q)->head;
    (*q)->head=(*q)->head->next;
    free(temp);
  }
  (*q)->len=0;
  (*q)->tail=(*q)->head=NULL;
}


/* Functie de adaugarea unui nod in coada. */
int add_queue(Queue * q, int val)
{
  node_q *nou=(node_q *)malloc(sizeof(node_q));
  if(nou==NULL)
    return 0;
  nou->data=val;
  nou->next=NULL;
  if(q->head==NULL)
  {
    q->head=q->tail=nou;
    q->len++;
  }
  else
  {
    q->tail->next=nou;
    q->tail=nou;
    q->len++;
  }
  return 1;
}


/* Functie de scoatere si intoarcere a primului nod adaugat in coada. */
int pop_queue(Queue *q)
{
  int el;node_q *aux;
  el=q->head->data;
  if(q->head==q->tail)
  {
    free(q->head);
    q->head=q->tail=NULL;
  }
  else
  {
    aux=q->head;
    q->head=q->head->next;
    free(aux);
  }
  q->len--;
  return el;
}


/* Functie de afisare a cozii. Pentru debugging. */
void print_queue(Queue *q)
{
  node_q * current=q->head;
  while(current!=NULL)
  {
    printf(" %d ",current->data );
    current=current->next;
  }
  printf("\n");
}


/* Functie ce intoarce primul nod adaugat in coada. */
int front(Queue *q)
{
  return(q->head->data);
}


/* Functie care intoarce lungimea cozii. */
int isEmptyQueue(Queue *q)
{
  return q->len;
}