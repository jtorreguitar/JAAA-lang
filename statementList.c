#include "statementList.h"
#include "cBuilder.h"

int language = C;

sList newList() {
	sList l = malloc(sizeof(*l));

	if(l == NULL) {
		fprintf(stderr, "Cannot allocate memory.");
		exit(0);
	}

	l->type = UNDEFINED;
	l->block = NULL;
	l->next = NULL;
	l->condition = -1;
	l->node = NULL;
	l->second = NULL;

	return l;
}

sList createAssingStatement(Node oldNode, Node newNode) {
	printf("in asignation\n");
	sList l 	= newList();
	l->type 	= ASIGNATION;
	l->node 	= oldNode;
	l->second 	= newNode;
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
				printf("llego aca3\n");

				generateConditionBlockCode(l);
				printf("if\n");
				break;

			case CONST_DECLARATION:
				generateConstDeclaration(l);
				printf("const declaration\n");
				break;

			case DECLARATION:
				generateDeclaration(l);
				printf("declaration\n");
				break;

			case ASIGNATION:
				generateAsignation(l);
				printf("asignation\n");
				break;

			case LOOP_BLOCK:
				printf("while\n");
				break;

			case EXIT_STATEMENT:
				generateExitCode();
				printf("exit statement\n");
				break;

			case PRINT:
				printf("print\n");
				break;
		}

		l = l->next;
	}
}

void setLanguage(int outputLanguage) {
	language = outputLanguage;
}

void generateConstDeclaration(sList l) {
	if(language == C) {
		createConstantCVar(l->node->type, l->node->name, l->node);
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateDeclaration(sList l) {
	if(language == C) {
		createCVar(l->node->type, l->node->name, l->node);
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateAsignation(sList l) {
	if(language == C) {
		assignCVar(l->node, l->second);
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateExitCode() {
	if(language == C) {
		generateCExitCode();
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateCodeStart() {
	if(language == C) {
		generateCCodeStart();
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateCodeEnd() {
	if(language == C) {
		generateCCodeEnd();
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

}

void generateConditionBlockCode(sList l) {
	if(language == C) {
			printf("llego aca2\n");

		generateCConditionBlock(l);
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

