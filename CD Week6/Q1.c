#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool T(char*,char**);
bool T1(char*,char**);


bool S(char *str, char **lookahead) {
	if(**lookahead=='a') {
		printf("Matched a\n");
		(*lookahead)++;
		return true;
	}
	else if(**lookahead=='>') {
		printf("Matched >\n");
		(*lookahead)++;
		return true;
	}
	else {
		if(**lookahead=='(') {
			printf("Matched (\n");
			(*lookahead)++;
			if(T(str, lookahead)) {
				printf("BACK FROM T\n");
				if(**lookahead==')') {
					printf("Matched )\n");
					(*lookahead)++;
					return true;
				}
			}
		}
		return false;
	}
}

bool T(char *str, char **lookahead) {
	printf("INSIDE T\n");
	return S(str, lookahead)&&T1(str, lookahead);
}

bool T1(char *str, char **lookahead) {
	if(**lookahead==',') {
		printf("Matched ,\n");
		(*lookahead)++;
		return S(str, lookahead)&&T1(str, lookahead);
	}
	else
		return true;
}

int main() {
	printf("\nEnter the input string: ");
	char *str = (char*) calloc(50, sizeof(char));
	char *lookahead = str;
	scanf("%s",str);
	bool flag = S(str, &lookahead);//&& (lookahead==&str[strlen(str)-1]);
	printf("\nString present in grammar? %d",flag);
	return 0;
}
