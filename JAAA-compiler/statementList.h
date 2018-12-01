#ifndef STATEMENT_LIST_H
#define STATEMENT_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "text.h"

enum STATEMENT_TYPE { UNDEFINED = 0, DECLARATION, ASIGNATION, CONST_DECLARATION,
								PRINT, CONDITION_BLOCK, LOOP_BLOCK, EXIT_STATEMENT,
								READ_STATEMENT, DECLARE_READ_STATEMENT};
enum CONDITIONAL_TYPE {UNDEFINED_TYPE = 0, IF_TYPE, IF_ELSE_TYPE, ELSE_IF_TYPE,
													ELSE_IF_ELSE_TYPE, ELSE_TYPE};
enum LOOP_TYPE { WHILE_TYPE, DO_WHILE_TYPE, UNTIL_TYPE, DO_UNTIL_TYPE};

enum OUTPUT_CODE {C = 0, JAVA = 1};

struct statementList {
	enum STATEMENT_TYPE type;
	enum CONDITIONAL_TYPE conditionType;
	enum LOOP_TYPE loopType;
	struct statementList *block;
	struct statementList *elseBlock;
	char *condition;
	struct statementList *next;
	Node node;
	Node second;
	textToPrint text;
};

typedef struct statementList *sList;

sList newList();
sList createAssingStatement(Node oldNode, Node newNode);
sList createDeclareStatement(Node n, Node expression);
sList createConstDeclareStatement(Node n, Node expression);
sList createConditionalStatement();
sList createExitStatement();
sList createLoopStatement();
sList createReadStatement(Node assing, Node var);
sList createReadDeclareStatement(Node assing, Node var);
sList createPrintStatement();

void printList(sList l);
void generateConstDeclaration(sList l);
void generateDeclaration(sList l);
void generateAsignation(sList l);
void setLanguage(int outputLanguage);
void generateExitCode();
void generateCodeStart();
void generateCodeEnd();
void generateConditionBlockCode(sList l);
void generateLoopBlockCode(sList l);
void generateReadStatement(sList l);
void genearteDeclareString(Node n);
void generatePrintCode(sList l);
void freeMemory(sList l);

//expressions
Node buildBooleanExpression(Node n);
Node buildStringExpression(Node n);
Node buildIntegerExpression(Node n);
Node buildFloatExpression(Node n);
Node buildNotExpression(Node n);
Node buildMinusExpression(Node n);
Node buildBinaryExpression(Node first, Node second, int operator);
Node buildRelationalExpression(Node first, Node second, int operator);
Node buildLogicalExpression(Node first, Node second, int operator);
Node buildParenthesisExpression(Node n);
int validateRelationalExpressionType(int type1, int type2);

#endif
