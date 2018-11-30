%{
	#include "node.h"
	#include "statementList.h"
	#include "text.h"
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
	textToPrint printedString;
	Node n;
	sList l;
}



%token <n> NAME
%token <n> FLOAT
%token <n> INTEGER
%token <n> STRING
%token <n> BOOL
%token <n> WHILE
%token <n> UNTIL
%token <n> DO
%token <n> LOOP
%token CONST LTOET GTOET ET NET AND OR NOT VOIDEXPR EXIT IF END ELSE PRINT_TEXT READ_TEXT CHAR
%token PRINT_TEXT_NEW_LINE
%left AND OR NOT
%left '<' LTOET '>' GTOET
%left ET NET
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%type <n> expression
%type <l> statement_list
%type <l> statement
%type <l> while_loop
%type <l> do_loop_statement
%type <n> final_condition
%type <l> conditional
%type <l> else_block
%type <n> readExpression
%type <printedString> text
%type <printedString> printExpression
%%

statement_list: statement
					{
						$$ = $1;

						first = $$;
						current = first;
						current->next = NULL;

						//printf("creating only one block\n"); evans
					}
				|statement statement_list
					{
						$$ = $1;
						current = $$;
						current->next = first;
						first = current;
						//printf("creating second block\n"); evans
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

						//var->type = $3->type; to add weak typed
						assignValue(var, $3->value);
						Node oldNode = clone(var);

						if(var->type == string) {
							var->dataSize = $3->dataSize;
						}

						$$ = createAssingStatement(oldNode, $3);
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
						$$ = createDeclareStatement(aux, $3);
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
						$$ = createConstDeclareStatement(var, $4);
					}
				}
		 | printExpression
		 	{
		 		$$ = createPrintStatement();
		 		$$->text = $1;
		 	}
		 | conditional
		 | exit_statement {$$ = createExitStatement();}
		 | while_loop
		 | readExpression {
			 $$ = createReadStatement($1, newNode(NULL, string, NULL, 0));
		 }
		 | NAME '=' readExpression
		 	{
				Node var = (Node) getL(symbolList, $1->name);
				if(var != 0  && !var->constant)
				{
					if(var->type != string) {
						yyerror("variables cannot change their type");
					}

					var->dataSize = *((int *)$3->value);
					$$ = createReadStatement($3, var);
				}
				else if(var != 0 && var->constant)
				{
					yyerror("constants cannot change their value");
				}
				else
				{
					addL(symbolList, newNode($1->name, string, $3->value, 0));
					Node var = (Node) getL(symbolList, $1->name);
					var->dataSize = *((int *)$3->value) + 1;
					$$ = createReadDeclareStatement($3, var);
				}
			}
		 ;


