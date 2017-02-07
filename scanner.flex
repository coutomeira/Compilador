%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
#.*\n { yyline++; }
#.* { yyline++; }

[\n] { yyline++; }
[ \t]+ { }


"if" 	 { return IFF; }
"else" 	 { return ELSEE; }
"elseif" { return ELSEEIFF; }
"while"  { return WHILEE; };
"for" 	 { return FORR;}
"end" 	 { return END; }
"true" 	 { return TRUE; }
"false"  { return FALSE; }
"+" 	 { return PLUS; }
"-" 	 { return MINUS; }
"*" 	 { return MUL; }
"/" 	 { return DIV; }
"%" 	 { return MOD; }
"=" 	 { return ATRIB; }
"==" 	 { return EQUAL; }
">" 	 { return GREATER; }
"<" 	 { return LESS; }
">=" 	 { return GREATER_EQUAL; }
"<=" 	 { return LESS_EQUAL; }
"~=" 	 { return NOT_EQUAL; }
";" 	 { return FINAL; }
":"	 { return COLON; }
"(" 	 { return OPEN; }
")" 	 { return CLOSE; }
"disp" 	 { return OUTPUT; }
"input"  { return INPUT; }

[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

[a-z,A-Z]+ {
   yylval.varval = strdup(yytext);
   return VAR;
}

.  { yyerror("unexpected character"); }
%%

