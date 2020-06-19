#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAX 500
typedef struct node {
  int value;
  struct node *next, *extra;
  int e_t_s;
} node;

typedef struct ppl
{
   node** heads;
   node** tails;
   node* tail;
   int size,cap,vr;
}ppl;

node *create_node(int value, int e_t_s, node *next, node* extra) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->value = value;
  tmp->next = next;
  tmp->extra = extra;
  tmp->e_t_s = e_t_s;
  return tmp;
}

ppl* create_ppl(node** heads, node** tails, node* tail, int size, int cap,int vr)
{
    ppl *tmp = (ppl *)malloc(sizeof(ppl));
    tmp->heads = heads;
    tmp->tails = tails;
    tmp->tail = tail;
    tmp->size = size;
    tmp->vr = vr;
    tmp->cap = tmp->cap;
    return tmp;
}

void check_size(ppl* list)
{
    if(list->size == list->cap)
    {
       node** new_heads, **new_tails;
       list->cap = 2*list->cap;
       new_heads = (node**)malloc(list->cap*sizeof(node*));
       new_tails = (node**)malloc(list->cap*sizeof(node*));
      for(int i=0;i<list->size;i++)
         {
            new_heads[i] = list->heads[i];  
            new_tails[i] = list->tails[i];
         }
        free(list->heads);
        free(list->tails);
        list->heads = new_heads;
        list->tails = new_tails;
     }
}

node* search(int val, int vr,ppl* list)
{

  if(list->heads == NULL || vr>=list->size ||  list->heads[vr] == NULL)
   {  
     //fprintf(out,"Hello\n"); 
     return NULL;}
  node* tmp = list->heads[vr];
  while(tmp != NULL && tmp->value != val)
   {
      if(tmp->e_t_s > vr)
         tmp = tmp->next;
      else
         tmp = tmp->extra;
    //fprintf(out,"Helloi");
  }
  //fprintf(out,"Helloe");
  return tmp;
}
node* delete_util(node* head, int value, int* change, bool* is_tail, ppl* list, node* del_ptr)
{
  int i,j,k;
  if(head == NULL)
    { *change = 0;return NULL;}
  node* tmp;
  if(head->value != value ||  ( (del_ptr != NULL) && (del_ptr != head ))){
    i = *change;
    tmp = delete_util((head->e_t_s!=INT_MAX)?head->extra:head->next, value, &i,is_tail,list,del_ptr);
    
  if(i)
   {
     if(head->e_t_s != INT_MAX)
     {
       node* t = create_node(head->value,INT_MAX,tmp,NULL);
       if(*is_tail)
         { list->tail = t;
           *is_tail = false;
         }
       return t;  
     }
     else
     {
        
       head->extra = tmp;
       head->e_t_s = *change+1;
       *change = 0;
       if(*is_tail)
         { list->tail = head;
           *is_tail = false;
         }
       return NULL;     
     }
   }
   else
   {
     *change = 0;
     return NULL;
   }
  }
   if(list->tail == head)
     *is_tail = true;
   return (head->e_t_s!=INT_MAX)?head->extra:head->next;  
}

int delete(ppl* list, int value,bool back)
{
   if(!list)
     return 1;
   int vr = list->vr;
   int tmp = vr;
   bool is_tail = false;
  if(list != NULL){
   check_size(list);
  if(search( value, vr, list)==NULL)
   return 1;
   node* nxt;
   if(back)
    nxt = delete_util(list->heads[list->size-1], value, &tmp,&is_tail,list,list->tail);
   else
    nxt = delete_util(list->heads[list->size-1], value, &tmp,&is_tail,list,NULL);
   if(tmp)
    list->heads[vr+1] = nxt;
   else
    list->heads[vr+1] = list->heads[vr]; 
   list->size = list->size + 1; 
   if(is_tail)
      list->tail = NULL; 
   list->vr = list->vr + 1;
   list->tails[list->vr] = list->tail;
   return 0;
   }
   return 1;
}

void print_list(ppl* list, int vr)
{
  if(list->heads == NULL || vr>=list->size ||  list->heads[vr] == NULL)
   {  printf("\n");
     return;}
  node* tmp = list->heads[vr];
  while(tmp != NULL)
   {
      printf("%d ",tmp->value);
      if(tmp->e_t_s > vr)
         tmp = tmp->next;
      else
         tmp = tmp->extra;
  }
  printf("\n");
  return ;
}

