#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <lexicalAnalyzer.h>
//TOKEN_TYPE_ID
//TOKEN_TYPE_KEYWORD
//TOKEN_TYPE_SPCL_CHAR
//TOKEN_TYPE_CONSTANT
bool declaration(token* , token**);
bool dataType(token* , token**);
bool identifierList(token* , token**);
bool X(token*, token**);
bool Y(token*, token**);
bool assignmentStatement(token* , token**);

//Added for L8
bool statementList(token*, token**);
bool statement(token*, token**);
bool expression(token*, token**);
bool addOperation(token*, token**);

bool program(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_ID && strcmp(lookahead->tokenString, "main")==0) {
		printf("\nMatched main");
		lookahead++;
		if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, "(")==0 {
			lookahead++;
			printf("\nMatched (");
			if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, ")")==0 {
				lookahead++;
				printf("\nMatched )");
				if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, "{")==0 {
					printf("\nMatched {");
					lookahead++;
					if(declaration(lookahead, myTokens) && statementList(lookahead, myTokens)) {	//Modified for L8
						printf("\nBack from declaration() and statementList()");
						if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString,"}") {
							printf("\nMatched }");
							lookahead++;
							return true;
						}
					}
				}
			}
		}
	}
	else return false;
}


bool identifierList(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_ID) {
		printf("\nMatched identifier");
		lookahead++;
		return X(lookahead, myTokens);
	}
	else
		return false;
}

bool X(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, ",")==0) {
		printf("\nMatched ,");
		lookahead++;
		return identifierList(lookahead, myTokens);
	}
	else if(lookahead->tokenType == TOKEN_TYPE_ID) {
		printf("\nMatched identifier");
		lookahead++;
		if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, "[")) {
			printf("\nMatched [");
			lookahead++;
			if(lookahead->tokenType == TOKEN_TYPE_CONSTANT) {
				printf("\nMatched constant");
				lookahead++;
				if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, "]")==0) {
					printf("\nMatched ]");
					lookahead++;
					return Y(lookahead, myTokens);
				}
			}
		}
		else
			return false;
	}
	
	else
		return true;
}

bool Y(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, ",")==0) {
		printf("\nMatched ,");
		lookahead++;
		return identifierList(lookahead, myTokens);
	}
	else
		return true;
}

bool declaration(token* lookahead, token** myTokens) {
	if(dataType(lookahead, myTokens)&&identifierList(lookahead,myTokens)) {
		printf("\nBack from dataType() and identifierList()");
		if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, ";") {
			printf("\nMatched ;");
			lookahead++;
			return declaration(lookahead, myTokens));
		}
		else
			return false;
	}
	else
		return true;
}
/*
bool dataType(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_KEYWORD && (strcmp(lookahead->tokenString,"int")==0 || strcmp(lookahead->tokenString,"char")==0)) {
		printf("\nMatched dataType");
		lookahead++;
		return true;
	}
	else
		return false;
}
*/
bool assignmentStatement(token* lookahead, token** myTokens) {
	if(lookahead->tokenType == TOKEN_TYPE_ID) {
		printf("\nMatched identifier");
		lookahead++;
		if(lookahead->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, "=")==0) {
			printf("\nMatched =");
			lookahead++;
			return expression(lookahead, myTokens);
		}
		else
			return false;
	}
	else
		return false;
}

bool statement(token* lookahead, token** myTokens) {
	if(assignmentStatement(lookahead, myTokens)) {
		printf("\nBack from assignmentStatement()");
		if(lookahead->tokenType==TOKEN_TYPE_SPCL_CHAR && strcmp(lookahead->tokenString, ";")==0) {
			printf("\nMatched ;");
			lookahead++;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool statementList(token* lookahead, token** myTokens) {
	if(statement(lookahead, myTokens)) {
		printf("\nBack from statement()");
		return statementList(lookahead, myTokens);
	}
	else
		return true;
}

bool expression(token* lookahead, token** myTokens) {
	if(simpleExpression(lookahead, myTokens) {
		printf("\nBack from expression()");
		return expressionPrime(lookahead, myTokens);
	}
	else
		return false;
}

int main() {
	FILE* myFile = NULL;
	
	do {
		printf("\nEnter the file name: ");
		char *str = (char*)calloc(20,sizeof(char));
		scanf("%s",str);
		myFile = fopen(str,"r");
	}while(myFile==NULL);
	
	token** myTokens = NULL;
	myTokens = lexicalAnalyzer(myFile);
	
	if(myTokens==NULL) {
		printf("\nNo tokens generated.");
		return -1;
	}
	
	token* lookahead = myTokens[0];
	bool parsedSuccessfully = program(lookahead, myTokens);
	
	return 0;
}
