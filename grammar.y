%{
	#include "node.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include "listADT.h"

	extern void yyeror(char* text);

	listADT symbolList;
%}

%union {
	Node n;
}

%token <n> NAME
%token <n> FLOAT
%token <n> INTEGER
%token <n> STRING
%left '-' '+'
%right '*' '/'
%nonassoc UMINUS
%type <n> expression

%%

statement_list: statement '\n'
			  | statement_list statement '\n'
			  ;

statement: NAME '=' expression 
				{ 
					Node n = (Node) getL(symbolList, $1->name);
					if(n != 0)
					{
						free(n->value);
						n->type = $3->type;
						assignValue(n, $3->value);
					}
					else
					{
						addL(symbolList, newNode($1->name, $3->type, $3->value));
					}
				}
		 | expression { printByValue(*$1); }
		 ;

expression: expression '+' expression 
				{
					$$ = binaryOperation($1, $3, addition);
					free($1->value);
					free($1);
					free($3->value);
					free($3);
				}
		  | expression '-' expression 
		  		{
		  			$$ = binaryOperation($1, $3, subtraction); 
					free($1->value);
					free($1);
					free($3->value);
					free($3);
		  		}
		  | expression '*' expression 
		  		{
		  			$$ = binaryOperation($1, $3, multiplication);
					free($1->value);
					free($1);
					free($3->value);
					free($3);
		  		}
		  | expression '/' expression 
		  		{ 
		  			$$ = binaryOperation($1, $3, division);
					free($1->value);
					free($1);
					free($3->value);
					free($3);
		  		}
		  | '-' expression %prec UMINUS 
		  		{ 
		  			$$ = UMinusByType($2);
					free($2->value);
					free($2);
		  		}
		  | '(' expression ')' 
		  		{ 
		  			$$ = malloc(sizeof(node));
		  			$$->type = $2->type;
		  			assignValue($$, $2->value);
					free($2->value);
					free($2);
		  		}
		  | FLOAT
		  | INTEGER
		  | STRING
		  | NAME { $$ = (Node) getL(symbolList, $$->name); }
		  ;

%%

int main(int argc, char **argv)
{
	symbolList = createListL(cmpFunction, sizeof(node));
	yyparse();
}
