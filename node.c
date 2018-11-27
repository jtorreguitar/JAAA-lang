#include "node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "y.tab.h"

typedef Node (*converterFunction)(Node n1);
typedef void (*typedConverterFunction)(Node n, Node result);
typedef Node (*operationFunction)(Node typeDefiner, Node typeBetrayer);
typedef Node (*typedOperationFunction)(Node n1, Node n2);
typedef Node (*relationalFunction)(Node n1, Node n2, Node result);
typedef void (*typedRelationalFunction)(Node n1, Node n2, Node result);

converterFunction converterFunctions[] = { toInteger, toFloating, toString, toBoolean };
typedConverterFunction toIntegerFunctions[] = { toIntegerFromInteger, toIntegerFromFloating, toIntegerFromString, toIntegerFromBoolean };
typedConverterFunction toFloatingFunctions[] = { toFloatingFromInteger, toFloatingFromFloating, toFloatingFromString, toFloatingFromBoolean };
typedConverterFunction toStringFunctions[] = { toStringFromInteger, toStringFromFloating, toStringFromString, toStringFromBoolean };
typedConverterFunction toBooleanFunctions[] = { toBooleanFromInteger, toBooleanFromFloating, toBooleanFromString, toBooleanFromBoolean };

operationFunction operationFunctions[] = { addByType, subtractByType, multiplyByType, divideByType };
typedOperationFunction additionFunctions[] = { addIntegers, addFloats, addStrings, addBooleans };
typedOperationFunction subtractionFunctions[] = { subtractIntegers, subtractFloats, subtractStrings, subtractBooleans };
typedOperationFunction multiplicationFunctions[] = { multiplyIntegers, multiplyFloats, multiplyStrings, multiplyBooleans };
typedOperationFunction divisionFunctions[] = { divideIntegers, divideFloats, divideStrings, divideBooleans };

relationalFunction relationalFunctions[] = { lessThan, lessThanOrEqualTo, greaterThan, greaterThanOrEqualTo, 
												equalTo, notEqualTo };
typedRelationalFunction lessThanFunctions[] = { lessThanInteger, lessThanFloating, lessThanString, lessThanBoolean };
typedRelationalFunction lessThanOrEqualToFunctions[] = { lessThanOrEqualToInteger, lessThanOrEqualToFloating, lessThanOrEqualToString, 
															lessThanOrEqualToBoolean };
typedRelationalFunction greaterThanFunctions[] = { greaterThanInteger, greaterThanFloating, greaterThanString, greaterThanBoolean };
typedRelationalFunction greaterThanOrEqualToFunctions[] = { greaterThanOrEqualToInteger, greaterThanOrEqualToFloating, 
																greaterThanOrEqualToString, greaterThanOrEqualToBoolean };
typedRelationalFunction equalToFunctions[] = { equalToInteger, equalToFloating, equalToString, equalToBoolean };
typedRelationalFunction notEqualToFunctions[] = { notEqualToInteger, notEqualToFloating, notEqualToString, notEqualToBoolean };


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
	return !strcmp((char*)((Node)node1)->name,(char*)name);
}

Node newNode(char* name, TYPE type, void* value, int constant)
{
	Node n = calloc(1, sizeof(node));
	n->type = type;
	n->constant = constant;
	n->name = calloc(strlen(name) + 1, sizeof(char));
	strcpy(n->name, name);
	assignValue(n, value);
	assignSize(n, value);
	return n;
}

void assignValue(Node n, void* value)
{
	int size = 1;
	int sizeOfMember;
	switch (n->type)
	{
		case integer:
			sizeOfMember = sizeof(int);
			break;
		case floating:
			sizeOfMember = sizeof(float);
			break;
		case string:
			size = strlen((char*) value) + 1;
			sizeOfMember = sizeof(char);
			break;
		case boolean:
			sizeOfMember = sizeof(int);
			break;
	}
	n->value = calloc(size, sizeOfMember);
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
		case boolean:
			n->dataSize = sizeof(int);
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
		case boolean:
			printf(">> %s\n", (*(int*)n.value) == 0 ? "false":"true");
	}
}

