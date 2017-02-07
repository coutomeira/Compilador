// threeAddress definitions
#ifndef __threeAddress_h__
#define __threeAddress_h__
#define MAX 100
//INSTR = (TYPEINSTR, A1, A2, A3)

//TYPE OF INSTR
typedef enum 
{ 
    C_PLUS,
    C_MINUS,
    C_MUL,
    C_DIV,
    
    C_IF,
    C_GOTO,
    
    C_SEQ,
    C_SNE,
    C_SGT,
    C_SGE,
    C_SLT,
    C_SLE,
    
    C_BEQ,
    C_BNE,
    C_BGT,
    C_BGE,
    C_BLT,
    C_BLE,

    C_MOVE,
    C_SW,
    C_LABEL,
    C_LI,
    C_LW,
    C_SYSCALL
}opkind;

//TYPE OF A[123]
typedef enum
{
  C_INT,
  C_VAR,
  C_EMPTY
}AddressKind;

//A[123]
typedef struct
{
  AddressKind kind;

  union
  {
    int val;
    char var[MAX];
  }content;

}Address;

//INSTR
typedef struct 
{
  opkind opkind; 
  Address *address1;
  Address *address2;
  Address *address3;
}Instr;

typedef struct _List
{
  Instr* inst;
  struct _List *next;
}List;

typedef struct
{
  Address *address;
  List *List;
}Pair;

typedef struct _Stack
{
  char *var;
  struct _Stack *next;
} Stack;


Instr* makeInstr(opkind op, Address *a1, Address *a2, Address *a3);

Address* makeEmpty();
Address* makeVar(char* c);
Address* makeInt(int i);

List* makeLinkedList(Instr *head, List* tail);
List* append(List* l1, List* l2);
Pair* makePair(Address *a, List *l);

Stack* makeStack(char *v, Stack* stack);
int isInStack(char *v, Stack* stack);

List* addInstrToList(List* list, Instr* inst);
Instr* getLastInstr(List* list);
List* changeLastInstr(List* list, Instr* inst);

#endif
