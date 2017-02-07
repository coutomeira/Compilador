// AST definitions
#ifndef __ast_h__
#define __ast_h__
#define MAX 100
// AST for expressions
typedef struct _Expr 
{
  enum 
  { 
    E_INTEGER,
    E_VARIABLE,
    E_OPERATION,
    E_BOOLEAN,
    E_INPUT,
    E_BRACKETS
  } kind;

  union 
  {
    int value; // E_INTEGER
    
    char var[MAX]; // E_VARIABLE

    //char input[MAX]; //E_INPUT
    
    struct // E_OPERATION
    { 
      int operator; 
      struct _Expr* left;
      struct _Expr* right;
    } op;

    struct // E_BRACKETS
    {
      struct _Expr* expr;  
    } brackets;

  } attr;
} Expr;

// AST for commands
typedef struct _cmd
{
  enum
  {
    E_ATRIB,
    E_IF,
    E_WHILE,
    E_FOR, 
    E_OUTPUT,
  } kind;
  
  union
  {
    struct //E_ATRIB
    {
      char *var;
      struct _Expr* exp;
    } atrib;
    
    struct //E_IF
    {
      struct _Expr* cond;
      struct _cmdList* bodyIf;
      struct _elseIfList* elseIfList;
      struct _cmdList* bodyElse;
    } iff;

    struct //E_WHILE
    {
      struct _Expr* cond;
      struct _cmdList* body;
    } whilee;

    struct //E_FOR
    {
      struct _cmd* decl;
      struct _Expr* cond;
      struct _cmdList* body;
    } forr;
    
    Expr* output; //E_OUTPUT

  } attr;
} cmd;

typedef struct _cmdList
{
  struct _cmd* head;
  struct _cmdList* tail;
}cmdList;

typedef struct _elseIf
{
  Expr* cond;
  cmdList* body;
} elseIf;

typedef struct _elseIfList
{
  struct _elseIf* head;
  struct _elseIfList* tail;
}elseIfList;



  

// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_variable(char *v);
Expr* ast_operation(int operator, Expr* left, Expr* right);
Expr* ast_boolean(int operator, Expr* left, Expr* right);
Expr* ast_brackets (Expr* expression);
Expr* ast_input();


cmd* makeIf(Expr* cond, cmdList* bodyIf, elseIfList* elseIfList, cmdList* bodyElse);
elseIf* makeElseIf(Expr* cond, cmdList* body);
elseIfList* makeElseIfList(elseIf* head, elseIfList* tail);

cmd* makeAtrib(char *var, Expr* e);
cmd* makeWhile(Expr* cond, cmdList* body);
cmd* makeFor(cmd* decl, Expr* cond, cmdList* body);
cmd* makeOutput(Expr* e);

cmdList* makeList(cmd* head, cmdList* tail);

#endif