// -------------------------- ARITHMETIC OPERATIONS --------------------------

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
	Node result = additionFunctions[typeDefiner->type](typeDefiner, auxBetrayer);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node addIntegers(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = integer;
	int auxint = (*((int*)n1->value)) + (*((int*)n2->value));
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	memcpy(result->value, (void*)&auxint, sizeof(int));
	return result;
}

Node addFloats(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = floating;
	double auxdouble = (*((double*)n1->value)) + (*((double*)n2->value));
	result->dataSize = sizeof(double);
	result->value = calloc(1, sizeof(double));
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
	return result;
}

Node addStrings(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = string;
	result->value = calloc(n1->dataSize + n2->dataSize - 1, sizeof(char));
	memcpy(result->value, n1->value, n1->dataSize - 1);
	result->value = strcat((char*)result->value, (char*)n2->value);
	result->dataSize = n1->dataSize + n2->dataSize - 1;
	return result;
}

Node addBooleans(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	int val1 = *(int*)n1->value;
	int val2 = *(int*)n2->value;
	int finalVal;
	if(val1 + val2 > 0)
		finalVal = 1;
	else
		finalVal = 0;
	memcpy(result->value, &finalVal, sizeof(int));
	return result;
}

Node subtractByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = subtractionFunctions[typeDefiner->type](typeDefiner, auxBetrayer);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node subtractIntegers(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = integer;
	int auxint = (*((int*)n1->value)) - (*((int*)n2->value));
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	memcpy(result->value, (void*)&auxint, sizeof(int));
	return result;
}

Node subtractFloats(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = floating;
	double auxdouble = (*((double*)n1->value)) - (*((double*)n2->value));
	result->dataSize = sizeof(double);
	result->value = calloc(1, sizeof(double));
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
	return result;
}

Node subtractStrings(Node n1, Node n2)
{
	yyerror("invalid operation exception: strings cannot be subtracted");
	return NULL;
}

Node subtractBooleans(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	int val1 = *(int*)n1->value;
	int val2 = *(int*)n2->value;
	int finalVal;
	if(val1 - val2 > 0)
		finalVal = 1;
	else
		finalVal = 0;
	memcpy(result->value, &finalVal, sizeof(int));
	return result;
}

Node multiplyByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = multiplicationFunctions[typeDefiner->type](typeDefiner, auxBetrayer);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node multiplyIntegers(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = integer;
	int auxint = (*((int*)n1->value)) * (*((int*)n2->value));
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	memcpy(result->value, (void*)&auxint, sizeof(int));
	return result;
}

Node multiplyFloats(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = floating;
	double auxdouble = (*((double*)n1->value)) * (*((double*)n2->value));
	result->dataSize = sizeof(double);
	result->value = calloc(1, sizeof(double));
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
	return result;
}

Node multiplyStrings(Node n1, Node n2)
{
	yyerror("invalid operation exception: strings cannot be multiplied");
	return NULL;
}

Node multiplyBooleans(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	int val1 = *(int*)n1->value;
	int val2 = *(int*)n2->value;
	val1 *= val2;
	memcpy(result->value, &val1, sizeof(int));
	return result;
}

Node divideByType(Node typeDefiner, Node typeBetrayer)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	Node result = divisionFunctions[typeDefiner->type](typeDefiner, auxBetrayer);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

Node divideIntegers(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	if(*((int*)n1->value) == 0)
	{
		yyerror("division by zero");
		return NULL;
	}
	int auxint = (*((int*)n1->value)) / (*((int*)n2->value));
	result->type = integer;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	memcpy(result->value, (void*)&auxint, sizeof(int));
	return result;
}

Node divideFloats(Node n1, Node n2)
{
	Node result = calloc(1, sizeof(node));
	result->type = floating;
	if(*((double*)n1->value) == 0)
	{
		yyerror("division by zero");
		return NULL;
	}
	double auxdouble = (*((double*)n1->value)) / (*((double*)n2->value));
	result->dataSize = sizeof(double);
	result->value = calloc(1, sizeof(double));
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
	return result;
}

