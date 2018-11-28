#ifndef JAVA_BUILDER_H
#define JAVA_BUILDER_H

#include "node.h"
#include "statementList.h"


void createJavaVar(int type, char *name, struct node *expression);
void assignJavaVar(struct node *var, struct node*newValue);
void createConstantJavaVar(int type, char *name, struct node *expression);
void generateJavaExitCode();
void generateJavaCodeStart();
void generateJavaCodeEnd();
// void generateCConditionBlock(sList l);

// //expressions
// Node buildCBooleanExpression(Node n);
// Node buildCStringExpression(Node n);
// Node buildCIntegerExpression(Node n);
// Node buildCFloatExpression(Node n);
// Node buildCNotExpression(Node n);
// Node buildCMinusExpression(Node n);
// Node buildCBinaryExpression(Node first, Node second, int operator);
// Node buildCRelationalExpression(Node first, Node second, int operator);
// Node buildCLogicalExpression(Node first, Node second, int operator);
// Node buildCParenthesisExpression(Node n);



#endif