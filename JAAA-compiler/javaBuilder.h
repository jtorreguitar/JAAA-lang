#ifndef JAVA_BUILDER_H
#define JAVA_BUILDER_H

#include "node.h"
#include "statementList.h"

#define MAX_NUMBER_LENGTH 120



void createJavaVar(int type, char *name, struct node *expression);
void assignJavaVar(struct node *var, struct node*newValue);
void createConstantJavaVar(int type, char *name, struct node *expression);
void generateJavaExitCode();
void generateJavaCodeStart();
void generateJavaCodeEnd();
void generateJavaConditionBlock(sList l);
void generateJavaLoopBlock(sList l);
void generateJavaPrintCode(sList l);

// //expressions
Node buildJavaBooleanExpression(Node n);
Node buildJavaStringExpression(Node n);
Node buildJavaIntegerExpression(Node n);
Node buildJavaFloatExpression(Node n);
Node buildJavaNotExpression(Node n);
Node buildJavaMinusExpression(Node n);
Node buildJavaBinaryExpression(Node first, Node second, int operator);
Node buildJavaRelationalExpression(Node first, Node second, int operator);
Node buildJavaLogicalExpression(Node first, Node second, int operator);
Node buildJavaParenthesisExpression(Node n);



#endif