Node divideStrings(Node n1, Node n2)
{
	yyerror("invalid operation exception: strings cannot be divided");
	return NULL;
}

Node divideBooleans(Node n1, Node n2)
{
	yyerror("invalid operation exception: booleans cannot be divided");
	/*
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	int val1 = *(int*)n1->value;
	int val2 = *(int*)n2->value;
	val1 *= val2;
	memcpy(result->value, &val1, sizeof(int));
	*/
	return NULL;
}

// -------------------------- RELATIONAL OPERATIONS --------------------------

Node relationalOperation(Node n1, Node n2, int operation)
{
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	return relationalFunctions[operation](n1, n2, result);
}

Node lessThan(Node typeDefiner, Node typeBetrayer, Node result)
{
	// I gotta do this check cause otherwise if the betrayer gets truncated it might end up being equal.
	if(typeDefiner->type == integer && typeBetrayer->type == floating)
	{
		int auxint = *(int*)typeDefiner->value;
		double auxdouble = *(double*)typeBetrayer->value;
		int val = auxint < auxdouble ? 1:0;
		memcpy(result->value, (void*) &val, sizeof(int));
		return result;
	}
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	lessThanFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void lessThanInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 < auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void lessThanFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 < auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void lessThanString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) < 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void lessThanBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 < auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

Node lessThanOrEqualTo(Node typeDefiner, Node typeBetrayer, Node result)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	lessThanOrEqualToFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void lessThanOrEqualToInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 <= auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void lessThanOrEqualToFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 <= auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void lessThanOrEqualToString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) <= 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

// this function, unfortunately, will always return true, however, we gotta include it
// for consistency and I can't think of a better definition that'd be consistent.
void lessThanOrEqualToBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 <= auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

Node greaterThan(Node typeDefiner, Node typeBetrayer, Node result)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	greaterThanFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void greaterThanInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 > auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 > auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) > 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 > auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

Node greaterThanOrEqualTo(Node typeDefiner, Node typeBetrayer, Node result)
{
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	greaterThanOrEqualToFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void greaterThanOrEqualToInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 >= auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanOrEqualToFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 >= auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanOrEqualToString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) >= 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void greaterThanOrEqualToBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 >= auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

Node equalTo(Node typeDefiner, Node typeBetrayer, Node result)
{
	// I gotta do this check cause otherwise if the betrayer gets truncated it might end up being equal.
	if(typeDefiner->type == integer && typeBetrayer->type == floating)
	{
		int auxint = *(int*)typeDefiner->value;
		int auxdouble = *(double*)typeBetrayer->value;
		int val = auxint == auxdouble ? 1:0;
		memcpy(result->value, &val, sizeof(int));
	}
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	equalToFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void equalToInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 == auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void equalToFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 == auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void equalToString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) == 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void equalToBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 == auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

Node notEqualTo(Node typeDefiner, Node typeBetrayer, Node result)
{
	// I gotta do this check cause otherwise if the betrayer gets truncated it might end up being equal.
	if(typeDefiner->type == integer && typeBetrayer->type == floating)
	{
		int auxint = *(int*)typeDefiner->value;
		int auxdouble = *(double*)typeBetrayer->value;
		int val = auxint != auxdouble ? 1:0;
		memcpy(result->value, &val, sizeof(int));
	}
	Node auxBetrayer = converterFunctions[typeDefiner->type](typeBetrayer);
	notEqualToFunctions[typeDefiner->type](typeDefiner, auxBetrayer, result);
	free(auxBetrayer->value);
	free(auxBetrayer);
	return result;
}

