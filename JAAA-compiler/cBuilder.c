#include "cBuilder.h"
#include <stdio.h>
#include <string.h>

static int printNode(textNode n);
static textNode printVariables(textNode n, int *var);

void createCVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("int %s = %s;", name, expression->name);
			break;

		case floating:
			printf("double %s = %s;", name, expression->name);
			break;

		case string:
			printf("char *%s = malloc(sizeof(char) * %d);", name,
												expression->dataSize);
			printf("if(%s == NULL){", name);
			printf("fprintf(stderr,\"Cannot allocate memory\");exit(1);}");
			printf("memcpy(%s, \"%s\", %d);", name, (char *)expression->value,
														expression->dataSize);
			break;
	}
}

void assignCVar(struct node *var, struct node *newValue) {
	switch(var->type) {
		case integer:
		case boolean:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case floating:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case string:
			if(var->dataSize < newValue->dataSize) {
				printf("%s = realloc(%s, sizeof(char) * %d);", var->name,
											var->name, newValue->dataSize);
				printf("if(%s == NULL){", var->name);
				printf("fprintf(stderr,\"Cannot allocate memory\");exit(1);}");
			}

			printf("memcpy(%s, \"%s\", %d);", var->name, (char *)newValue->value,
																newValue->dataSize);

			break;
	}
}

void createConstantCVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("const int %s = %s;", name, expression->name);
			break;

		case floating:
			printf("const double %s = %s;", name, expression->name);
			break;

		case string:
			printf("const char *%s = \"%s\";", name, expression->name);

			break;
	}

}

void generateCExitCode() {
	printf("exit(0);");
}

void generateCCodeStart() {
	printf("#include <stdio.h>\n#include<stdlib.h>\n#include<string.h>\n");
	printf("#include <unistd.h>\n");
	printf("int main(void) {");
}

void generateCCodeEnd() {
	printf("return 0;}");
}

void generateCConditionBlock(sList l) {

	switch(l->conditionType) {
		case IF_TYPE:
			printf("if( %s ) {", l->condition);
			printList(l->block);
			printf("}");
			break;

		case IF_ELSE_TYPE:
			printf("if( %s ) {", l->condition);
			printList(l->block);
			printf("}");
			printList(l->elseBlock);
			break;

		case ELSE_IF_TYPE:
			printf("else if( %s ) {", l->condition);
			printList(l->block);
			printf("}");
			break;

		case ELSE_IF_ELSE_TYPE:
			printf("else if( %s ) {", l->condition);
			printList(l->block);
			printf("}");
			printList(l->elseBlock);
			break;

		case ELSE_TYPE:
			printf("else {");
			printList(l->block);
			printf("}");
			break;

		default:
			break;
	}
}

void generateCLoopBlock(sList l) {

	switch(l->loopType) {
		case WHILE_TYPE:
			printf("while( %s ) {", l->condition);
			printList(l->block);
			printf("} ");
			break;

		case DO_WHILE_TYPE:
			printf("do {");
			printList(l->block);
			printf("} while( %s ); ", l->condition);
			break;

		case UNTIL_TYPE:
			printf("while( !(%s) ) {", l->condition);
			printList(l->block);
			printf("} ");
			break;

		case DO_UNTIL_TYPE:
			printf("do {");
			printList(l->block);
			printf("} while( !(%s) ); ", l->condition);
			break;

		default:
			break;
	}
}

void generateCPrintCode(sList l) {
	textNode text = l->text->first;
	textNode aux = text;
	int vars = 0;
	printf("printf(\"");

	while(aux != NULL) {
		vars += printNode(aux);
		aux = aux->next;
	}

	if(vars > 0) {
		if(l->text->type == ONE_LINE) {
			printf("\"");
		}
		else {
			printf("\\n\"");
		}

		while(vars > 0) {
			text = printVariables(text, &vars);
		}

		printf(");");

	}
	else {
		if(l->text->type == ONE_LINE) {
			printf("\");");
		}
		else {
			printf("\\n\");");
		}
	}
}

static int printNode(textNode n) {
	if(n->type == TEXT) {
		printf("%s ", n->value);
		return 0;
	}
	else {

		switch(n->varType) {
			case integer:
			case boolean:
				printf("%%d ");
			break;
			case floating:
				printf("%%lf ");
				break;
			case string:
				printf("%%s ");
				break;
		}

		return 1;

	}
}

static textNode printVariables(textNode n, int *vars) {
	if(n->type == VAR) {
		printf(", %s", n->value);
		*vars = *vars - 1;
	}

	return n->next;
}

Node buildCBooleanExpression(Node n) {
	n->name = calloc(2, sizeof(char));
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	if(*((int *)n->value) == 1) {
		memcpy(n->name, "1", 2);
	}
	else {
		memcpy(n->name, "0", 2);
	}

	return n;
}

Node buildCStringExpression(Node n) {
	n->name = calloc(n->dataSize, sizeof(char));
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	memcpy(n->name, n->value, n->dataSize);
	return n;
}

