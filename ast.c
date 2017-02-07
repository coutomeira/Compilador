// AST constructor functions
#include <string.h>
#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr* ast_integer(int v) 
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_operation (int op, Expr* l, Expr* r) 
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = op;
  node->attr.op.left = l;
  node->attr.op.right = r;
  return node;
}

Expr* ast_brackets (Expr* e) 
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_BRACKETS;
  node->attr.brackets.expr = e;
  return node;
}

Expr* ast_boolean (int op, Expr* l, Expr* r) 
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_BOOLEAN;
  node->attr.op.operator = op;
  node->attr.op.left = l;
  node->attr.op.right = r;
  return node;
}

Expr* ast_variable(char *v)
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  strcpy(node->attr.var,v);
  return node;
}

Expr* ast_input()
{
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INPUT;
  return node;       
}

cmdList* makeList(cmd* h, cmdList* t)
{
  cmdList* node = (cmdList*) malloc(sizeof(cmdList));
  node->head = h;
  node->tail = t;
  return node;	
}

cmd* makeAtrib(char *v, Expr* e)
{
  cmd* node = (cmd*) malloc(sizeof(cmd));
  node->kind = E_ATRIB;
  node->attr.atrib.var = v;
  node->attr.atrib.exp = e;
  return node;		
}

cmd* makeWhile(Expr* c, cmdList* b)
{
  cmd* node = (cmd*) malloc(sizeof(cmd));
  node->kind = E_WHILE;
  node->attr.whilee.cond = c;
  node->attr.whilee.body = b;
  return node;     
}

cmd* makeFor(cmd* d, Expr* c, cmdList* b)
{
  cmd* node = (cmd*) malloc(sizeof(cmd));
  node->kind = E_FOR;
  node->attr.forr.decl = d; 
  node->attr.forr.cond = c;
  node->attr.forr.body = b;
  return node;     
}

cmd* makeOutput(Expr* e)
{
  cmd* node = (cmd*) malloc(sizeof(cmd));
  node->kind = E_OUTPUT;
  node->attr.output = e; 
  return node;      
}

cmd* makeIf(Expr* c, cmdList* bIf, elseIfList* eIfL, cmdList* bE)
{
  cmd* node = (cmd*) malloc(sizeof(cmd));
  node->kind = E_IF;
  node->attr.iff.cond = c; 
  node->attr.iff.bodyIf = bIf;
  node->attr.iff.elseIfList = eIfL;
  node->attr.iff.bodyElse = bE;
  return node;  
}

elseIf* makeElseIf(Expr* c, cmdList* b)
{
  elseIf* node = (elseIf*) malloc(sizeof(elseIf));
  node->cond = c;
  node->body = b;
  return node;     
}

elseIfList* makeElseIfList(elseIf* h, elseIfList* t)
{
  elseIfList* node = (elseIfList*) malloc(sizeof(elseIfList));
  node->head = h;
  node->tail = t;
  return node;  
}

