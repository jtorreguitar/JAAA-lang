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
%token <n> BOOL
%token CONST
%left '-' '+'
%left '*' '/'
%left '<' '<=' '>' '>=' '=='
%nonassoc UMINUS
%type <n> expression

%%

statement_list: statement '\n'
			  | statement_list statement '\n'
			  ;

statement: NAME '=' expression 
				{ 
					Node var = (Node) getL(symbolList, $1->name);
					if(var != 0  && !var->constant)
					{
						free(var->value);
						var->type = $3->type;
						assignValue(var, $3->value);
					}
					else if(var != 0 && var->constant)
					{
						yyerror("constants cannot change their value");
					}
					else
					{
						addL(symbolList, newNode($1->name, $3->type, $3->value, 0));
					}
				}
		 | CONST NAME '=' expression
		 		{
					Node var = (Node) getL(symbolList, $2->name);
					if(var != 0)
					{
						yyerror("constants cannot change their value");
					}
					else
					{
						addL(symbolList, newNode($2->name, $4->type, $4->value, 1));
					}
				}
		 | expression { printByValue(*$1); }
		 ;

expression: expression '+' expression 
				{
					$$ = binaryOperation($1, $3, addition);
				}
		  | expression '-' expression 
		  		{
		  			$$ = binaryOperation($1, $3, subtraction);
		  		}
		  | expression '*' expression 
		  		{
		  			$$ = binaryOperation($1, $3, multiplication);
		  		}
		  | expression '/' expression 
				{ 
					$$ = binaryOperation($1, $3, division);
				}
		  | '-' expression %prec UMINUS 
		  		{ 
		  			$$ = UMinusByType($2);
		  		}
		  | '(' expression ')' 
		  		{ 
		  			$$ = $2;
		  		}
		  | FLOAT
		  | INTEGER
		  | STRING
		  | BOOL
		  | NAME 
		  		{
					Node n = (Node) getL(symbolList, $1->name);
					if(n != 0) 
						$$ = n;
					else
						yyerror("undeclared variable");
				}
		  ;

%%

int main(int argc, char **argv)
{
	symbolList = createListL(cmpFunction, sizeof(node));
	yyparse();
}
