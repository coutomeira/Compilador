#include <stdio.h>
#include <string.h>

FILE *file;

void printAddress(Address *a)
{
  switch(a -> kind)
    {
    case C_INT:
      fprintf(file,"%8d", a->content.val);
      break;
    case C_VAR:
      fprintf(file,"%8s", a->content.var);
      break;
    case C_EMPTY:
      fprintf(file,"%8s","");
      break;
    }
}

void printWord(char *var)
{
  fprintf(file,"%8s%-8s","", var);
}

void printInstr(Instr *inst)
{
  switch (inst->opkind) 
    {
    case C_LABEL:
      fprintf(file,"%7s:\n",inst->address1->content.var);
      return;
    case C_GOTO:
      fprintf(file,"%8sj %s\n\n","", inst->address1->content.var);
      return;
    case C_SYSCALL:
      fprintf(file,"%8ssyscall\n","");
      return;
    case C_PLUS: 
      printWord("add");  
      break;
    case C_MINUS: 
      printWord("sub");
      break;
    case C_DIV: 
      printWord("div");
      break;       
    case C_MUL: 
      printWord("mul");
      break;
    case C_SW:
      printWord("sw");
      break;
    case C_BEQ:
      printWord("beq");
      break;
    case C_BNE:
      printWord("bne");
      break;
    case C_BGT:
      printWord("bgt");
      break;
    case C_BGE:
      printWord("bge");
      break;
    case C_BLT:
      printWord("blt");
      break;
    case C_BLE:
      printWord("ble");
      break;
    case C_SEQ:
      printWord("seq");
      break;
    case C_SNE:
      printWord("sne");
      break;
    case C_SGT:
      printWord("sgt");
      break;
    case C_SGE:
      printWord("sge");
      break;
    case C_SLT:
      printWord("slt");
      break;
    case C_SLE:
      printWord("sle");
      break;
    case C_LI:
      printWord("li");
      break;
    case C_MOVE:
      printWord("move");
      break;
    case C_LW:
      printWord("lw");
      break;
    default:
      printWord("def");
    }

  printAddress(inst->address1);
  fprintf(file,",");
  printAddress(inst->address2);
  if(inst->opkind == C_LI || inst->opkind == C_LW || inst->opkind == C_SW || inst->opkind == C_MOVE)
    {
      fprintf(file,"\n");
      return;
    }
  fprintf(file,",");
  printAddress(inst->address3);
  fprintf(file,"\n");
}

void printList(List *list)
{
  if(list != NULL)
    fprintf(file,"\n%8s\n\n",".text");
  
  while(list != NULL)
    {
      printInstr(list->inst);
      list = list->next;
    }
  fprintf(file,"\n");
}

void printStack(Stack *stack)
{
  if(stack != NULL)
    fprintf(file, "\n%8s\n\n",".data");
  
  while(stack != NULL)
    {
      fprintf(file,"%8s%s:\t\t.space  4\n","",stack->var);
      stack = stack->next;
    }
}

void print(Stack *stack, List *list)
{
  file = fopen("mips.txt","w");
  
  printStack(stack);
  printList(list);

  fclose(file);
}