void notEqualToInteger(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 != auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void notEqualToFloating(Node n1, Node n2, Node result)
{
	double auxdouble1 = *(double*) n1->value;
	double auxdouble2 = *(double*) n2->value;
	int val = auxdouble1 != auxdouble2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void notEqualToString(Node n1, Node n2, Node result)
{
	char *auxstring1 = (char*) n1->value;
	char *auxstring2 = (char*) n2->value;
	int val = strcmp(auxstring1, auxstring2) != 0 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

void notEqualToBoolean(Node n1, Node n2, Node result)
{
	int auxint1 = *(int*) n1->value;
	int auxint2 = *(int*) n2->value;
	int val = auxint1 != auxint2 ? 1:0;
	memcpy(result->value, &val, sizeof(int));
}

// -------------------------- LOGICAL OPERATIONS --------------------------

Node logicalOperation(Node n1, Node n2, LOGICAL_OPERATOR operation)
{
	Node leftSide = toBoolean(n1);
	Node rightSide = NULL;
	if(n2 != NULL)
		rightSide = toBoolean(n2);
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	int val;
	switch (operation)
	{
		case and:
			if ((*(int*)leftSide->value == 1) && (*(int*)rightSide->value == 1))
				val = 1;
			else
				val = 0;
			break;
		case or:
			if((*(int*)leftSide->value == 1) || (*(int*)rightSide->value == 1))
				val = 1;
			else
				val = 0;
			break;
		case not:
			val = *(int*)leftSide->value == 0;
			break;
	}
	memcpy(result->value, (void*) &val, sizeof(int));
	return result;
}

// -------------------------- CONVERSION OPERATIONS --------------------------

Node toInteger(Node n)
{
	Node ret = calloc(1, sizeof(node));
	ret->value = calloc(1, sizeof(int));
	ret->dataSize = sizeof(int);
	ret->type = integer;
	toIntegerFunctions[n->type](n, ret);
	return ret;
}

void toIntegerFromInteger(Node n, Node result)
{
	memcpy(result->value, n->value, sizeof(int));
}

void toIntegerFromFloating(Node n, Node result)
{
	double auxdouble = *((double*)n->value);
	int auxint = (int) auxdouble;
	memcpy(result->value, &auxint, sizeof(int));
}

void toIntegerFromString(Node n, Node result)
{
	int auxint = atoi((char*)n->value);
	memcpy(result->value, (void*)&auxint, sizeof(int));
}

void toIntegerFromBoolean(Node n, Node result)
{
	memcpy(result->value, n->value, sizeof(int));
}

Node toFloating(Node n)
{
	Node ret = calloc(1, sizeof(node));
	ret->value = calloc(1, sizeof(double));
	ret->dataSize = sizeof(double);
	ret->type = floating;
	toFloatingFunctions[n->type](n, ret);
	return ret;
}

void toFloatingFromInteger(Node n, Node result)
{
	int auxint = *((int*)n->value);
	double auxdouble = (double) auxint;
	memcpy(result->value, &auxdouble, sizeof(double));
}

void toFloatingFromFloating(Node n, Node result)
{
	memcpy(result->value, n->value, sizeof(int));
}

void toFloatingFromString(Node n, Node result)
{
	double auxdouble = atof((char*)n->value);
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
}

void toFloatingFromBoolean(Node n, Node result)
{
	int auxint = *(int*)n->value;
	int auxdouble = (double) auxint;
	memcpy(result->value, (void*)&auxdouble, sizeof(double));
}

Node toString(Node n)
{
	Node ret = calloc(1, sizeof(node));
	ret->type = string;
	toStringFunctions[n->type](n, ret);
	return ret;
}

void toStringFromInteger(Node n, Node result)
{
	int digits = floor(log10(abs(*((int*)n->value)))) + 1;
	result->dataSize = (digits + 1)*sizeof(char);
	result->value = calloc(digits + 1, sizeof(char));
	my_itoa(*((int*)n->value), ((char*)result->value));
}

void toStringFromFloating(Node n, Node result)
{
	char* auxstring;
	double auxdouble = *((double*)n->value);
	result->dataSize = ftoa(auxdouble, &auxstring);
	result->value = calloc(result->dataSize, sizeof(char));
	memcpy(result->value, auxstring, result->dataSize);
}

void toStringFromString(Node n, Node result)
{
	result->dataSize = n->dataSize;
	result->value = calloc(n->dataSize, sizeof(char));
	memcpy(result->value, n->value, n->dataSize);
}

void toStringFromBoolean(Node n, Node result)
{
	int auxint = *(int*)n->value;
	char *auxstring;
	if(auxint == 0)
		auxstring = "false";
	else
		auxstring = "true";
	result->dataSize = (strlen(auxstring) + 1);
	result->value = calloc(result->dataSize, sizeof(char));
	memcpy(result->value, auxstring, result->dataSize);
}

Node toBoolean(Node n)
{
	Node result = calloc(1, sizeof(node));
	result->type = boolean;
	result->dataSize = sizeof(int);
	result->value = calloc(1, sizeof(int));
	toBooleanFunctions[n->type](n, result);
	return result;
}

void toBooleanFromInteger(Node n, Node result)
{
	int auxint = *(int*)n->value;
	int value = auxint == 0 ? 0:1;
	memcpy(result->value, (void*) &value, sizeof(int));
}

void toBooleanFromFloating(Node n, Node result)
{
	double auxdouble = *(double*)n->value;
	int value = auxdouble == 0 ? 0:1;
	memcpy(result->value, (void*) &value, sizeof(int));
}

void toBooleanFromString(Node n, Node result)
{
	char *auxstring = (char*)n->value;
	int value;
	if(!strcmp(auxstring, "") || !strcmp(auxstring, "false") || !strcmp(auxstring, "0"))
		value = 0;
	else
		value = 1;
	memcpy(result->value, (void*) &value, sizeof(int));
}

void toBooleanFromBoolean(Node n, Node result)
{
	memcpy(result->value, n->value, sizeof(int));
}

Node UMinusByType(Node n)
{
	Node ret = calloc(1, sizeof(node));
	double auxdouble;
	int auxint;
	switch (n->type)
	{
		case string:
			yyerror("operation exception: unary minus not applicable to string");
			return n;
		case floating:
			ret->value = calloc(1, sizeof(double));
			ret->type = floating;
			auxdouble = -*((double*)n->value);
			memcpy(ret->value, (void*)&auxdouble, sizeof(double));
			break;
		case integer:
			ret->type = integer;
			ret->value = calloc(1, sizeof(int));
			auxint = -*((int*)n->value);
			memcpy(ret->value, (void*)&auxint, sizeof(int));
			break;
		case boolean:
			ret->type = boolean;
			ret->value = calloc(1, sizeof(int));
			auxint = *(int*)n->value == 0;
			memcpy(ret->value, (void*)&auxint, sizeof(int));
	}
	return ret;
}

void createVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("int %s = %d;", name, *(int *)expression->value);
			break;

		case floating:
			printf("float %s = %lf;", name, *(float *)expression->value);
			break;

		case string:
			printf("char *%s = malloc(sizeof(char) * %d);", name,
												expression->dataSize);
			printf("if(%s == NULL){", name);
			printf("fprintf(stderr,\"Cannot allocate memory\");exit(0);}");
			printf("memcpy(%s, \"%s\", %d);", name, (char *)expression->value,
														expression->dataSize);
			break;
	}
}

void assignVar(struct node *var, struct node *newValue) {
	switch(var->type) {
		case integer:
		case boolean:
			printf("%s = %d;",var->name, *(int *)newValue->value);
			break;

		case floating:
			printf("%s = %lf;",var->name, *(float *)newValue->value);
			break;

		case string:
			if(var->dataSize < newValue->dataSize) {
				printf("%s = realloc(%s, sizeof(char) * %d);", var->name,
											var->name, newValue->dataSize);
				printf("if(%s == NULL){", var->name);
				printf("fprintf(stderr,\"Cannot allocate memory\");exit(0);}");
			}

			printf("memcpy(%s, \"%s\", %d);", var->name, (char *)newValue->value,
																newValue->dataSize);                                                  
			
			break;
	}
}

void createConstantVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("const int %s = %d;", name, *(int *)expression->value);
			break;

		case floating:
			printf("const float %s = %lf;", name, *(float *)expression->value);
			break;

		case string:
			printf("const char *%s = %s);", name, (char *)expression->value);
		
			break;
	}

}
