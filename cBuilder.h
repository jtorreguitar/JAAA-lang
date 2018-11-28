#ifndef C_BUILDER_H
#define C_BUILDER_H

#include "node.h"
#include "statementList.h"

#define MAX_NUMBER_LENGTH 120

void createCVar(int type, char *name, struct node *expression);
void assignCVar(struct node *var, struct node*newValue);
void createConstantCVar(int type, char *name, struct node *expression);
void generateCExitCode();
void generateCCodeStart();
void generateCCodeEnd();
void generateCConditionBlock(sList l);

//expressions
Node buildCBooleanExpression(Node n);
Node buildCStringExpression(Node n);
Node buildCIntegerExpression(Node n);
Node buildCFloatExpression(Node n);
Node buildCNotExpression(Node n);




#endif