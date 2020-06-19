#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define MAX 500
typedef struct node {
  int val;
  struct node *left, *right,*leftx,*rightx;
  int l_ts,r_ts;
} node;

node *create_node(int val, int l_ts, int r_ts, node *left, node* right, node* leftx, node* rightx) {
  node *tmp = (node *)malloc(sizeof(node));
  tmp->val = val;
  tmp->left = left;
  tmp->right = right;
  tmp->l_ts = l_ts;
  tmp->r_ts = r_ts;
  tmp->leftx = leftx;
  tmp->rightx = rightx;
  return tmp;
}

typedef struct pbst
{
   node** roots;
   int size,vr;
}pbst;

pbst* create_pbst(node** roots, int size, int vr)
{
    pbst *tmp = (pbst *)malloc(sizeof(pbst));
    tmp->roots = roots;
    tmp->size = size;
    tmp->vr = vr;
    return tmp;
}

node* get_left(node* t)
{
  if(t!=NULL)
     return t->l_ts == INT_MAX?t->left:t->leftx;
}

node* get_right(node* t)
{
  if(t!=NULL)
     return t->r_ts == INT_MAX?t->right:t->rightx;
}


node* search(pbst* bst, int val, int vr)
{
  if(bst == NULL || bst->roots == NULL)
    return NULL;
  node* tmp = bst->roots[vr];
  while(tmp!=NULL && tmp->val != val)
  {
      if(val>tmp->val)
        {
            if(vr>=tmp->r_ts)
               tmp = tmp->rightx;
            else
               tmp = tmp->right;  
        }
       else
       {
            if(vr>=tmp->l_ts)
               tmp = tmp->leftx;
            else
               tmp = tmp->left;  
       }
  }
   return tmp;
}


node* insert_util(node* root, int val, int ts)
{
   if(root == NULL)
      return create_node(val,INT_MAX,INT_MAX,NULL,NULL,NULL,NULL);
    node* tmp;
  
     if(val>root->val)
        {
            if(ts>=root->r_ts){
                 tmp = insert_util(root->rightx, val,ts);
                  if(tmp!=NULL)
                   return create_node(root->val,INT_MAX,INT_MAX,get_left(root),tmp ,NULL,NULL);
                  return NULL;
                }
            else{
                  tmp =  insert_util(root->right, val,ts);
                  if(tmp!=NULL){
                    root->rightx = tmp;
                    root->r_ts = ts;}
                  return NULL;
               }
        }
       else
       {
            if(ts>=root->l_ts){
              tmp = insert_util(root->leftx, val,ts);
              if(tmp!=NULL)
                return create_node(root->val,INT_MAX,INT_MAX,tmp,get_right(root),NULL,NULL);
               return NULL;
              }
            else{
                 tmp = insert_util(root->left, val,ts);
                 if(tmp!=NULL)
                  {
                    root->leftx = tmp;
                    root->l_ts = ts;
                  }
                 return NULL;
              }
       }

}


int insert(pbst* bst, int val)
{
   if(bst != NULL) {
    ++bst->vr;
    int ts = bst->vr;
    node* tmp;
    if(bst->roots == NULL)
    {
      bst->roots = (node**)malloc(MAX*sizeof(node*));
     for(int it = 0;it < MAX; it++)
       bst->roots[it] = NULL;
     bst->roots[ts] = create_node(val,INT_MAX,INT_MAX,NULL,NULL,NULL,NULL);
     bst->size = 1;
     return 0;
    }
     if((tmp = insert_util(bst->roots[ts-1],val,ts)))
         bst->roots[ts] = tmp; 
     else
         bst->roots[ts] = bst->roots[ts-1];  
     bst->size = bst->size + 1; 
     return 0; 
    }
    return 1;
}

node* max(node* root)
{
   node* r = get_right(root);
   if(r == NULL)
     return root;
}



node* delete_util(node* root, int val, int ts, int* found)
{
   if(root == NULL)
     return NULL;
    node* tmp;
    node *l,*r;
    if( root->val < val)
    {
       r = get_right(root);
       tmp = delete_util(r,val,ts,found);
       if(tmp || (*found ==1) && ( r->val == val))
        {
          if(root->r_ts != INT_MAX)
            return  create_node(root->val,INT_MAX,INT_MAX,get_left(root),tmp,NULL,NULL);
         else
          {
              root->rightx = tmp;
              root->r_ts = ts+1;
              return NULL;
          }
        }
        return NULL;

    }
    if(root->val > val)
    {
        l = get_left(root);
       tmp = delete_util(l,val,ts,found);
       if(tmp || (*found ==1) && ( l->val == val))
        {
          if(root->l_ts != INT_MAX)
            return  create_node(root->val,INT_MAX,INT_MAX,tmp,get_right(root),NULL,NULL);
         else
          {
              root->leftx = tmp;
              root->l_ts = ts+1;
              return NULL;
          }
        }
        return NULL;
    }
  
       *found = 1;
       l = get_left(root);
       r = get_right(root);
       if(l&&r)
       {
         node* t = max(l);
         node* new = create_node(t->val,INT_MAX,INT_MAX,l,r,NULL,NULL);
         int f=0;
         node* x = delete_util(l,t->val,ts,&f);
         if(x || l->val == t->val)
           new->left = x;
         return new;
       }
       if(l||r)
       {
          if(l)
            return l;
          return r;     
       }
     return NULL;
   
}



int delete(pbst* bst, int val)
{
   if(bst == NULL || bst->roots == NULL)
      return 1;
   int ts = bst->vr,found = 0;
   node* tmp = delete_util(bst->roots[ts],val,ts,&found);
   if(!found)
      return 1;
   if( tmp || bst->roots[ts]->val== val)
        bst->roots[ts+1] = tmp;
   else
       bst->roots[ts+1] = bst->roots[ts];
   bst->size = bst->size - 1;
   ++bst->vr;
   return 0;
}

void inorder_bst(node* root, int vr,pbst* bst)
{
  if(root == NULL)
    return ;
  inorder_bst(vr>=root->l_ts?root->leftx:root->left, vr,bst);
  printf("%d ", search(bst,root->val,vr)->val);
  inorder_bst(vr>=root->r_ts?root->rightx:root->right,vr,bst);
}



int main()
{
   int t,i,j,k,n,v=-1;
   pbst* bst;
   bst = create_pbst(NULL,0,-1);
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
        if(search(bst,j,k))
          printf("found\n");
        else
          printf("Not found\n");
      }
      else if(i==2)
      {
        printf("Enter the number to insert\n");
        scanf("%d", &j);
        insert(bst,j);
      }
      else if(i==3)
      {
        printf("Enter the number to delete\n");
        scanf("%d", &j);
        delete(bst,j);
      }
      else if(i==4)
      {
        printf("Enter the version\n");
        scanf("%d", &j); 
        inorder_bst(bst->roots[j],j,bst);
        printf("\n");
      }
   }
}








