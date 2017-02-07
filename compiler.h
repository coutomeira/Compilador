#include <stdio.h>
#include <string.h>

int count1=0, count2=1;
Stack* stack = NULL;


char* newTemp()
{
  if (count1>9)
    {
      int temp = count1-10;
      if(temp>7)
	{
	  count1=0;
	  return newTemp();
	}
      char *foo = malloc(sizeof(int)+2);   
      sprintf(foo, "$s%d", temp);
      count1++;
      return foo;
    }
  
  char *foo = malloc(sizeof(int)+2);   
  sprintf(foo, "$t%d",  count1);
  count1++;
  return foo;
}

char* newLabel()
{
  char *foo = malloc(sizeof(int)+5);   
  sprintf(foo, "label%d",  count2);
  count2++;
  return foo;
}

opkind transformOpIf(int op)
{
 switch (op) 
   {
   case EQUAL: 
     return C_BNE; 
   case NOT_EQUAL: 
     return C_BEQ;  
   case GREATER: 
     return C_BLE;  
   case LESS: 
     return C_BGE;
   case GREATER_EQUAL: 
     return C_BLT;  
   case LESS_EQUAL: 
     return C_BGT;  
   }
} 

opkind transformOpExp(int op)
{
 switch (op) 
    {
    case PLUS: 
      return C_PLUS;  
    case MINUS: 
      return C_MINUS;
    case MUL:
      return C_MUL;    
    case DIV:
      return C_DIV;
    case EQUAL: 
      return C_SEQ; 
    case NOT_EQUAL: 
      return C_SNE;  
    case GREATER: 
      return C_SGT;  
    case LESS: 
      return C_SLT;
    case GREATER_EQUAL: 
      return C_SGE;  
    case LESS_EQUAL: 
      return C_SLE;  
    }
} 

Pair* compileExpr(Expr* expr) 
{
  if (expr->kind == E_INTEGER) 
    {    
      return makePair(makeInt(expr->attr.value), NULL); 
    }
  
  else if (expr->kind == E_VARIABLE) 
    {
      Pair *p;
      List *list = NULL;
      Address *address = makeVar(newTemp());
      Instr *inst;

      if(!isInStack(expr->attr.var, stack))
	stack = makeStack(expr->attr.var, stack);
     
      inst = makeInstr(C_LW, address, makeVar(expr->attr.var), makeEmpty());
      list = addInstrToList(list, inst);
    
      return makePair(address, list);
    }
    
  else if (expr->kind == E_BRACKETS)
    {
      compileExpr(expr->attr.brackets.expr);
    }  

  else if (expr->kind == E_OPERATION || expr->kind == E_BOOLEAN) 
    {
      Pair *left, *right;
      left = compileExpr(expr->attr.op.left);
      right = compileExpr(expr->attr.op.right);

      if(left->address->kind == C_INT)
	{
	  Instr *newInst = makeInstr(C_LI, makeVar(newTemp()), left->address, makeEmpty());
	  left->address = newInst->address1;
	  left->List = addInstrToList(left->List, newInst);
	}
      
      if(right->address->kind == C_INT)
	{
	  Instr *newInst = makeInstr(C_LI, makeVar(newTemp()), right->address, makeEmpty());
	  right->address = newInst->address1;
	  right->List = addInstrToList(right->List, newInst);
	}
      
      char *var = newTemp();
      Instr *inst = makeInstr(transformOpExp(expr->attr.op.operator), makeVar(var), left->address, right->address);
      List *list = append(left->List, right->List);

      List *finalList = addInstrToList(list, inst);
      return makePair(makeVar(var), finalList);
    }
  
  else if(expr->kind == E_INPUT)
    {
      List *list=NULL;
      Instr *inst;
      
      //LI para v0
      inst = makeInstr(C_LI, makeVar("$v0"), makeInt(5), makeEmpty());
      list = addInstrToList(list,inst);
      //Chamada ao sistema
      inst = makeInstr(C_SYSCALL, makeEmpty(), makeEmpty(), makeEmpty());
      list = addInstrToList(list,inst);
      //Transferir
      inst = makeInstr(C_MOVE, makeVar(newTemp()), makeVar("$v0"), makeEmpty());
      list = addInstrToList(list,inst);
     
      return makePair(inst->address1,list);
    }    
  
}

