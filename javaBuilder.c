#include "javaBuilder.h"
#include <stdio.h>
#include <string.h>

void createJavaVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
		case boolean:
			printf("int %s = %s;", name, expression->name);
			break;

		case floating:
			printf("double %s = %s;", name, expression->name);
			break;

		case string:
			printf("String %s = %s;", name, (char *)expression->value);
			break;
	}
}

void assignJavaVar(struct node *var, struct node *newValue) {
	switch(var->type) {
		case integer:
		case boolean:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case floating:
			printf("%s = %s;",var->name, newValue->name);
			break;

		case string:
			printf("%s = %s;", var->name, (char *)newValue->value);
			break;
	}
}

void createConstantJavaVar(int type, char *name, struct node *expression) {
	switch(type) {
		case integer:
			printf("private static final int %s = %s;", name, expression->name);

		case boolean:
			printf("private static final boolean %s = %s;", name, expression->name);
			break;

		case floating:
			printf("private static final double %s = %s;", name, expression->name);
			break;

		case string:
			printf("private static final String %s = %s);", name, expression->name);
		
			break;
	}

}

void generateJavaExitCode() {
	printf("System.exit(0);");
}

void generateJavaCodeStart() {
	printf("import java.util.*;\nimport java.lang.*;\n");
	printf("public class Main {\n");
	printf("public static coid main(String args[]) {\n");
}

void generateJavaCodeEnd() {
	printf("}}");
}
