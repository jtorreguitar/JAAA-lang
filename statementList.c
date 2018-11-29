#include "statementList.h"
#include "cBuilder.h"
#include "javaBuilder.h"

int language = C;
int scope = 0;

sList newList() {
	sList l = malloc(sizeof(*l));

	if(l == NULL) {
		fprintf(stderr, "Cannot allocate memory.");
		exit(1);
	}

	l->type 			= UNDEFINED;
	l->conditionType 	= UNDEFINED_TYPE;
	l->block 			= NULL;
	l->elseBlock		= NULL;
	l->next 			= NULL;
	l->condition 		= NULL;
	l->node 			= NULL;
	l->second 			= NULL;
	l->text				= NULL;

	return l;
}

sList createAssingStatement(Node oldNode, Node newNode) {
	sList l 	= newList();
	l->type 	= ASIGNATION;
	l->node 	= oldNode;
	l->second 	= newNode;
	return l;
}

sList createDeclareStatement(Node n, Node expression) {
	sList l 	= newList();
	l->type 	= DECLARATION;
	l->node 	= n;
	l->second 	= expression;
	return l;
}

sList createConstDeclareStatement(Node n, Node expression) {
	sList l 	= newList();
	l->type 	= CONST_DECLARATION;
	l->node 	= n;
	l->second 	= expression;
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

sList createPrintStatement() {
	sList l = newList();
	l->type = PRINT;
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
				scope++;
				generateConditionBlockCode(l);
				//printf("if condition=%s\n", l->condition);
				scope--;
				break;

			case CONST_DECLARATION:

				if(scope > 0) {
					fprintf(stderr, "Cannot declare a constant inside a inner block\n");
				}
			
				generateConstDeclaration(l);
				//printf("const declaration\n"); evans
				break;

			case DECLARATION:
			
				if(scope > 0) {
					fprintf(stderr, "Cannot declare a variable inside a inner block\n");
				}
			
				generateDeclaration(l);
				//printf("declaration\n");evans
				break;

			case ASIGNATION:
				generateAsignation(l);
				break;

			case LOOP_BLOCK:
				scope++;
				generateLoopBlockCode(l);
				printf("while\n");
				scope--;
				break;

			case EXIT_STATEMENT:
				generateExitCode();
				break;

			case PRINT:
				generatePrintCode(l);
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
		createConstantCVar(l->node->type, l->node->name, l->second);
	}
	else if(language == JAVA) {
		createConstantJavaVar(l->node->type, l->node->name, l->second);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateDeclaration(sList l) {
	if(language == C) {
		createCVar(l->node->type, l->node->name, l->second);
	}
	else if(language == JAVA) {
		createJavaVar(l->node->type, l->node->name, l->second);
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
		assignJavaVar(l->node, l->second);
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
		generateJavaExitCode();
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
		generateJavaCodeStart();
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
		generateJavaCodeEnd();
	}
	else {
		fprintf(stderr, "unsupported language\n");
	}

}

void generateConditionBlockCode(sList l) {
	if(language == C) {
		generateCConditionBlock(l);
	}
	else if(language == JAVA) {
		generateJavaConditionBlock(l);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generateLoopBlockCode(sList l) {
	if(language == C) {
		generateCLoopBlock(l);
	}
	else if(language == JAVA) {
		fprintf(stderr, "java on development\n");
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

void generatePrintCode(sList l) {
	if(language == C) {
		generateCPrintCode(l);
	}
	else if(language == JAVA) {
		generateJavaPrintCode(l);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
}

Node buildBooleanExpression(Node n) {
	if(language == C) {
		return buildCBooleanExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaBooleanExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");
	}

	exit(1);
}

Node buildStringExpression(Node n) {
	if(language == C) {
		return buildCStringExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaStringExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");
	}

	exit(1);
}

Node buildIntegerExpression(Node n) {
	if(language == C) {
		return buildCIntegerExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaIntegerExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");
	}

	exit(1);
}

Node buildFloatExpression(Node n) {
	if(language == C) {
		return buildCFloatExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaFloatExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");
	}

	exit(1);

}

Node buildNotExpression(Node n) {
	if(language == C) {
		return buildCNotExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaNotExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

	exit(1);

}

Node buildMinusExpression(Node n) {
	if(language == C) {
		return buildCMinusExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaMinusExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

	exit(1);

}

Node buildBinaryExpression(Node first, Node second, int operator) {
	if(language == C) {
		return buildCBinaryExpression(first, second, operator);
	}
	else if(language == JAVA) {
		return buildJavaBinaryExpression(first, second, operator);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}
	exit(1);
}

Node buildRelationalExpression(Node first, Node second, int operator) {
	if(language == C) {
		return buildCRelationalExpression(first, second, operator);
	}
	else if(language == JAVA) {
		return buildJavaRelationalExpression(first, second, operator);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

	exit(1);
}

Node buildLogicalExpression(Node first, Node second, int operator) {
	if(language == C) {
		return buildCLogicalExpression(first, second, operator);
	}
	else if(language == JAVA) {
		return buildJavaLogicalExpression(first, second, operator);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

	exit(1);
}

Node buildParenthesisExpression(Node n) {
	if(language == C) {
		return buildCParenthesisExpression(n);
	}
	else if(language == JAVA) {
		return buildJavaParenthesisExpression(n);
	}
	else {
		fprintf(stderr, "unsupported language\n");

	}

	exit(1);
}
