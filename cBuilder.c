#include "cBuilder.h"
#include <stdio.h>

void createCVar(int type, char *name, struct node *expression) {
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

void assignCVar(struct node *var, struct node *newValue) {
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

void createConstantCVar(int type, char *name, struct node *expression) {
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

void generateCExitCode() {
	printf("exit(0);");
}

void generateCCodeStart() {
	printf("#include <stdio.h>\n#include<stdlib.h>\n#include<string.h>\n");
	printf("int main(void) {");
}

void generateCCodeEnd() {
	printf("return 0;}");
}
