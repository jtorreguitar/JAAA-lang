%{
	#include "node.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "listADT.h"

	extern void yyeror(char* text);
	extern int yylex();

	listADT symbolList;
%}

%union {
	char *printedString;
	Node n;
}

%token <n> NAME
%token <n> FLOAT
%token <n> INTEGER
%token <n> STRING
%token <n> BOOL
%token <printedString> PRINTEXPR
%token <printedString> PRINTLNEXPR
%token CONST LTOET GTOET ET NET AND OR NOT VOIDEXPR EXIT IF
%left AND OR NOT
%left '<' LTOET '>' GTOET
%left ET NET
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%type <n> expression

%%

statement_list: statement '\n'
			  | statement_list statement '\n'
			  | exit_statement statement_list
			  | conditional statement_list
			  | '\n'
			  ;

statement: NAME '=' expression 
				{ 
					Node var = (Node) getL(symbolList, $1->name);
					if(var != 0  && !var->constant)
					{
						free(var->value);
						
						if(var->type != $3->type) {
							yyerror("variables cannot change their type");
						}
						
						//var->type = $3->type;
						assignValue(var, $3->value);
						assignVar(var, $3);
						var->dataSize = $3->dataSize;
					}
					else if(var != 0 && var->constant)
					{
						yyerror("constants cannot change their value");
					}
					else
					{
						addL(symbolList, newNode($1->name, $3->type, $3->value, 0));
						createVar($3->type, $1->name, $3);
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
						createConstantVar($4->type, $2->name, $4);
					}
				}
		 | expression { printByValue(*$1); }
		 | printExpression
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
		  | expression '<' expression
				{
					$$ = relationalOperation($1, $3, LESSTHAN);
				}
		  | expression LTOET expression
		  		{
					$$ = relationalOperation($1, $3, LESSTHANOREQUALTO);
				}
		  | expression '>' expression
		  		{
					$$ = relationalOperation($1, $3, GREATERTHAN);  
				}
		  | expression GTOET expression
		  		{
					$$ = relationalOperation($1, $3, GREATERTHANOREQUALTO);
				}
		  | expression ET expression
		  		{
					$$ = relationalOperation($1, $3, EQUALTO);
				}
		  | expression NET expression
		  		{
					$$ = relationalOperation($1, $3, NOTEQUALTO);
				}
		  | expression AND expression
		  		{
					$$ = logicalOperation($1, $3, and);
				}
		  | expression OR expression
		  		{
					$$ = logicalOperation($1, $3, or);
				}
		  | NOT expression
		  		{
				    $$ = logicalOperation($2, NULL, not);
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

printExpression: PRINTEXPR
					{
						printf("%s", $1);
					}
				| PRINTLNEXPR
					{
						printf("%s\n", $1);
					}
				;

exit_statement: EXIT '\n'
				{
					printf("exit(0);");
				}

conditional: IF '(' expression ')' '{' statement_list '}'
				{
					printf("if(expression){statement;}");
				}

%%

int main(int argc, char **argv)
{
	symbolList = createListL(cmpFunction, sizeof(node));
	printf("#include <stdio.h>\n#include<stdlib.h>\n#include<string.h>\n");
	printf("int main(void) {");
	yyparse();
	printf("return 0;}");
}
