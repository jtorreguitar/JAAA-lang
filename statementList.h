#ifndef STATEMENT_LIST_H
#define STATEMENT_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

enum STATEMENT_TYPE { UNDEFINED = 0, DECLARATION, ASIGNATION, CONST_DECLARATION,
								PRINT, CONDITION_BLOCK, LOOP_BLOCK, EXIT_STATEMENT};

enum OUTPUT_CODE {C = 0, JAVA = 1};

struct statementList {
	enum STATEMENT_TYPE type;
	struct statementList *block;
	int condition;
	struct statementList *next;	
	Node node;
};


typedef struct statementList *sList;

sList newList();
sList createAssingStatement(Node n);
sList createDeclareStatement(Node n);
sList createConstDeclareStatement(Node n);
sList createConditionalStatement();
sList createExitStatement();
sList createLoopStatement();
void printList(sList l);
void generateConstDeclaration(sList l);
void generateDeclaration(sList l);
void generateAsignation(sList l);
void setLanguage(int outputLanguage);
void generateExitCode();
void generateCodeStart();
void generateCodeEnd();
void generateConditionBlockCode(sList l);

#endif
