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

[ \t] ;
[a-z] { 
	yylval.n = malloc(sizeof(node));
	yylval.n->name = malloc(sizeof(char)*(strlen(yytext) + 1));
	strcpy(yylval.n->name, yytext);
	return NAME;
}
"$" { return 0; }
\n |
. return yytext[0];

%%