int insert(ppl* list, int value)
{
   
   if(list != NULL){
   int t_s =++list->vr;
   if(list->heads == NULL)
   {
     list->heads = (node**)malloc(MAX*sizeof(node*));
     list->tails = (node**)malloc(MAX*sizeof(node*));
     for(int it = 0;it < MAX; it++)
        list->tails[it] = list->heads[it] = NULL;
     list->tails[t_s] = list->tail = list->heads[t_s] = create_node(value,INT_MAX,NULL,NULL);
     list->size = 1;
     return 0;
   }
   check_size(list);
   node* tmp = create_node(value,INT_MAX,NULL,NULL), *prev = list->heads[t_s -1];
   list->size = list->size + 1;
   tmp->next = prev;
   list->heads[t_s] = tmp;
   if(prev == NULL)
      list->tail = tmp; 
    list->tails[t_s] = list->tail; 
      return  0;
   }
   return 1;
}
node* insert_end_util(node* head, ppl* list,int value,int t_s)
{
   if(head == NULL)
    {
        node* tmp = create_node(value,INT_MAX,NULL,NULL);
        list->size = list->size + 1;
        list->tail =  tmp;
        return tmp;
    }
     if(head->e_t_s > t_s)
        { 
         node* tmp =  insert_end_util( head->next, list, value, t_s);
         if(tmp!= NULL)
            {
                head->extra = tmp;
                head->e_t_s = t_s;
            }
           return NULL;
        }
      else
       {
         node* tmp =  insert_end_util( head->extra, list, value, t_s);
         if(tmp!=NULL)
         {
            node* t = create_node(head->value,INT_MAX,tmp,NULL);
            return t;
         }
         return NULL;
       }
}


int insert_end(ppl* list, int value)
{
   
    if(list != NULL){
   int t_s =++list->vr;
   if(list->heads == NULL)
   {
     list->heads = (node**)malloc(MAX*sizeof(node*));
     list->tails = (node**)malloc(MAX*sizeof(node*));
     for(int it = 0;it < MAX; it++)
        list->heads[it] = NULL;
     list->tails[t_s] = list->tail = list->heads[t_s] = create_node(value,INT_MAX,NULL,NULL);
     list->size = 1;
     return 0;
   }
   check_size(list);
   node* tmp = insert_end_util(list->heads[t_s-1],list,value,t_s);
   if(tmp!=NULL)
      list->heads[t_s] = tmp;
   else
     list->heads[t_s] = list->heads[t_s-1];
    list->tails[t_s] = list->tail; 
    return 0;
   }
   return 1;
}

node* front(ppl* list, int vr)
{
    return list->heads[vr];
}

node* back(ppl* list, int vr)
{
    return list->tails[vr];
}

typedef struct queue
{
  ppl* list;
  int size;
}queue;
void init_queue(queue* q)
{
  q->size = 0;
  q->list =  create_ppl(NULL,NULL,NULL,0,MAX,-1);
}
int enqueue(queue* q,int val)
{
  if(q!=NULL){
   insert_end(q->list,val);
   q->size = q->size +1;
   return 0;}
  return 1;
}
bool empty(queue *q, int vr)
{
  if(q!=NULL){
   if(q->list->heads[vr] == NULL)
      return true;
    return false;
    }
    return true;
}

int dequeue(queue* q)
{
    if(q!=NULL){
    if(q->size == 0)
     {
       printf("queue is Empty\n");
       return 1;
     } 
     delete(q->list,q->list->heads[q->list->vr]->value,false);
     q->size = q->size-1;
     return 0; }
    return 1;
}
int front_queue(queue* q, int vr)
{
  if(q!=NULL){
  node* tmp = front(q->list,vr);
  if(tmp != NULL)
    return tmp->value;
    }
  printf("queue Empty/UnInitialized At version %d\n",vr);
  return INT_MIN;
}
void print_queue(queue* q, int vr)
{
   print_list(q->list,vr);

}


int main()
{
  queue* st;
  init_queue(st);
  int t,i,j,k,n,v=-1;
  printf("Enter the no of opearations you want to perform\n");
   scanf("%d", &n);
   printf(" 1. enqueue \n 2. dequeue \n 3. front \n 4. print\n");
   for(int it=0;it<n;it++)
   {
     scanf("%d", &i);
      if(i==1)
      { 
           printf("Enter the number to enqueue\n");
             scanf("%d", &j);
          enqueue(st,j);
      }
      else if(i==2)
      {
        dequeue(st);
      } 
      else if(i==3)
      {
           printf("Enter the version\n");
          scanf("%d", &j);
          int t= front_queue(st,j);
          if(t != INT_MIN)
           printf("%d\n",t);
      }
      else if(i==4)
      {
           printf("Enter the version\n");
          scanf("%d", &j);
          print_queue( st, j);
      }
    
   }
}

 
