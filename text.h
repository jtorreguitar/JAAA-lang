#ifndef TEXT_H
#define TEXT_H

enum TEXT_TYPE { TEXT=0, VAR };



struct textNode {
	enum TEXT_TYPE type;
	char *value;
	int varType;
	struct textNode *next;
};

typedef struct textNode *textNode;

struct textList {
	textNode first;
	textNode curr;
};

typedef struct textList *textToPrint;

textNode newTextNode();

textToPrint newTextToPrint();

textToPrint addNode(textToPrint l, textNode n);

textToPrint concatenate(textToPrint l1, textToPrint l2);

textNode createNode(char *name, void *value, int dataSize, int type);

#endif