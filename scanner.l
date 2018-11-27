%{
	#include "node.h"
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%

[0-9]+\.[0-9]+ {
	yylval.n = malloc(sizeof(node));
	double* val = (double*) malloc(sizeof(double));
	*val = atof(yytext);
	yylval.n->value = (void*) val;
	yylval.n->type = floating;
	return FLOAT;
}

[0-9]+ {
	yylval.n = malloc(sizeof(node));
	int* val = (int*) malloc(sizeof(node));
	*val = atoi(yytext);
	yylval.n->value = (void*) val;
	yylval.n->type = integer;
	return INTEGER;
}

\"(\\.|[^"\\])*\" {
	int len = strlen(yytext);
	yylval.n = malloc(sizeof(node));
	yylval.n->value = malloc((len - 1)*sizeof(char));
	yylval.n->type = string;
	yylval.n->dataSize = len - 1;
	memcpy(yylval.n->value, yytext + 1, (len - 2) * sizeof(char));
	((char*)yylval.n->value)[len - 1] = 0;
	return STRING;
}

"exit" {
	return EXIT;
}

"if" {
	return IF;
}

"true" {
	yylval.n = calloc(1, sizeof(node));
	yylval.n->type = boolean;
	yylval.n->dataSize = sizeof(int);
	yylval.n->value = malloc(sizeof(int));
	int val = 1;
	memcpy(yylval.n->value, (void*)&val, sizeof(int));
	return BOOL;
}

"false" {
	yylval.n = calloc(1, sizeof(node));
	yylval.n->type = boolean;
	yylval.n->dataSize = sizeof(int);
	yylval.n->value = malloc(sizeof(int));
	int val = 0;
	memcpy(yylval.n->value, (void*)&val, sizeof(int));
	return BOOL;
}

"const" {
	return CONST;
}

">=" {
	return GTOET;
}

"<=" {
	return LTOET;
}

"!=" {
	return NET;
}

"==" {
	return ET;
}

"&&" {
	return AND;
}

"||" {
	return OR;
}

"!" {
	return NOT;
}

"while" {
	return WHILE;
}

"do" {
	return DO;
}

"loop" {
	return LOOP;
}

"print "\"(\\.|[^"\\])*\" {
	int len = strlen(yytext + 7) - 1;
	yylval.printedString = calloc(len, sizeof(char));
	memcpy((void*) yylval.printedString, (void*) (yytext + 7), len*sizeof(char));
	return PRINTEXPR;
}

"println "\"(\\.|[^"\\])*\" {
	int len = strlen(yytext + 9) - 1;
	yylval.printedString = calloc(len, sizeof(char));
	memcpy((void*) yylval.printedString, (void*) (yytext + 9), len*sizeof(char));
	return PRINTEXPR;
}

[ \t] ;
[a-z]+ {
	yylval.n = calloc(1, sizeof(node));
	yylval.n->name = calloc(strlen(yytext) + 1, sizeof(char));
	strcpy(yylval.n->name, yytext);
	return NAME;
}
"$" { return 0; }
\n |
. return yytext[0];

%%
