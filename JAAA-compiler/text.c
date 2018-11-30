#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

textToPrint newTextToPrint() {
	textToPrint text = malloc(sizeof(*text));

	if(text == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	text->first 	= NULL;
	text->curr  	= NULL;
	text->type 		= -1;
	return text;
}

textNode createNode(char *name, void *value, int dataSize, int type) {
	textNode text = newTextNode();

	if(name != NULL) {
		int length = strlen(name) + 1, i = 0;
		text->value = calloc(length, sizeof(char));

		if(text->value == NULL) {
			fprintf(stderr, "Cannot allocate memory\n");
			exit(1);
		}

		while(i < length) {
			(text->value)[i] = name[i];
			i++;
		}
		//memcpy(text->value, name, strlen(name + 1));
		text->type = VAR;
		text->varType = type;
		//free(name);
	}
	else {
		text->value = calloc(dataSize, sizeof(char));
		
		if(text->value == NULL) {
			fprintf(stderr, "Cannot allocate memory\n");
			exit(1);
		}
		
		memcpy(text->value, value, dataSize);
		text->type = TEXT;
		//free(value);
	}


	return text;
}


textNode newTextNode() {
	textNode text = malloc(sizeof(*text));
	
	if(text == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		exit(1);
	}

	text->type 		= TEXT;
	text->varType 	= -1;
	text->value		= NULL;
	text->next 		= NULL;

	return text;
}

textToPrint addNode(textToPrint l, textNode n) {
	if(l == NULL || n == NULL) {
		fprintf(stderr, "invalid text list\n");
		exit(1);
	}

	if(l->first == NULL) {
		l->first = n;
		l->curr  = l->first;
	}
	else {
		l->curr->next = n;
		l->curr = l->curr->next;
	}

	return l;
}

textToPrint concatenate(textToPrint l1, textToPrint l2) {
	if(l1 == NULL) {
		fprintf(stderr, "invalid text list\n");
		exit(1);
	}

	if(l2 == NULL) {
		return l1;
	}

	if(l1->first == NULL) {
		l1->first = l2->first;
	}
	else {
		l1->curr->next = l2->first;
	}

	return l1;
}
