#ifndef TEXT_H
#define TEXT_H

enum TEXT_TYPE { TEXT=0, VAR };


struct textToPrint {
	enum TEXT_TYPE type;
	char *value;
	int varType;
};

typedef struct textToPrint *textToPrint;

textToPrint newText();

#endif