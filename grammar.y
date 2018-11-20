%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "listADT.h"
	#include "node.h"

	extern void yyeror(char* text);

	void printByValue(node n);

	double vbltable[26];

	listADT symbolList;
%}

%union {
	node n;
}

%token <node> NAME
%token <node> NUMBER
%left '-' '+'
%right '*' '/'
%nonassoc UMINUS
%type <node> expression

%%

statement_list: statement '\n'
			  | statement_list statement '\n'
			  ;

statement: NAME '=' expression 
				{ 
					Node n = (Node)getL(symbolList, $1.name);
					if(n != 0)
					{
						free(n->value);
						n->type = $3.type;
						assignValue(n, $3.value);
					}
					else
					{
						addL(symbolList, newNode($1.name, $3.type, $3.value));
					}
				}
		 | expression { printByValue($1); }
		 ;

expression: expression '+' expression 
				{
					$$ = binaryOperation(&$1, &$3, 0) 
				}
		  | expression '-' expression 
		  		{
		  			$$ = binaryOperation(&$1, &$3, 1); 
		  		}
		  | expression '*' expression 
		  		{
		  			$$ = binaryOperation(&$1, &$3, 2); 
		  		}
		  | expression '/' expression 
		  		{ 
		  			$$ = binaryOperation(&$1, &$3, 3); 
		  		}
		  | '-' expression %prec UMINUS 
		  		{ 
		  			$$ = UMinusByType(&$2); 
		  		}
		  | '(' expression ')' 
		  		{ 
		  			$$.type = $2.type;
		  			assignValue(&$$, $2.value) ;
		  		}
		  | NUMBER
		  | NAME { $$ = (Node) getL(symbolList, $$.name); }
		  ;

%%

int main(int argc, char **argv)
{
	symbolList = createListL(cmpFunction, sizeof(node));
}