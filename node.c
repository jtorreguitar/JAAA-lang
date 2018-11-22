#include "node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "y.tab.h"

typedef Node (*converterFunction)(Node n1);
typedef Node (*operationFunction)(Node typeDefiner, Node typeBetrayer);
converterFunction converterFunctions[] = { toInteger, toFloating, toString };
operationFunction operationFunctions[] = { addByType, subtractByType, multiplyByType, divideByType };

void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
   exit(1);
 }


int ftoa(double d, char** buffer)
{
	int len = snprintf(NULL, 0, "%f", d);
	*buffer = (char *)calloc(len + 1, sizeof(char));
	snprintf(*buffer, len + 1, "%f", d);
	return len + 1;
}

char *my_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

int cmpFunction(void* node1, void* name)
{
	return strcmp(((Node)node1)->name,(char*)name);
}

Node newNode(char* name, TYPE type, void* value)
{
	Node n = malloc(sizeof(node));
	n->type = type;
	n->name = malloc(strlen(name) + 1);
	strcpy(n->name, name);
	assignValue(n, value);
	assignSize(n, value);
	return n;
}

void assignValue(Node n, void* value)
{
	int size;
	switch (n->type)
	{
		case integer:
			size = sizeof(int);
		case floating:
			size = sizeof(float);
		case string:
			size = strlen((char*) value) + 1;
	}
	n->value = malloc(size);
	memcpy(n->value, value, size);
}

void assignSize(Node n, void* value)
{
	switch (n->type)
	{
		case integer:
			n->dataSize = sizeof(int);
			break;
		case floating:
			n->dataSize = sizeof(double);
			break;
		case string:
			n->dataSize = (strlen((char*) n->value) + 1)*sizeof(char);
			break;
	}
}

void printByValue(node n)
{
	switch (n.type)
	{
		case integer:
			printf(">> %d\n", *((int*)n.value));
			break;
		case floating:
			printf(">> %g\n", *((double*)n.value));
			break;
		case string:
			printf(">> \"%s\"\n", ((char*)n.value));
			break;
	}
}

// for the following methods the typeDefiner will be the one that defines
// the type of the resulting node. The typeBetrayer will be the one that
// changes type if needed.

Node binaryOperation(Node typeDefiner, Node typeBetrayer, int operation)
{
	return operationFunctions[operation](typeDefiner, typeBetrayer);
}