List* compileCmd(cmd* command)
{ 
  if (command->kind == E_ATRIB) 
    {
      Instr *inst;
      
      if(!isInStack(command->attr.atrib.var, stack))
	stack = makeStack(command->attr.atrib.var, stack);

      Pair* p = compileExpr(command->attr.atrib.exp);

      if(p->address->kind == C_INT)
	{
	  inst = makeInstr(C_LI, makeVar(newTemp()), p->address, makeEmpty());
	  p->address = inst->address1;
	  p->List = addInstrToList(p->List,inst);
	}
      
      inst = makeInstr(C_SW,p->address,makeVar(command->attr.atrib.var), makeEmpty());
      p->List = addInstrToList(p->List, inst);
      
      return p->List;
    } 
  
  else if(command->kind == E_IF)
    {
      List *res=NULL;
      List *list;
      Pair *p;
      Address *endLabel = makeVar(newLabel());
      Address *nextLabel;
      Instr *goToInst =  makeInstr(C_GOTO, endLabel, makeEmpty(), makeEmpty());
      Instr *labelInst, *lastInst, *newInst;
      Expr *cond = command->attr.iff.cond;
      
      p = compileExpr(cond);
      count1=0;//DECREMENTA
      lastInst = getLastInstr(p->List);
      nextLabel = makeVar(newLabel());
      newInst = makeInstr(transformOpIf(cond->attr.op.operator),lastInst->address2,lastInst->address3,nextLabel);
      p->List = changeLastInstr(p->List,newInst);
      res = append(res,p->List);
      
      cmdList* bodyIf = command->attr.iff.bodyIf; 
      
      while(bodyIf!=NULL)
	{ 
	  list = compileCmd(bodyIf->head);
	  res = append(res,list);
	  bodyIf = bodyIf->tail;
	}
      res = addInstrToList(res, goToInst); 
      
      elseIfList* elseIfList = command->attr.iff.elseIfList; 

      while(elseIfList!=NULL)
	{
	  labelInst = makeInstr(C_LABEL, nextLabel, makeEmpty(), makeEmpty());
	  res = addInstrToList(res, labelInst);
	  
	  elseIf* elseIf = elseIfList->head; 
	  
	  p = compileExpr(elseIf->cond);
	  count1=0;//DECREMENTA
	  lastInst = getLastInstr(p->List);
	  nextLabel = makeVar(newLabel());
	  newInst = makeInstr(transformOpIf(elseIf->cond->attr.op.operator),lastInst->address2,lastInst->address3,nextLabel);
	  p->List = changeLastInstr(p->List,newInst);
	  res = append(res,p->List);
	  
	  cmdList* bodyElseIf = elseIf->body;
	  
	  while(bodyElseIf != NULL)
	    { 
	      list = compileCmd(bodyElseIf->head);
	      res = append(res,list);
	      bodyElseIf = bodyElseIf->tail;
	    }
	  
	  res = addInstrToList(res, goToInst);
	  elseIfList = elseIfList->tail;  
	}
     
      cmdList* bodyElse = command->attr.iff.bodyElse;

      labelInst = makeInstr(C_LABEL, nextLabel, makeEmpty(), makeEmpty());
      res = addInstrToList(res, labelInst);

      if(bodyElse != NULL)
	{
	  while(bodyElse != NULL)
	    {
	      list = compileCmd(bodyElse->head);
	      res = append(res,list);
	      bodyElse = bodyElse->tail;
	    }
	}
      
      labelInst = makeInstr(C_LABEL, endLabel, makeEmpty(), makeEmpty());
      res = addInstrToList(res, labelInst);

      return res;
    }
  
  else if(command->kind == E_WHILE)
    {
      List *res=NULL;
      List *list;
      Pair *p;
      Instr *lastInst, *loopInst, *goToInst, *newInst;
      Address *loop = makeVar(newLabel());
      Address *endLoop = makeVar(newLabel());
      Expr *cond = command->attr.whilee.cond;
      
      loopInst =  makeInstr(C_LABEL, loop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, loopInst);
      
      p = compileExpr(command->attr.whilee.cond);
      count1=0;//DECREMENTA
      lastInst = getLastInstr(p->List);
      newInst = makeInstr(transformOpIf(cond->attr.op.operator),lastInst->address2,lastInst->address3,endLoop);
      p->List = changeLastInstr(p->List,newInst);
      res = append(res,p->List);
   
      cmdList* whileBody = command->attr.whilee.body; 
    
      while(whileBody!=NULL)
	{ 
	  list = compileCmd(whileBody->head);
	  res = append(res,list);
	  whileBody = whileBody->tail;
	}
      
      goToInst = makeInstr(C_GOTO, loop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, goToInst); 
      loopInst =  makeInstr(C_LABEL, endLoop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, loopInst);

      return res;
    }
  
  else if(command->kind == E_FOR)
    {
      List *res=NULL;
      List *list;
      Pair *p;
      Instr *forInst, *loopInst, *inst;
      Address *temp;
      Address *loop = makeVar(newLabel());
      Address *endLoop = makeVar(newLabel());
      Address *forDecl;//onde esta a decl do for
      Address *forCond;//onde esta a cond do for
      
      cmd *Decl =  command->attr.forr.decl;
      Address *var = makeVar(Decl->attr.atrib.var);
      
      if(!isInStack(Decl->attr.atrib.var, stack))
	stack = makeStack(Decl->attr.atrib.var, stack);
      
      p = compileExpr(Decl->attr.atrib.exp);
      count1=0;//DECREMENTA
      res = append(res,p->List);
      forDecl = p->address;

      if(forDecl->kind == C_INT)
	{
	  Address *newDecl = makeVar(newTemp());
	  forInst = makeInstr(C_LI, newDecl, forDecl, makeEmpty());
	  res = addInstrToList(res,forInst);
	  forDecl = newDecl;
	}
      //SW na variavel do forDecl
      inst = makeInstr(C_SW,forDecl,var,makeEmpty());
      res = addInstrToList(res,inst);

      //Label Inicial
      loopInst = makeInstr(C_LABEL, loop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, loopInst);

      p = compileExpr(command->attr.forr.cond);
      count1=0;//DECREMENTA
      res = append(res,p->List);
      forCond = p->address;

      temp = makeVar(newTemp());
      //LW da variavel
      loopInst = makeInstr(C_LW, temp, var, makeEmpty());
      res = addInstrToList(res, loopInst);

      //Inst para testar se chegou ao fim
      loopInst = makeInstr(C_BGT, temp, forCond, endLoop);
      res = addInstrToList(res, loopInst);

      cmdList* forBody = command->attr.forr.body; 
    
      while(forBody!=NULL)
	{ 
	  list = compileCmd(forBody->head);
	  res = append(res, list);
	  forBody = forBody->tail; 
      	}
      
      temp = makeVar(newTemp());
      //LW da variavel
      loopInst = makeInstr(C_LW, temp, var, makeEmpty());
      res = addInstrToList(res, loopInst);

      //Incremento
      loopInst = makeInstr(C_PLUS, temp, temp, makeInt(1));
      res = addInstrToList(res, loopInst);

      //SW na variavel do forDecl
      inst = makeInstr(C_SW, temp, var, makeEmpty());
      res = addInstrToList(res,inst);
      
      //Jump para o inicio
      loopInst = makeInstr(C_GOTO, loop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, loopInst);
      
      //Label de final
      loopInst = makeInstr(C_LABEL, endLoop, makeEmpty(), makeEmpty());
      res = addInstrToList(res, loopInst);
      
      return res;
    }
  
  else if(command->kind == E_OUTPUT)
    {
      List *res=NULL;
      Pair *p;
      Instr *inst;
      
      p = compileExpr(command->attr.output);
      if(p->address->kind == C_INT)
	{
	  inst = makeInstr(C_LI, makeVar(newTemp()), p->address, makeEmpty());
	  p->address = inst->address1;
	  p->List = addInstrToList(p->List, inst);
	}
      res = append(res,p->List);
      count1=0;//DECREMENTA
      
      //LI para v0
      inst = makeInstr(C_LI, makeVar("$v0"), makeInt(1), makeEmpty());
      res = addInstrToList(res,inst);
      //Colocar em a0
      inst = makeInstr(C_MOVE, makeVar("$a0"), p->address, makeEmpty());
      res = addInstrToList(res,inst);
      //Chamada ao sistema
      inst = makeInstr(C_SYSCALL, makeEmpty(), makeEmpty(), makeEmpty());
      res = addInstrToList(res,inst);

      return res;
    }  
}

List* compile(cmdList* root)
{
  List *list=NULL;
  List *newList=NULL;
  Instr *inst;
  
  while(root != NULL)
    {
      newList = compileCmd(root->head);
      count1=0;//DECREMENTA
      list = append(list, newList); 
      root = root->tail;
    }

  //LI para v0
  inst = makeInstr(C_LI, makeVar("$v0"), makeInt(10), makeEmpty());
  list = addInstrToList(list,inst);
  //Chamada ao sistema
  inst = makeInstr(C_SYSCALL, makeEmpty(), makeEmpty(), makeEmpty());
  list = addInstrToList(list,inst);
  
  return list;
}

Stack* getGlobalData()
{
  return stack;
}

