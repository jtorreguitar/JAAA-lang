#ifndef STATEMENT_LIST_H
#define STATEMENT_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

enum STATEMENT_TYPE { UNDEFINED = 0, DECLARATION, ASIGNATION, CONST_DECLARATION,
								PRINT, CONDITION_BLOCK, LOOP_BLOCK, EXIT_STATEMENT};

struct statementList {
	enum STATEMENT_TYPE type;
	struct statementList *body;
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
#endif
