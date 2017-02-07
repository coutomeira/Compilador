#include "parser.h"
#include "threeAddress.h"
#include "printer.h"
#include "compiler.h"


int main(int argc, char** argv) 
{
  --argc; ++argv;
  if (argc != 0) 
  {
    yyin = fopen(*argv, "r");
    if (!yyin) 
    {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } 

  if (yyparse() == 0) 
  {
    List *resList = compile(root);
    Stack *stack = getGlobalData();
    print(stack, resList);
  }
  
  return 0;
}
