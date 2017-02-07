#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include "threeAddress.h"

Instr* makeInstr(opkind op, Address *a1, Address *a2, Address *a3)
{
  Instr* node = (Instr*) malloc(sizeof(Instr));
  node->opkind = op;
  node->address1 = a1;
  node->address2 = a2;
  node->address3 = a3;
  return node;
}

Address* makeInt(int i)
{
   Address* node = (Address*) malloc(sizeof(Address));
   node->kind = C_INT;
   node->content.val = i;
   return node;
}

Address* makeVar(char* c)
{
   Address* node = (Address*) malloc(sizeof(Address));
   node->kind = C_VAR;
   strcpy(node->content.var,c);
   return node;
}

Address* makeEmpty()
{
   Address* node = (Address*) malloc(sizeof(Address));
   node->kind = C_EMPTY;
   return node;
}

List* makeLinkedList(Instr* head, List *tail)
{
  List *node;
  node = (List *)malloc(sizeof(List));
  node->inst = head;
  node->next = tail;
  return node;
}

Pair* makePair(Address *a, List *l)
{
  Pair *pair;
  pair = (Pair *)malloc(sizeof(Pair));
  pair->address = a;
  pair->List = l;
  return pair;
}

Instr* head(List *l){
  if(l != NULL) return (l->inst);
}

List* tail(List *l){
  if(l != NULL) return (l->next);
}

List* append(List *l1, List *l2){
  List *p;
 
  if(l1 == NULL) return l2;
  if(l2 == NULL) return l1;
  
  for(p=l1; tail(p)!=NULL; p=tail(p));//MOVE P
  p->next = l2;
  return l1;
}


Stack* makeStack(char *v, Stack* stack)
{
  Stack* node = (Stack*) malloc(sizeof(Stack));

  node->var = v;
  node->next = stack;
  return node;
}

int isInStack(char *v, Stack* stack)
{
  if(stack == NULL)
    return 0;

  if(strcmp(v,stack->var) == 0)
  {
    return 1;
  }

  while(stack != NULL)
    {
      if(strcmp(v,stack->var) == 0)
        return 1;
      stack = stack->next;
    }
    return 0;
}

List* addInstrToList(List* list, Instr* inst)
{
  List *listTemp = makeLinkedList(inst, NULL);
  return append(list, listTemp); 
}

Instr* getLastInstr(List* list)
{
  while(list->next != NULL)
    {
      list=list->next;
    }
  return list->inst;
}

List* changeLastInstr(List* list, Instr* inst)
{
  List *newList = list;
  while(list->next!=NULL)
    {
      list = list->next;
    }
  list->inst = inst;
  return newList;
}
