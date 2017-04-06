#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool T(char*,char**);
bool T1(char*,char**);
bool E(char*,char**);
bool E1(char*,char**);
bool F(char*,char**);


bool E(char *str, char **lookahead) {
	printf("INSIDE E\n");
	return T(str, lookahead)&&E1(str, lookahead);
}

bool T(char *str, char **lookahead) {
	printf("INSIDE T\n");
	return F(str, lookahead)&&T1(str, lookahead);
}

bool E1(char *str, char **lookahead) {
	printf("INSIDE E1\n");
	if(**lookahead=='+') {
		printf("Matched +\n");
		(*lookahead)++;
		return T(str, lookahead)&&E1(str, lookahead);
	}
	else
		return true;
}

bool T1(char *str, char **lookahead) {
	printf("INSIDE T1\n");
	if(**lookahead=='*') {
		printf("Matched *\n");
		(*lookahead)++;
		return F(str, lookahead)&&T1(str, lookahead);
	}
	else
		return true;
}

bool F(char *str, char **lookahead) {
	printf("INSIDE F\n");
	if(**lookahead=='(') {
		printf("Matched (\n");
		(*lookahead)++;
		if(E(str, lookahead)) {
			printf("BACK FROM E\n");
				if(**lookahead==')') {
					printf("Matched )\n");
					(*lookahead)++;
					return true;
				}
		}
		return false;
	}
	else if(**lookahead=='x') {
		printf("Matched x\n");
		(*lookahead)++;
		return true;
	}
	else
		return false;
}


int main() {
	printf("\nEnter the input string: ");
	char *str = (char*) calloc(50, sizeof(char));
	char *lookahead = str;
	scanf("%s",str);
	printf("\nString present in grammar? %d",E(str, &lookahead));
	return 0;
}
