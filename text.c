#include "text.h"
#include <stdio.h>
#include <stdlib.h>

textToPrint newText() {
	textToPrint text = malloc(sizeof(*text));
	
	if(text == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
	}

	text->type 		= TEXT;
	text->varType 	= -1;
	text->value		= NULL;

	return text;
}

