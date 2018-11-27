%{
	#include "node.h"
	#include "statementList.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "listADT.h"

	extern void yyeror(char* text);
	extern int yylex();

	listADT symbolList;
	sList first = NULL;
	sList current = NULL;
%}

%union {
	char *printedString;
	Node n;
	sList l;
}

%token <n> NAME
%token <n> FLOAT
%token <n> INTEGER
%token <n> STRING
%token <n> BOOL
%token <n> WHILE
%token <n> DO
%token <n> LOOP
%token <printedString> PRINTEXPR
%token <printedString> PRINTLNEXPR
%token CONST LTOET GTOET ET NET AND OR NOT VOIDEXPR EXIT IF END
%left AND OR NOT
%left '<' LTOET '>' GTOET
%left ET NET
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%type <n> expression
%type <l> statement_list
%type <l> statement
%%

statement_list: statement '\n'
					{
						$$ = $1;
						first = $$;
						current = first;
						printf("creating only one block\n");
					}
				|statement_list statement '\n'
					{
						current->next = $2;
						current = current->next;
						printf("creating second block\n");
					}
			  	| '\n'
			  		{
			  			printf("final endline\n");
			  			current = NULL;
			  		}
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
						$$ = createAssingStatement(var);
					}
					else if(var != 0 && var->constant)
					{
						yyerror("constants cannot change their value");
					}
					else
					{
						addL(symbolList, newNode($1->name, $3->type, $3->value, 0));
						createVar($3->type, $1->name, $3);
						Node var = (Node) getL(symbolList, $1->name);
						$$ = createDeclareStatement(var);
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
						Node var = (Node) getL(symbolList, $2->name);
						$$ = createConstDeclareStatement(var);
					}
				}
		 | expression {printByValue(*$1); $$ = newList();}
		 | printExpression {$$ = newList();}
		 | conditional {$$ = createConditionalStatement();}
		 | exit_statement {$$ = createExitStatement();}
		 | whileLoop {$$ = createLoopStatement();}	  
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

whileLoop: WHILE ' ' expression ' ' DO statement_list LOOP
			{
				printf("while(expression) {statement;}"); //TODO
			}
			;

exit_statement: EXIT '\n'
				{
					printf("exit(0);");
				}
				;

conditional: IF expression DO statement_list END
				{
					printf("if(%d) {}", *(int *)$2->value);

				}
				;

%%

int main(int argc, char **argv)
{
	symbolList = createListL(cmpFunction, sizeof(node));
	printf("#include <stdio.h>\n#include<stdlib.h>\n#include<string.h>\n");
	printf("int main(void) {");
	yyparse();
	printf("return 0;}");
	printList(first);
}
