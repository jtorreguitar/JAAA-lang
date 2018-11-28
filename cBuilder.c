#include "cBuilder.h"
#include <stdio.h>
#include <string.h>

void createCVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("int %s = %d;", name, *(int *)expression->value);
			break;

		case floating:
			printf("double %s = %lf;", name, *(double *)expression->value);
			break;

		case string:
			printf("char *%s = malloc(sizeof(char) * %d);", name,
												expression->dataSize);
			printf("if(%s == NULL){", name);
			printf("fprintf(stderr,\"Cannot allocate memory\");exit(0);}");
			printf("memcpy(%s, \"%s\", %d);", name, (char *)expression->value,
														expression->dataSize);
			break;
	}
}

void assignCVar(struct node *var, struct node *newValue) {
	switch(var->type) {
		case integer:
		case boolean:
			printf("%s = %d;",var->name, *(int *)newValue->value);
			break;

		case floating:
			printf("%s = %lf;",var->name, *(double *)newValue->value);
			break;

		case string:
			if(var->dataSize < newValue->dataSize) {
				printf("%s = realloc(%s, sizeof(char) * %d);", var->name,
											var->name, newValue->dataSize);
				printf("if(%s == NULL){", var->name);
				printf("fprintf(stderr,\"Cannot allocate memory\");exit(0);}");
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
			printf("const int %s = %d;", name, *(int *)expression->value);
			break;

		case floating:
			printf("const double %s = %lf;", name, *(double *)expression->value);
			break;

		case string:
			printf("const char *%s = %s);", name, (char *)expression->value);
		
			break;
	}

}

void generateCExitCode() {
	printf("exit(0);");
}

void generateCCodeStart() {
	printf("#include <stdio.h>\n#include<stdlib.h>\n#include<string.h>\n");
	printf("int main(void) {");
}

void generateCCodeEnd() {
	printf("return 0;}");
}

void generateCConditionBlock(sList l) {
	printf("llego aca\n");
	printf("if( %d ) {}", l->condition);
	//still needs to iterate in all blocks
}

Node buildCBooleanExpression(Node n) {
	n->name = calloc(2, sizeof(char));
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(0);
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
		exit(0);
	}

	memcpy(n->name, n->value, n->dataSize);
	return n;
}

Node buildCIntegerExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));
	
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(0);
	}

	sprintf(n->name, "%d", *((int *)n->value));
	return n;
}

Node buildCFloatExpression(Node n) {
	n->name = calloc(MAX_NUMBER_LENGTH, sizeof(char));
	
	if(n->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(0);
	}

	sprintf(n->name, "%lf", *((double *)n->value));
	return n;
}

Node buildCNotExpression(Node n) {
	Node newNode = clone(n);
	newNode->name = calloc(strlen(n->name) + 2, sizeof(char));

	if(newNode->name == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(0);
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
		exit(0);
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
		exit(0);
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