expression: expression '+' expression
				{
					//$$ = binaryOperation($1, $3, addition); interpreter mode
					$$ = buildBinaryExpression($1, $3, addition);
					//printf("expression = %s\n", $$->name); evans
				}
		  | expression '-' expression
		  		{
		  			//$$ = binaryOperation($1, $3, subtraction); interpreter mode
		  			$$ = buildBinaryExpression($1, $3, subtraction);
		  			//printf("expression = %s\n", $$->name); evans


		  		}
		  | expression '*' expression
		  		{
		  			//$$ = binaryOperation($1, $3, multiplication); interpreter mode
		  			$$ = buildBinaryExpression($1, $3, multiplication);
		  			//printf("expression = %s\n", $$->name); evans
		  		}
		  | expression '/' expression
				{
					//$$ = binaryOperation($1, $3, division); interpreter mode
					$$ = buildBinaryExpression($1, $3, division);
					//printf("expression = %s\n", $$->name); evans
				}
		  | '-' expression %prec UMINUS
		  		{
		  			//$$ = UMinusByType($2); interpreter mode
		  			$$ = buildMinusExpression($2);
		  			//printf("expression = %s\n", $$->name); evans
		  		}
		  | '(' expression ')'
		  		{
		  			//$$ = $2; interpreter mode
		  			$$ = buildParenthesisExpression($2);
		  			//printf("expression = %s\n", $$->name); evans
		  		}
		  | expression '<' expression
				{
					//$$ = relationalOperation($1, $3, LESSTHAN); interpreter mode
					$$ = buildRelationalExpression($1, $3, LESSTHAN);
					//printf("expression = %s\n", $$->name); evans


				}
		  | expression LTOET expression
		  		{
					//$$ = relationalOperation($1, $3, LESSTHANOREQUALTO); interpreter
					$$ = buildRelationalExpression($1, $3, LESSTHANOREQUALTO);
					//printf("expression = %s\n", $$->name); evans
				}
		  | expression '>' expression
		  		{
					//$$ = relationalOperation($1, $3, GREATERTHAN); interpreter mode
					$$ = buildRelationalExpression($1, $3, GREATERTHAN);
					//printf("expression = %s\n", $$->name); evans

				}
		  | expression GTOET expression
		  		{
					//$$ = relationalOperation($1, $3, GREATERTHANOREQUALTO);interpreter
					$$ = buildRelationalExpression($1, $3, GREATERTHANOREQUALTO);
					//printf("expression = %s\n", $$->name);evans
				}
		  | expression ET expression
		  		{
					//$$ = relationalOperation($1, $3, EQUALTO); interpreter mode
					$$ = buildRelationalExpression($1, $3, EQUALTO);
					//printf("expression = %s\n", $$->name); evans
				}
		  | expression NET expression
		  		{
					//$$ = relationalOperation($1, $3, NOTEQUALTO); interpreter mode
					$$ = buildRelationalExpression($1, $3, NOTEQUALTO);
					//printf("expression = %s\n", $$->name); evans
				}
		  | expression AND expression
		  		{
					//$$ = logicalOperation($1, $3, and); interpreter mode
					$$ = buildLogicalExpression($1, $3, and);
					//printf("expression = %s\n", $$->name);evans
				}
		  | expression OR expression
		  		{
					//$$ = logicalOperation($1, $3, or); interpreter mode
					$$ = buildLogicalExpression($1, $3, or);
					//printf("expression = %s\n", $$->name); evans
				}
		  | NOT expression
		  		{
				    //$$ = logicalOperation($2, NULL, not); interpreter mode
				 	$$ = buildNotExpression($2);
					//printf("name3= %s", $$->name); evans
				}
		  | FLOAT
		  		{
		  			//output mode
		  			$$ = buildFloatExpression($1);
		  			//printf("name= %s", $$->name); evans
		  		}
		  | INTEGER
		  		{
		  			//output mode
		  			$$ = buildIntegerExpression($1);
		  			//printf("name= %s", $$->name); evans
		  		}
		  | STRING
		  		{
		  			//output mode
		  			$$ = buildStringExpression($1);
		  			//printf("name= %s", $$->name); evans
		  		}
		  | BOOL
		  		{
		  			//output mode
		  			$$ = buildBooleanExpression($1);
		  			//printf("name= %s", $$->name); evans
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

printExpression: PRINT_TEXT text ';'
					{
						$$ = $2;
						$$->type = ONE_LINE;
					}
				| PRINT_TEXT_NEW_LINE text ';'
					{
						$$ = $2;
						$$->type = NEW_LINE;
					}
				;

	text:	STRING ',' text
				{
					$$ = newTextToPrint();
					textNode n = createNode(NULL, $1->value, $1->dataSize, 0);
					$$ = addNode($$, n);
					$$ = concatenate($$, $3);
					//printf("string text\n");// evans
				}
			| NAME ',' text
				{
					$$ = newTextToPrint();
					Node var = (Node) getL(symbolList, $1->name);

					if(var == NULL) {
						yyerror("Can't print undefined variable");
					}

					textNode n = createNode(var->name, NULL, 0, var->type);
					$$ = addNode($$, n);
					$$ = concatenate($$, $3);
					//printf("name text\n");// evans
				}
			| STRING
				{
					$$ = newTextToPrint();
					textNode n = createNode(NULL, $1->value, $1->dataSize, 0);
					$$ = addNode($$, n);
					//printf("string1\n");//evans
				}
			| NAME
				{
					$$ = newTextToPrint();
					Node var = (Node) getL(symbolList, $1->name);

					if(var == NULL) {
						yyerror("Can't print undefined variable");
					}
					textNode n = createNode(var->name, NULL, 0, var->type);
					$$ = addNode($$, n);
					//printf("name1\n");// evans
				}
			;

while_loop: WHILE expression DO statement_list LOOP
			{
				$$ 				= createLoopStatement();
				$$->condition 	= $2->name;
				$$->block 		= $4;
				$$->loopType 	= WHILE_TYPE;
			}
			| UNTIL expression DO statement_list LOOP
				{
					$$ 				= createLoopStatement();
					$$->condition 	= $2->name;
					$$->block 		= $4;
					$$->loopType 	= UNTIL_TYPE;
				}
			| do_loop_statement

			;

do_loop_statement: DO statement_list LOOP final_condition
					{
						$$ 				= createLoopStatement();
						$$->condition 	= $4->name;
						$$->block 		= $2;
						$$->loopType 	= $4->loopType;

					}

final_condition: WHILE expression
					{
						$$			  = $2;
						$$->loopType  = DO_WHILE_TYPE;
					}
				| UNTIL expression
					{
						$$			  = $2;
						$$->loopType  = DO_UNTIL_TYPE;
					}
				;


exit_statement: EXIT;

conditional: IF expression DO statement_list END
				{
					$$ = createConditionalStatement();
					$$->condition = $2->name;
					$$->block = $4;
					$$->conditionType = IF_TYPE;
					//printf("if(%s) {}", $2->name);
				}
			| IF expression DO statement_list else_block
				{
					$$ = createConditionalStatement();
					$$->condition = $2->name;
					$$->block = $4;
					$$->elseBlock = $5;
					$$->conditionType = IF_ELSE_TYPE;
					//printf("\n if with else\n");//evans
				}
			;

else_block: ELSE IF expression DO statement_list END
				{
					$$ = createConditionalStatement();
					$$->condition = $3->name;
					$$->block = $5;
					$$->conditionType = ELSE_IF_TYPE;
					//printf("\n first and final else\n");//evans
				}
			|
			ELSE IF expression DO statement_list else_block
				{
					$$ = createConditionalStatement();
					$$->condition = $3->name;
					$$->block = $5;
					$$->elseBlock = $6;
					$$->conditionType = ELSE_IF_ELSE_TYPE;
					//printf("\n midle else\n");//evans
				}
			|
			ELSE DO statement_list END
				{
					$$ = createConditionalStatement();
					$$->block = $3;
					$$->conditionType = ELSE_TYPE;
					//printf("\n final else\n");//evans
				}
			;

readExpression: READ_TEXT
				{
					int i = 1;
					Node n = newNode(NULL, integer, &i, 0);
					$$ = n;
				}
			| READ_TEXT NAME CHAR
				{
					Node var = (Node) getL(symbolList, $2->name);
					if(var != 0)
					{
						if(!isInteger(var->type)) {
							yyerror("variables is not integer");
						}
						$$ = var;
					}
					else
					{
						yyerror("variable is not initialize");
					}
				}
			| READ_TEXT INTEGER CHAR
				{
					$2->name = NULL;
					$$ = $2;
				}
			;

%%

int main(int argc, char **argv)
{
	if(argc == 2) {
		if(strcmp(argv[1], "-b") == 0) {
			//printf("language set to JAVA\n");
			setLanguage(JAVA);
		}
	}
	symbolList = createListL(cmpFunction, sizeof(node));

	generateCodeStart();
	yyparse();

	printList(first);
	generateCodeEnd();
	//freeMemory();
}
