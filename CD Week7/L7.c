#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lexicalAnalyzer.c"
//TOKEN_TYPE_ID
//TOKEN_TYPE_KEYWORD
//TOKEN_TYPE_SPCL_CHAR
//TOKEN_TYPE_CONSTANT
bool declaration(int* , token**);
bool dataType(int* , token**);
bool identifierList(int* , token**);
bool X(int*, token**);
bool assignmentStatement(int* , token**);

bool program(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID && strcmp(myTokens[*lookahead]->tokenString, "main")==0) {
		printf("\nMatched main");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "(")==0) {
			(*lookahead)++;
			printf("\nMatched (");
			if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ")")==0) {
				(*lookahead)++;
				printf("\nMatched )");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
					printf("\nMatched {");
					(*lookahead)++;
					if(declaration(lookahead, myTokens) && assignmentStatement(lookahead, myTokens)) {
						printf("\nBack from declaration() and assignmentStatement()");
						if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString,"}")==0) {
							printf("\nMatched }");
							(*lookahead)++;
							return true;
						}
					}
				}
			}
		}
	}
	else return false;
}

bool declaration(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(dataType(lookahead, myTokens)&&identifierList(lookahead,myTokens)) {
		printf("\nBack from dataType() and identifierList()");
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
			printf("\nMatched ;");
			(*lookahead)++;
			return declaration(lookahead, myTokens);
		}
		else
			return false;	//Is this valid? ... Mostly
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool dataType(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_KEYWORD && (strcmp(myTokens[*lookahead]->tokenString,"int")==0 || strcmp(myTokens[*lookahead]->tokenString,"char")==0)) {
		printf("\nMatched dataType");
		(*lookahead)++;
		return true;
	}
	else
		return false;
}

bool identifierList(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID) {
		printf("\nMatched identifier");
		(*lookahead)++;
		return X(lookahead, myTokens);
	}
	else
		return false;
}

bool X(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ",")==0) {
		printf("\nMatched ,");
		(*lookahead)++;
		return identifierList(lookahead, myTokens);
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool assignmentStatement(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID) {
		printf("\nMatched identifier");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\nMatched =");
			(*lookahead)++;
			if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID || myTokens[*lookahead]->tokenType == TOKEN_TYPE_CONSTANT) {
				printf("\nMatched identifier or number");
				(*lookahead)++;
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
					printf("\nMatched ;");
					(*lookahead)++;
					return true;
				}
			}
		}
	}
	else
		return false;
}

int main() {
	/*FILE* myFile = NULL;

	do {
		printf("\nEnter the file name: ");
		char *str = (char*)calloc(20,sizeof(char));
		scanf("%s",str);
		myFile = fopen(str,"r");
	}while(myFile==NULL);

	*/
	token** myTokens = NULL;
	myTokens = lexicalAnalyzer();

	if(myTokens==NULL) {
		printf("\nNo tokens generated.");
		return -1;
	}

	int* lookahead = calloc(1,sizeof(int));
	bool parsedSuccessfully = program(lookahead, myTokens);
	if(parsedSuccessfully)
		printf("\nParse success!");
	else
		printf("\nParsing error!");
	printf("\n________________\n");
	return 0;
}
