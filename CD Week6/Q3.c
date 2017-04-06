#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool A(char*,char**);
bool A1(char*,char**);
bool S(char*,char**);
bool B(char*,char**);


bool S(char *str, char **lookahead) {
	printf("INSIDE S\n");
	if(**lookahead=='a') {
		printf("Matched a\n");
		(*lookahead)++;
		if(A(str,lookahead)) {
			if(**lookahead=='c') {
				printf("Matched c\n");
				(*lookahead)++;
				if(B(str,lookahead)) {
					if(**lookahead=='e')
						printf("Matched e\n");
						(*lookahead)++;
						return true;
				}
			}
		}
					/*else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false; */
	}
	else
		return false;
}

bool A(char *str, char **lookahead) {
	printf("INSIDE A\n");
	if(**lookahead=='b') {
		printf("Matched b\n");
		(*lookahead)++;
		return A1(str, lookahead);
	}
	else
		return false;
}

bool A1(char *str, char **lookahead) {
	printf("INSIDE A\n");
	if(**lookahead=='b') {
		printf("Matched b\n");
		(*lookahead)++;
		return A1(str, lookahead);
	}
	else
		return true;
}

bool B(char *str, char **lookahead) {
	printf("INSIDE B\n");
	if(**lookahead=='d') {
		printf("Matched d\n");
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
	printf("\nString present in grammar? %d",S(str, &lookahead));
	return 0;
}
