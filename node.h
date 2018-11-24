#ifndef NODE_H
#define NODE_H

enum type { integer = 0, floating, string, boolean };
enum binaryOperator { addition = 0, subtraction, multiplication, division };
typedef enum type TYPE;
typedef enum binaryOperator BINARY_OPERATOR; 

struct node
{
	char* name;
	TYPE type;
	void* value;
	int dataSize;
	int constant;
};

typedef struct node node;
typedef node * Node;

int cmpFunction(void* node1, void* node2);
void yyerror(char const *s);
Node newNode(char* name, TYPE type, void* value, int constant);
void assignValue(Node n, void* value);
void assignSize(Node n, void* value);
void printByValue(node n);
Node binaryOperation(Node typeDefiner, Node typeBetrayer, int operation);

Node addByType(Node typeDefiner, Node typeBetrayer);
Node addIntegers(Node n1, Node n2);
Node addFloats(Node n1, Node n2);
Node addStrings(Node n1, Node n2);
Node addBooleans(Node n1, Node n2);

Node subtractByType(Node typeDefiner, Node typeBetrayer);
Node subtractIntegers(Node n1, Node n2);
Node subtractFloats(Node n1, Node n2);
Node subtractStrings(Node n1, Node n2);
Node subtractBooleans(Node n1, Node n2);

Node multiplyByType(Node typeDefiner, Node typeBetrayer);
Node multiplyIntegers(Node n1, Node n2);
Node multiplyFloats(Node n1, Node n2);
Node multiplyStrings(Node n1, Node n2);
Node multiplyBooleans(Node n1, Node n2);

Node divideByType(Node typeDefiner, Node typeBetrayer);
Node divideIntegers(Node n1, Node n2);
Node divideFloats(Node n1, Node n2);
Node divideStrings(Node n1, Node n2);
Node divideBooleans(Node n1, Node n2);

Node UMinusByType(Node n);

Node toString(Node n);
void toStringFromInteger(Node n, Node result);
void toStringFromFloating(Node n, Node result);
void toStringFromString(Node n, Node result);
void toStringFromBoolean(Node n, Node result);

Node toInteger(Node n);
void toIntegerFromInteger(Node n, Node result);
void toIntegerFromFloating(Node n, Node result);
void toIntegerFromString(Node n, Node result);
void toIntegerFromBoolean(Node n, Node result);

Node toFloating(Node n);
void toFloatingFromInteger(Node n, Node result);
void toFloatingFromFloating(Node n, Node result);
void toFloatingFromString(Node n, Node result);
void toFloatingFromBoolean(Node n, Node result);

Node toBoolean(Node n);
void toBooleanFromInteger(Node n, Node result);
void toBooleanFromFloating(Node n, Node result);
void toBooleanFromString(Node n, Node result);
void toBooleanFromBoolean(Node n, Node result);

#endif