Node addByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = malloc(sizeof(node));
	result->type = typeDefiner->type;
	int auxint;
	double auxdouble;
	switch (typeDefiner->type)
	{
		case integer:
			auxint = (*((int*)typeDefiner->value)) + (*((int*)auxBetrayer->value));
			result->dataSize = sizeof(int);
			result->value = malloc(sizeof(int));
			memcpy(result->value, (void*)&auxint, sizeof(int));
			break;
		case floating:
			auxdouble = (*((double*)typeDefiner->value)) + (*((double*)auxBetrayer->value));
			result->dataSize = sizeof(double);
			result->value = malloc(sizeof(double));
			memcpy(result->value, (void*)&auxdouble, sizeof(double));
			break;
		case string:
			result->value = malloc(typeDefiner->dataSize + auxBetrayer->dataSize - 1);
			memcpy(result->value, typeDefiner->value, typeDefiner->dataSize);
			result->value = strcat((char*)result->value, (char*)auxBetrayer->value);
			result->dataSize = typeDefiner->dataSize + auxBetrayer->dataSize - 1;
			break;
	}
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node subtractByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = malloc(sizeof(node));
	result->type = typeDefiner->type;
	int auxint;
	int auxdouble;
	switch (typeDefiner->type)
	{
		case integer:
			auxint = (*((int*)typeDefiner->value)) - (*((int*)auxBetrayer->value));
			result->dataSize = sizeof(int);
			result->value = malloc(sizeof(int));
			memcpy(result->value, (void*)&auxint, sizeof(int));
			break;
		case floating:
			auxdouble = (*((double*)typeDefiner->value)) - (*((double*)auxBetrayer->value));
			result->dataSize = sizeof(double);
			result->value = malloc(sizeof(double));
			memcpy(result->value, (void*)&auxdouble, sizeof(double));
			break;
		case string:
			yyerror("invalid operation exception: strings cannot be subtracted");
			break;
	}
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node multiplyByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = malloc(sizeof(node));
	result->type = typeDefiner->type;
	int auxint;
	double auxdouble;
	switch (typeDefiner->type)
	{
		case integer:
			auxint = (*((int*)typeDefiner->value)) * (*((int*)auxBetrayer->value));
			result->dataSize = sizeof(int);
			result->value = malloc(sizeof(int));
			memcpy(result->value, (void*)&auxint, sizeof(int));
			break;
		case floating:
			auxdouble = (*((double*)typeDefiner->value)) * (*((double*)auxBetrayer->value));
			result->dataSize = sizeof(double);
			result->value = malloc(sizeof(double));
			memcpy(result->value, (void*)&auxdouble, sizeof(double));
			break;
		case string:
			yyerror("invalid operation exception: strings cannot be multiplied");
			break;
	}
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node divideByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = malloc(sizeof(node));
	result->type = typeDefiner->type;
	double auxdouble;
	int auxint;
	switch (typeDefiner->type)
	{
		case integer:
			if(*((int*)typeBetrayer->value) == 0)
			{
				yyerror("division by zero");
				return typeDefiner;
			}
			auxint = (*((int*)typeDefiner->value)) / (*((int*)auxBetrayer->value));
			result->dataSize = sizeof(int);
			result->value = malloc(sizeof(int));
			memcpy(result->value, (void*)&auxint, sizeof(int));
			break;
		case floating:
			if(*((double*)typeBetrayer->value) == 0)
			{
				yyerror("division by zero");
				return typeDefiner;
			}
			auxdouble = (*((double*)typeDefiner->value)) / (*((double*)auxBetrayer->value));
			result->dataSize = sizeof(double);
			result->value = malloc(sizeof(double));
			memcpy(result->value, (void*)&auxdouble, sizeof(double));
			break;
		case string:
			yyerror("invalid operation exception: strings cannot be divided");
			break;
	}
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node toInteger(Node n)
{
	Node ret = malloc(sizeof(node));
	ret->value = malloc(sizeof(int));
	ret->dataSize = sizeof(int);
	ret->type = integer;
	int auxint;
	double auxdouble;
	switch (n->type)
	{
		case integer:
			memcpy(ret->value, n->value, sizeof(int));
			break;
		case floating:
			auxdouble = *((double*)n->value);
			auxint = (int) auxdouble;
			memcpy(ret->value, &auxint, sizeof(int));
			break;
		case string:
			auxint = atoi((char*)ret->value);
			memcpy(ret->value, (void*)&auxint, sizeof(int));
			break;
	}
	return ret;
}

Node toFloating(Node n)
{
	Node ret = malloc(sizeof(node));
	ret->value = malloc(sizeof(double));
	ret->dataSize = sizeof(double);
	ret->type = floating;
	double auxdouble;
	int auxint;
	switch (n->type)
	{
		case floating:
			memcpy(ret->value, n->value, sizeof(double));
			break;
		case integer:
			auxint = *((int*)n->value);
			auxdouble = (double) auxint;
			memcpy(ret->value, &auxdouble, sizeof(double));
			break;
		case string:
			auxdouble = atof((char*)n->value);
			memcpy(ret->value, (void*)&auxdouble, sizeof(double));
			break;
	}
	return ret;
}

Node toString(Node n)
{
	Node ret = malloc(sizeof(node));
	ret->type = string;
	double auxdouble;
	char * auxstring;
	int digits;
	switch (n->type)
	{
		case string:
			ret->dataSize = n->dataSize;
			ret->value = malloc(n->dataSize*sizeof(char));
			memcpy(ret->value, n->value, n->dataSize);
			break;
		case integer:
			digits = floor(log10(abs(*((int*)n->value)))) + 1;
			ret->dataSize = (digits + 1)*sizeof(char);
			ret->value = calloc(digits + 1, sizeof(char));
			my_itoa(*((int*)n->value), ((char*)ret->value));
			break;
		case floating:
			auxdouble = *((double*)n->value);
			ret->dataSize = ftoa(auxdouble, &auxstring);
			ret->value = calloc(ret->dataSize, sizeof(char));
			memcpy(ret->value, auxstring, ret->dataSize);
			break;
	}
	return ret;
}

Node UMinusByType(Node n)
{
	Node ret = malloc(sizeof(node));
	double auxdouble;
	int auxint;
	switch (n->type)
	{
		case string:
			yyerror("operation exception: unary minus not applicable to string");
			return n;
		case floating:
			ret->value = malloc(sizeof(double));
			auxdouble = -*((double*)n->value);
			memcpy(ret->value, (void*)&auxdouble, sizeof(double));
			break;
		case integer:
			ret->value = malloc(sizeof(int));
			auxint = -*((int*)n->value);
			memcpy(ret->value, (void*)&auxint, sizeof(int));
			break;
	}
	return ret;
}