Node buildCIntegerExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));

	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(n->name, "%d", *((int *)n->value));
	return n;
}

Node buildCFloatExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));

	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(n->name, "%lf", *((double *)n->value));
	return n;
}

Node buildCNotExpression(Node n) {
	Node newNode = clone(n);
	newNode->name = calloc(strlen(n->name) + 2, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(newNode->name, "!%s", n->name);

	//not to free variables evans
	//free(n->name);
	//free(n);
	return newNode;
}

Node buildCMinusExpression(Node n) {
	Node newNode = clone(n);
	newNode->name = calloc(strlen(n->name) + 2, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(newNode->name, "-%s", n->name);

	//not to free variables evans
	//free(n->name);
	//free(n);
	return newNode;
}


Node buildCBinaryExpression(Node first, Node second, int operator) {
	Node newNode = clone(first);
	int length = strlen(first->name) + 1 + strlen(second->name) + 1 +2;
	newNode->name = calloc(length, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	switch(operator) {
		case addition:
			sprintf(newNode->name, "%s + %s", first->name, second->name);
			break;
		case subtraction:
			sprintf(newNode->name, "%s - %s", first->name, second->name);
			break;
		case multiplication:
			sprintf(newNode->name, "%s * %s", first->name, second->name);
			break;
		case division:
			sprintf(newNode->name, "%s / %s", first->name, second->name);
			break;
	}

	//not to free variables evans
	// free(first->name);
	// free(first);
	// free(second->name);
	// free(second);
	return newNode;
}

Node buildCRelationalExpression(Node first, Node second, int operator) {
	Node newNode = clone(first);
	int length = strlen(first->name) + 1 + strlen(second->name) + 1 + 3;
	newNode->name = calloc(length, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	switch(operator) {
		case LESSTHAN:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) < 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") < 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) < 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") < 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s < %s", first->name, second->name);
			}

			break;

		case LESSTHANOREQUALTO:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) <= 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") <= 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) <= 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") <= 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s <= %s", first->name, second->name);
			}

			break;

		case GREATERTHAN:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) > 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") > 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) > 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") > 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s > %s", first->name, second->name);
			}

			break;

		case GREATERTHANOREQUALTO:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) >= 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") >= 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) >= 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") >= 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s >= %s", first->name, second->name);
			}

			break;

		case EQUALTO:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) == 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") == 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) == 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") == 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s == %s", first->name, second->name);
			}

			break;

		case NOTEQUALTO:
			if(first->type == string) {
				if(first->variable == 1 && second->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, %s) != 0", first->name, second->name);
				}
				else if(first->variable == 1) {
					sprintf(newNode->name, "strcmp(%s, \"%s\") != 0", first->name, second->name);
				}
				else if(second->variable == 1) {
					sprintf(newNode->name, "strcmp(\"%s\", %s) != 0", first->name, second->name);
				}
				else {
					sprintf(newNode->name, "strcmp(\"%s\", \"%s\") != 0", first->name, second->name);
				}
			}
			else {
				sprintf(newNode->name, "%s != %s", first->name, second->name);
			}

			break;
	}

	//not to free variables evans
	// free(first->name);
	// free(first);
	// free(second->name);
	// free(second);
	return newNode;
}

Node buildCLogicalExpression(Node first, Node second, int operator) {
	Node newNode = clone(first);
	int length = strlen(first->name) + 1 + strlen(second->name) + 1 + 3;
	newNode->name = calloc(length, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	switch(operator) {
		case and:
			sprintf(newNode->name, "%s && %s", first->name, second->name);
			break;
		case or:
			sprintf(newNode->name, "%s || %s", first->name, second->name);
			break;
	}

	//not to free variables evans
	// free(first->name);
	// free(first);
	// free(second->name);
	// free(second);
	return newNode;
}

Node buildCParenthesisExpression(Node n) {
	Node newNode = clone(n);
	newNode->name = calloc(strlen(n->name) + 3, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(newNode->name, "(%s)", n->name);

	//not to free variables evans
	//free(n->name);
	//free(n);
	return newNode;

}

void buildCReadExpression(Node length, char* varName) {
	if(varName != NULL) {
		printf("%s = malloc(sizeof(char) * %d);", varName, *((int *)length->value) + 1);
		printf("if(%s == NULL){", varName);
		printf("fprintf(stderr,\"Cannot allocate memory\");exit(1);}");
	}
	printf("for(int I=0; I<");
	if(length->name != NULL) {
		printf("%s", length->name);
	}
	else {
		printf("%d", *((int *)length->value));
	}
	printf("; I++){");
	if(varName != NULL) {
		printf("%s[I]=", varName);
	}
	printf("getchar();}");
	if(varName != NULL) {
		printf("%s[%d] = 0;", varName, *((int *)length->value));
	}
}

void createDeclareString(Node n) {
	printf("char *%s = NULL;", n->name);
}
