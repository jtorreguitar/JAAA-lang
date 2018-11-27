#include "statementList.h"

sList newList() {
	sList l = malloc(sizeof(*l));

	if(l == NULL) {
		fprintf(stderr, "Cannot allocate memory.");
		exit(0);
	}

	l->type = UNDEFINED;
	l->body = NULL;
	l->next = NULL;
	l->condition = -1;
	l->node = NULL;

	return l;
}

sList createAssingStatement(Node n) {
	printf("in asignation\n");
	sList l = newList();
	l->type = ASIGNATION;
	l->node = n;
	return l;
}

sList createDeclareStatement(Node n) {
		printf("in declaration\n");

	sList l = newList();
	l->type = DECLARATION;
	l->node = n;
	return l;
}

sList createConstDeclareStatement(Node n) {
	sList l = newList();
	l->type = CONST_DECLARATION;
	l->node = n;
	return l;
}

sList createConditionalStatement() {
	sList l = newList();
	l->type = CONDITION_BLOCK;
	return l;
}

sList createExitStatement() {
	sList l = newList();
	l->type = EXIT_STATEMENT;
	return l;
}

sList createLoopStatement() {
	sList l = newList();
	l->type = LOOP_BLOCK;
	return l;
}

void printList(sList l) {
	if(l == NULL) {
		return;
	}

	while(l != NULL) {
		switch(l->type) {
			case UNDEFINED:
				printf("expression\n");
			break;

			case CONDITION_BLOCK:
				printf("if\n");
				break;

			case CONST_DECLARATION:
				printf("const declaration\n");
				break;

			case DECLARATION:
				printf("declaration\n");
				break;

			case ASIGNATION:
				printf("asignation\n");
				break;

			case LOOP_BLOCK:
				printf("while\n");
				break;

			case EXIT_STATEMENT:
				printf("exit statement\n");
				break;

			case PRINT:
				printf("print\n");
				break;
		}

		l = l->next;
	}
}
