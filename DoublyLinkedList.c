#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAX 500
typedef struct node {
  int value;
  int t_s;
  struct node *next,*prev,*prevx,*nextx;
  int p_t_s,n_t_s;
} node;

typedef struct ppl
{
   node** heads;
   int size,cap;
}ppl;

node* get_prev(node* t)
{
  if(t!=NULL)
     return t->p_t_s == INT_MAX?t->prev:t->prevx;
}

node* get_next(node* t)
{
  if(t!=NULL)
     return t->n_t_s == INT_MAX?t->next:t->nextx;
}

node *create_node(int value, int t_s, int p_t_s, int n_t_s, node *next, node* prev, node* nextx, node* prevx) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->value = value;
  tmp->next = next;
  tmp->prev = prev;
  tmp->t_s = t_s;
  tmp->n_t_s = n_t_s;
  tmp->p_t_s = p_t_s;
  tmp->nextx = nextx;
  tmp->prevx = prevx;
  return tmp;
}

ppl* create_ppl(node** heads, int size, int cap)
{
    ppl *tmp = (ppl *)malloc(sizeof(ppl));
    tmp->heads = heads;
    tmp->size = size;
    tmp->cap = tmp->cap;
    return tmp;
}

node* search(int val, int vr,ppl* list)
{

  if(list->heads == NULL || vr>=list->size ||  list->heads[vr] == NULL)
   {  
     return NULL;}
  node* tmp = list->heads[vr];
  while(tmp != NULL && tmp->value != val)
   {
      if(tmp->n_t_s > vr)
         tmp = tmp->next;
      else
         tmp = tmp->nextx;
  }
  return tmp;
}

int delete(ppl* list, int value,int vr)
{
   node* del = search(value,vr, list);
   
   if(del == NULL)
      return 1;
 
   int q=0;
      
   node* dprev, *dnext;
   dprev = get_prev(del);
   dnext = get_next(del);
   if(dprev != NULL && dprev->n_t_s != INT_MAX)
   {  
      printf("Hello\n"); 
      q+=2;
      node *x,*y,*t;
      x = get_prev(dprev);
      dprev = y = create_node(dprev->value,vr+1,INT_MAX,INT_MAX,NULL,x,NULL,NULL);
      int f =1;
      while(x!=NULL && f)
      {
        if(x->n_t_s != INT_MAX)
         {
          t = create_node(x->value,vr+1,INT_MAX,INT_MAX,y,get_prev(x),NULL,NULL);
          y->prev = t;
          y = t;
          x = t->prev;
         }
        else
        {
         x -> nextx = y;
         x->n_t_s = vr+1;
         f = 0;
        }
      }
      list->heads[vr+1] = f?y:list->heads[vr];
   }
  
    if(dnext != NULL && dnext->p_t_s != INT_MAX)
   {  
     // printf("Err %d\n",value);
      q+=1;
      node *x,*y,*t;
      x = get_next(dnext);
      dnext = y = create_node(dnext->value,vr+1,INT_MAX,INT_MAX,x,NULL,NULL,NULL);
      int f =1;
      while(x!=NULL && f)
      {
        if(x->p_t_s != INT_MAX)
         {
          t = create_node(x->value,vr+1,INT_MAX,INT_MAX,get_next(x),y,NULL,NULL);
          y->next = t;
          y = t;
          x = t->next;
         }
        else
        {
         x -> prevx = y;
         x->p_t_s = vr+1;
         f = 0;
        }
      }
    }
   
   int f = (dprev == NULL)*2 + (dnext == NULL);
   if(f==0)
   {
     if(q/2)
      dprev->next = dnext;
     else
      { dprev->nextx = dnext; list->heads[vr+1] = list->heads[vr]; dprev->n_t_s = vr +1;  }
     if(q%2)
       dnext->prev = dprev;
     else
      { dnext->p_t_s = vr + 1;
        dnext->prevx = dprev;      
   }}
   else if(f==1)
   {
      if(q/2)
        dprev->next = NULL;
      else
       { dprev->nextx = NULL;
        dprev->n_t_s = vr+1;
         list->heads[vr+1] = list->heads[vr]; 
         //printf("rr %d %d\n",list->heads[vr+1]->value, vr );
          }
   }
   else if(f==2)
   {
      list->heads[vr+1] = dnext;
      if(q%2)
        dnext->prev = NULL;
      else
        { dnext->p_t_s = vr+1;
          dnext->prevx = NULL;
        }
      list->heads[vr+1] = dnext; 
   }
   else if(f==3)
   {
      list->heads[vr+1] = NULL;
   }
   list->size = list->size + 1;
   
   return 0;
}

void print_list(ppl* list, int vr)
{
  if(list->heads == NULL || vr>=list->size ||  list->heads[vr] == NULL)
     return;
 // printf("Hello\n");
  node* tmp = list->heads[vr];
  while(tmp != NULL)
   {
      printf("%d ",tmp->value);
      if(tmp->n_t_s > vr)
         tmp = tmp->next;
      else
         tmp = tmp->nextx;
  }
  return ;
}

int insert(ppl* list, int value, int t_s)
{
   if(list != NULL){
   if(list->heads == NULL)
   {
     list->heads = (node**)malloc(MAX*sizeof(node*));
     for(int it = 0;it < MAX; it++)
        list->heads[it] = NULL;
     list->heads[t_s] = create_node(value,t_s,INT_MAX,INT_MAX,NULL,NULL,NULL,NULL);
     list->size = 1;
     return 0;
   }
   
   node* tmp = create_node(value,t_s,INT_MAX,INT_MAX,NULL,NULL,NULL,NULL), *prev = list->heads[t_s -1];
   list->size = list->size + 1;
   node *a,*b,*t;
   a = prev;
   b = tmp;
   int f = 1;
   while(a != NULL && f)
   {
     if(a->p_t_s != INT_MAX)
     {
       t = create_node(a->value,t_s,INT_MAX,INT_MAX,a->n_t_s == INT_MAX?a->next:a->nextx,tmp,NULL,NULL);
       tmp->next = t;
       tmp = t;
       a = t->next;
     }
     else
     {
       tmp->next = a;
       a->prevx = tmp;
       a->p_t_s = t_s;
       f = 0;
     }
   }
   tmp->next = a;
   list->heads[t_s] = b;   
      return  0;
   }
   return 1;
}

int main()
{
   int t,i,j,k,n,v=-1;
   ppl* list;
   list = create_ppl(NULL,0,MAX);
   printf("Enter the no of opearations you want to perform\n");
   scanf("%d", &n);
   printf(" 1. search \n 2. insert \n 3. delete \n 4. print\n");
   for(int it=0;it<n;it++)
   {
     scanf("%d", &i);
      if(i==1)
      {
        printf("Enter the number to search followed by the version number\n");
         scanf("%d %d",&j,&k); 
        if(search(j,k,list))
         printf("found\n");
        else
          printf("Not found\n");
      }
      else if(i==2)
      {
        printf("Enter the number to insert\n");
        scanf("%d", &j);
        insert(list,j,++v);
      }
      else if(i==3)
      {
        printf("Enter the number to delete\n");
        scanf("%d", &j);
        delete(list,j,v++);
      }
      else if(i==4)
      {
         printf("Enter the version\n");
         scanf("%d", &j);
        print_list(list,j);
        printf("\n");
      }
   }
}

