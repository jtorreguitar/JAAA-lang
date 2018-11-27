#ifndef C_BUILDER_H
#define C_BUILDER_H

#include "node.h"

void createCVar(int type, char *name, struct node *expression);
void assignCVar(struct node *var, struct node*newValue);
void createConstantCVar(int type, char *name, struct node *expression);

#endif