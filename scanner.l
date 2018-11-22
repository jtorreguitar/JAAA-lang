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
