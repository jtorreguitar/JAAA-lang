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
%type <l> conditional
%%

statement_list: statement
					{
						$$ = $1;
						
						first = $$;
						current = first;
					
						
					

						current->next = NULL;


						printf("creating only one block\n");
					}
				|statement statement_list
					{
						$$ = $1;
						current = $$;
						current->next = first;
						first = current;						
						printf("creating second block\n");
					}
				;
			  	

statement: NAME '=' expression
				{	

					Node var = (Node) getL(symbolList, $1->name);
					if(var != 0  && !var->constant)
					{
						free(var->value);

						if(diferentTypes(var->type, $3->type)) {
							yyerror("variables cannot change their type");
						}

						//var->type = $3->type;
						assignValue(var, $3->value);
						Node oldNode = clone(var);

						if(var->type == string) {
							var->dataSize = $3->dataSize;
						}

						Node newNode = clone(var);
						$$ = createAssingStatement(oldNode, newNode);
					}
					else if(var != 0 && var->constant)
					{
						yyerror("constants cannot change their value");
					}
					else
					{
						addL(symbolList, newNode($1->name, $3->type, $3->value, 0));
						Node var = (Node) getL(symbolList, $1->name);
						Node aux = clone(var);
						$$ = createDeclareStatement(aux);
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
						//createConstantVar($4->type, $2->name, $4);
						Node var = (Node) getL(symbolList, $2->name);
						$$ = createConstDeclareStatement(var);
					}
				}
		 //| expression {printByValue(*$1); $$ = newList();}
		 | printExpression {$$ = newList();}
		 | conditional //{$$ = createConditionalStatement();}
		 | exit_statement {$$ = createExitStatement();}
		 | whileLoop {$$ = createLoopStatement();}	  
		 ;
		 

expression: expression '+' expression
				{
					//$$ = binaryOperation($1, $3, addition);
					$$ = buildBinaryExpression($1, $3, addition);
					printf("expression = %s\n", $$->name);
				}
		  | expression '-' expression
		  		{
		  			//$$ = binaryOperation($1, $3, subtraction);
		  			$$ = buildBinaryExpression($1, $3, subtraction);
		  			printf("expression = %s\n", $$->name);


		  		}
		  | expression '*' expression
		  		{
		  			//$$ = binaryOperation($1, $3, multiplication);
		  			$$ = buildBinaryExpression($1, $3, multiplication);
		  			printf("expression = %s\n", $$->name);
		  		}
		  | expression '/' expression
				{
					//$$ = binaryOperation($1, $3, division);
					$$ = buildBinaryExpression($1, $3, division);
					printf("expression = %s\n", $$->name);
				}
		  | '-' expression %prec UMINUS
		  		{
		  			//$$ = UMinusByType($2);
		  			$$ = buildMinusExpression($2);
		  			printf("expression = %s\n", $$->name);
		  		}
		  | '(' expression ')'
		  		{
		  			$$ = $2;
		  		}
		  | expression '<' expression
				{
					//$$ = relationalOperation($1, $3, LESSTHAN);
					$$ = buildRelationalExpression($1, $3, LESSTHAN);
					printf("expression = %s\n", $$->name);


				}
		  | expression LTOET expression
		  		{
					//$$ = relationalOperation($1, $3, LESSTHANOREQUALTO);
					$$ = buildRelationalExpression($1, $3, LESSTHANOREQUALTO);
					printf("expression = %s\n", $$->name);
				}
		  | expression '>' expression
		  		{
					//$$ = relationalOperation($1, $3, GREATERTHAN);
					$$ = buildRelationalExpression($1, $3, GREATERTHAN);
					printf("expression = %s\n", $$->name);

				}
		  | expression GTOET expression
		  		{
					//$$ = relationalOperation($1, $3, GREATERTHANOREQUALTO);
					$$ = buildRelationalExpression($1, $3, GREATERTHANOREQUALTO);
					printf("expression = %s\n", $$->name);
				}
		  | expression ET expression
		  		{
					//$$ = relationalOperation($1, $3, EQUALTO);
					$$ = buildRelationalExpression($1, $3, EQUALTO);
					printf("expression = %s\n", $$->name);
				}
		  | expression NET expression
		  		{
					//$$ = relationalOperation($1, $3, NOTEQUALTO);
					$$ = buildRelationalExpression($1, $3, NOTEQUALTO);
					printf("expression = %s\n", $$->name);
				}
		  | expression AND expression
		  		{
					//$$ = logicalOperation($1, $3, and);
					$$ = buildLogicalExpression($1, $3, and);
					printf("expression = %s\n", $$->name);
				}
		  | expression OR expression
		  		{
					//$$ = logicalOperation($1, $3, or);
					$$ = buildLogicalExpression($1, $3, or);
					printf("expression = %s\n", $$->name);
				}
		  | NOT expression
		  		{
				    //$$ = logicalOperation($2, NULL, not);
				 	$$ = buildNotExpression($2);
					printf("name3= %s", $$->name);
				}
		  | FLOAT
		  		{
		  			$$ = buildFloatExpression($1);
		  			printf("name= %s", $$->name);
		  		}
		  | INTEGER
		  		{
		  			$$ = buildIntegerExpression($1);
		  			printf("name= %s", $$->name);
		  		}
		  | STRING
		  		{
		  			$$ = buildStringExpression($1);
		  			printf("name= %s", $$->name);
		  		}
		  | BOOL 
		  		{
		  			$$ = buildBooleanExpression($1);
		  			printf("name= %s", $$->name);
		  		}
		  | NAME
		  		{
					Node n = (Node) getL(symbolList, $1->name);

					if(n != 0) {
						$$ = n;
					}

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

whileLoop: WHILE expression DO statement_list LOOP
			{
				printf("while(expression) {statement;}"); //TODO
			}
			;

exit_statement: EXIT;

conditional: IF expression DO statement_list END
				{
					//$$->condition = *((int *)($2->value));
					//$$->block = $4;
					//printf("if(%d) {}", *(int *)$2->value);

				}
				;

%%

int main(int argc, char **argv)
{
	if(argc == 2) {
		if(strcmp(argv[1], "-b") == 0) {
			printf("language setted to JAVA\n");
			setLanguage(JAVA);
		}
	}
	symbolList = createListL(cmpFunction, sizeof(node));
	
	generateCodeStart();
	yyparse();

	generateCodeEnd();
	printList(first);
}
