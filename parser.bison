
// Tokens
%token 
  INT  		
  PLUS 		
  MINUS		
  MUL		
  DIV		
  ATRIB
  MOD		
  FINAL		
  COLON		
  VAR 			
  EQUAL		
  NOT_EQUAL     
  GREATER	
  LESS		
  GREATER_EQUAL	
  LESS_EQUAL	
  IFF	         
  ELSEE		 
  ELSEEIFF      
  END		
  WHILEE	 
  FORR		 
  TRUE		
  FALSE		
  OPEN		
  CLOSE		
  INPUT  
  OUTPUT  	

// Operator associativity & precedence
%left EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left MUL DIV
%precedence NEG

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union 
{
  int intValue;
  Expr* exprValue;                
  char *varval;
  cmdList* cmdList_;                                         
  cmd* cmd_;
  elseIf* elseIf_;
  elseIfList* elseIfList_; 
}

%type <intValue> INT;
%type <exprValue> expr;
%type <exprValue> exprBool;
%type <varval> VAR;
%type <cmdList_> cmdList;
%type <cmd_> cmd;
%type <cmd_> forCond;
%type <elseIf_> elseIf;
%type <elseIfList_> elseIfList;

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires 
{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"

  extern int yylex();
  extern int yyline;
  extern char* yytext;
  extern FILE* yyin;
  extern void yyerror(const char* msg);
  cmdList* root;
}

%%
program: cmdList { root = $1; }                         


cmdList: 
  	  cmd cmdList			     			{ $$ = makeList($1,$2); }

  	| cmd              		     			{ $$ = makeList($1,NULL); }

elseIfList:
	  elseIf elseIfList		     			{ $$ = makeElseIfList($1, $2); }

	| elseIf			     			{ $$ = makeElseIfList($1, NULL); }

elseIf:
	  ELSEEIFF exprBool cmdList          			{ $$ = makeElseIf($2, $3); }

cmd:
	  VAR ATRIB expr FINAL               			{ $$ = makeAtrib($1,$3); } 

	| VAR ATRIB exprBool FINAL           			{ $$ = makeAtrib($1,$3); } 

 	| IFF exprBool cmdList END        		        { $$ = makeIf($2, $3, NULL, NULL); }

        | IFF exprBool cmdList ELSEE cmdList END                { $$ = makeIf($2,$3,NULL, $5); }

  	| IFF exprBool cmdList elseIfList END     		{ $$ = makeIf($2, $3, $4, NULL); }

  	| IFF exprBool cmdList elseIfList ELSEE cmdList END     { $$ = makeIf($2, $3, $4, $6); }
	
 	| WHILEE exprBool cmdList END       			{ $$ = makeWhile($2, $3); }

 	| FORR forCond COLON expr cmdList END			{ $$ = makeFor($2, $4, $5); }

  	| OUTPUT OPEN expr CLOSE FINAL       			{ $$ = makeOutput($3); }

expr:
	  OPEN expr CLOSE					{ $$ = ast_brackets($2); }

	| INT				     			{ $$ = ast_integer($1); } 

	| MINUS INT %prec NEG                                   { $$ = ast_integer(-$2); } 

	| TRUE			             			{ $$ = ast_integer(1); }  

	| FALSE				     			{ $$ = ast_integer(0); }  

 	| VAR 			             			{ $$ = ast_variable($1); }

	| INPUT OPEN CLOSE	   				{ $$ = ast_input(); }
	
 	| expr PLUS expr              	     			{ $$ = ast_operation(PLUS, $1, $3); }

 	| expr MINUS expr  		     			{ $$ = ast_operation(MINUS, $1, $3); } 
  
 	| expr MUL expr		     			        { $$ = ast_operation(MUL, $1, $3); }

 	| expr DIV expr 	 	     			{ $$ = ast_operation(DIV, $1, $3); }

 	| expr MOD expr 	 	     			{ $$ = ast_operation(MOD, $1, $3); }

 	| expr EQUAL expr  		     			{ $$ = ast_operation(EQUAL, $1, $3); }

 	| expr NOT_EQUAL expr 		     			{ $$ = ast_operation(NOT_EQUAL, $1, $3); }

 	| expr GREATER expr 	 	     			{ $$ = ast_operation(GREATER, $1, $3); }

 	| expr LESS expr 	 	     			{ $$ = ast_operation(LESS, $1, $3); }

 	| expr GREATER_EQUAL expr 	     			{ $$ = ast_operation(GREATER_EQUAL, $1, $3); }

 	| expr LESS_EQUAL expr 		     			{ $$ = ast_operation(LESS_EQUAL, $1, $3); };


exprBool:
  	  OPEN exprBool CLOSE					{ $$ = ast_brackets($2); }

	| expr EQUAL expr  		     			{ $$ = ast_boolean(EQUAL, $1, $3); }

 	| expr NOT_EQUAL expr 		     			{ $$ = ast_boolean(NOT_EQUAL, $1, $3); }

 	| expr GREATER expr 	 	     			{ $$ = ast_boolean(GREATER, $1, $3); }

 	| expr LESS expr 	 	     			{ $$ = ast_boolean(LESS, $1, $3); }

 	| expr GREATER_EQUAL expr 	     			{ $$ = ast_boolean(GREATER_EQUAL, $1, $3); }

 	| expr LESS_EQUAL expr 		     			{ $$ = ast_boolean(LESS_EQUAL, $1, $3); }

	| INT				     			{ $$ = ast_boolean(NOT_EQUAL,ast_integer($1),ast_integer(0)); } 

	| MINUS INT %prec NEG                                   { $$ = ast_boolean(NOT_EQUAL,ast_integer(-$2),ast_integer(0)); } 

	| TRUE			             			{ $$ = ast_boolean(NOT_EQUAL,ast_integer(1),ast_integer(0)); }  

	| FALSE				     			{ $$ = ast_boolean(NOT_EQUAL,ast_integer(0),ast_integer(0)); }  

 	| VAR 			             			{ $$ = ast_boolean(NOT_EQUAL,ast_variable($1),ast_integer(0)); }

	| INPUT OPEN CLOSE	   				{ $$ = ast_boolean(NOT_EQUAL,ast_input(),ast_integer(0)); };

forCond:
	  VAR ATRIB expr		     			{ $$ = makeAtrib($1,$3); } 
%%



void yyerror(const char* err) 
{
    printf("Line %d: %s - '%s'\n", yyline, err, yytext);
}

