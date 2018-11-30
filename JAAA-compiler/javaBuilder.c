#include "javaBuilder.h"
#include <stdio.h>
#include <string.h>

void createJavaVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("int %s = %s;", name, expression->name);
			break;

		case floating:
			printf("double %s = %s;", name, expression->name);
			break;

		case string:
			printf("String %s = %s;", name, (char *)expression->value);
			break;
	}
}

void assignJavaVar(struct node *var, struct node *newValue) {
	switch(var->type) {
		case integer:
		case boolean:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case floating:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case string:
			printf("%s = %s;", var->name, (char *)newValue->value);
			break;
	}
}

void createConstantJavaVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
			printf("private static final int %s = %s;", name, expression->name);

		case boolean:
			printf("private static final boolean %s = %s;", name, expression->name);
			break;

		case floating:
			printf("private static final double %s = %s;", name, expression->name);
			break;

		case string:
			printf("private static final String %s = %s);", name, expression->name);

			break;
	}

}

void generateJavaExitCode() {
	printf("System.exit(0);");
}

void generateJavaCodeStart() {
	printf("import java.util.*;\nimport java.lang.*;\n");
	printf("import java.io.*;\n");
	printf("public class Main {\n");
	printf("public static void main(String args[]) {\n");
}

void generateJavaCodeEnd() {
	printf("}}");
}

void generateJavaConditionBlock(sList l) {

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

void generateJavaLoopBlock(sList l) {

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


void generateJavaPrintCode(sList l) {
	textNode text = l->text->first;
	textNode aux = text;
	if(l->text->type == ONE_LINE) {
		printf("System.out.print(");
	}
	else {
		printf("System.out.println(");
	}

	while(aux != NULL) {

		if(aux->type == TEXT) {
			printf("\"%s\"", aux->value);
		}
		else {
			printf("\"\" + %s", aux->value);
		}

		aux = aux->next;

		if(aux != NULL) {
			printf(" + \" \" + ");
		}
	}
	printf(");");
}

Node buildJavaBooleanExpression(Node n) {
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

Node buildJavaStringExpression(Node n) {
	n->name = calloc(n->dataSize, sizeof(char));
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	memcpy(n->name, n->value, n->dataSize);
	return n;
}

Node buildJavaIntegerExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));

	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(n->name, "%d", *((int *)n->value));
	return n;
}

Node buildJavaFloatExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));

	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	sprintf(n->name, "%lf", *((double *)n->value));
	return n;
}

Node buildJavaNotExpression(Node n) {
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

Node buildJavaMinusExpression(Node n) {
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

Node buildJavaBinaryExpression(Node first, Node second, int operator) {
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

Node buildJavaRelationalExpression(Node first, Node second, int operator) {
	Node newNode = clone(first);
	int length = strlen(first->name) + 1 + strlen(second->name) + 1 + 3;
	newNode->name = calloc(length, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	switch(operator) {
		case LESSTHAN:
			sprintf(newNode->name, "%s < %s", first->name, second->name);
			break;
		case LESSTHANOREQUALTO:
			sprintf(newNode->name, "%s <= %s", first->name, second->name);
			break;
		case GREATERTHAN:
			sprintf(newNode->name, "%s > %s", first->name, second->name);
			break;
		case GREATERTHANOREQUALTO:
			sprintf(newNode->name, "%s >= %s", first->name, second->name);
			break;
		case EQUALTO:
			sprintf(newNode->name, "%s == %s", first->name, second->name);
			break;
		case NOTEQUALTO:
			sprintf(newNode->name, "%s != %s", first->name, second->name);
			break;
	}

	//not to free variables evans
	// free(first->name);
	// free(first);
	// free(second->name);
	// free(second);
	return newNode;
}

Node buildJavaLogicalExpression(Node first, Node second, int operator) {
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

Node buildJavaParenthesisExpression(Node n) {
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

void buildJavaReadExpression(Node length, char* varName) {
	printf("try {for(int i=0; i<");
	if(length->name != NULL) {
		printf("%s", length->name);
	}
	else {
		printf("%d", *((int *)length->value));
	}
	printf("; i++){");
	if(varName != NULL) {
		printf("%s +=", varName);
	}
	printf("System.in.read();}}catch(IOException e){System.out.println(\"Error reading from user\");System.exit(0);}");
}
