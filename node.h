#ifndef NODE_H
#define NODE_H

typedef enum type { integer = 0, floating, string, boolean } TYPE;
typedef enum binaryOperator { addition = 0, subtraction, multiplication, division } BINARY_OPERATOR;
typedef enum relationalOperator { LESSTHAN = 0, LESSTHANOREQUALTO, GREATERTHAN, GREATERTHANOREQUALTO, 
									EQUALTO, NOTEQUALTO } RELATIONAL_OPERATOR;
typedef enum logicalOperator { and = 0, or, not } LOGICAL_OPERATOR;

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

Node logicalOperation(Node n1, Node n2, LOGICAL_OPERATOR operation);

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

Node relationalOperation(Node n1, Node n2, int operation);

Node lessThan(Node n1, Node n2, Node result);
void lessThanInteger(Node n1, Node n2, Node result);
void lessThanFloating(Node n1, Node n2, Node result);
void lessThanString(Node n1, Node n2, Node result);
void lessThanBoolean(Node n1, Node n2, Node result);

Node lessThanOrEqualTo(Node n1, Node n2, Node result);
void lessThanOrEqualToInteger(Node n1, Node n2, Node result);
void lessThanOrEqualToFloating(Node n1, Node n2, Node result);
void lessThanOrEqualToString(Node n1, Node n2, Node result);
void lessThanOrEqualToBoolean(Node n1, Node n2, Node result);

Node greaterThan(Node n1, Node n2, Node result);
void greaterThanInteger(Node n1, Node n2, Node result);
void greaterThanFloating(Node n1, Node n2, Node result);
void greaterThanString(Node n1, Node n2, Node result);
void greaterThanBoolean(Node n1, Node n2, Node result);

Node greaterThanOrEqualTo(Node n1, Node n2, Node result);
void greaterThanOrEqualToInteger(Node n1, Node n2, Node result);
void greaterThanOrEqualToFloating(Node n1, Node n2, Node result);
void greaterThanOrEqualToString(Node n1, Node n2, Node result);
void greaterThanOrEqualToBoolean(Node n1, Node n2, Node result);

Node equalTo(Node n1, Node n2, Node result);
void equalToInteger(Node n1, Node n2, Node result);
void equalToFloating(Node n1, Node n2, Node result);
void equalToString(Node n1, Node n2, Node result);
void equalToBoolean(Node n1, Node n2, Node result);

Node notEqualTo(Node n1, Node n2, Node result);
void notEqualToInteger(Node n1, Node n2, Node result);
void notEqualToFloating(Node n1, Node n2, Node result);
void notEqualToString(Node n1, Node n2, Node result);
void notEqualToBoolean(Node n1, Node n2, Node result);

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