%{
	#include "y.tab.h"
	#include <stdlib.h>
	#include <string.h>
	#include "node.h"
%}

%%

([0-9]+|[0-9]+\.[0-9]+) {
	double* val = malloc(sizeof(double))
	*val = atof(yytext);
	yylval.node.value = (void*) val;
	yylval.node.type = floating;
	return NUMBER;
}

[ \t] ;
[a-z] { 
	yylval.node.name = malloc(sizeof(char)*(strlen(yytext) + 1));
	strcpy(yylaval.node.name, yytext);
	return NAME;
}
"$" { return 0; }
\n |
. return yytext[0];

%%