#ifndef NODE_H
#define NODE_H

enum type { integer = 0, floating, string };
enum binaryOperator { addition = 0, subtraction, multiplication, division };
typedef enum type TYPE;
typedef enum binaryOperator BINARY_OPERATOR; 

struct node
{
	char* name;
	TYPE type;
	void* value;
	int dataSize;
};

typedef struct node node;
typedef node * Node;

int cmpFunction(void* node1, void* node2);
void yyerror(char const *s);
Node newNode(char* name, TYPE type, void* value);
void assignValue(Node n, void* value);
void assignSize(Node n, void* value);
void printByValue(node n);
Node binaryOperation(Node typeDefiner, Node typeBetrayer, int operation);

Node addByType(Node typeDefiner, Node typeBetrayer);
Node subtractByType(Node typeDefiner, Node typeBetrayer);
Node multiplyByType(Node typeDefiner, Node typeBetrayer);
Node divideByType(Node typeDefiner, Node typeBetrayer);
Node UMinusByType(Node n);

Node toString(Node n);
Node toInteger(Node n);
Node toFloating(Node n);

#endif