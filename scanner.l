%{
	#include "node.h"
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
%}

%%

([0-9]+|[0-9]+\.[0-9]+) {
	double* val = (double*) malloc(sizeof(double));
	*val = atof(yytext);
	yylval.n->value = (void*) val;
	yylval.n->type = floating;
	return NUMBER;
}

[ \t] ;
[a-z] { 
	yylval.n->name = malloc(sizeof(char)*(strlen(yytext) + 1));
	strcpy(yylval.n->name, yytext);
	return NAME;
}
"$" { return 0; }
\n |
. return yytext[0];

%%