#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lexicalAnalyzer.c"
//#include <lexicalAnalyzer.h>
//TOKEN_TYPE_ID
//TOKEN_TYPE_KEYWORD
//TOKEN_TYPE_SPCL_CHAR
//TOKEN_TYPE_CONSTANT
//TOKEN_TYPE_OPERATOR
//TOKEN_TYPE_MATH_OP
//If it doesn't match, and we return true anyway, that is, epsilon return, we need to send back the original lookahead, rather than the one which is changed from unsuccessfull calls
bool declaration(int* , token**);
bool dataType(int* , token**);
bool identifierList(int* , token**);
bool X(int*, token**);
bool Y(int*, token**);
bool assignmentStatement(int* , token**);

//Added for L8
bool statementList(int*, token**);
bool statement(int*, token**);
bool expression(int*, token**);
bool expressionPrime(int*, token**);
bool termPrime(int*, token**);
bool addOperator(int*, token**);
bool simpleExpression(int*, token**);
bool simpleExpressionPrime(int*, token**);
bool term(int*, token**);
bool factor(int*, token**);
bool relOperator(int*, token**);
bool mulOperator(int*, token**);

//Added for L9
bool decisionStatement(int*, token**);
bool decisionStatementPrime(int*, token**);

//Added for L10
bool loopingStatement(int*, token**);

bool program(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID && strcmp(myTokens[*lookahead]->tokenString, "main")==0) {
		printf("\n>>>>>Matched main");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "(")==0) {
			(*lookahead)++;
			printf("\n>>>>>Matched (");
			if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ")")==0) {
				(*lookahead)++;
				printf("\n>>>>>Matched )");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
					printf("\n>>>>>Matched {");
					(*lookahead)++;
					if(declaration(lookahead, myTokens) && statementList(lookahead, myTokens)) {	//Modified for L8
						printf("\n[DEBUG] Back from declaration() and statementList()");
						if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString,"}")==0) {
							printf("\n>>>>>Matched }");
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

bool dataType(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_KEYWORD && (strcmp(myTokens[*lookahead]->tokenString,"int")==0 || strcmp(myTokens[*lookahead]->tokenString,"char")==0)) {
		printf("\n>>>>>Matched dataType");
		(*lookahead)++;
		return true;
	}
	else
		return false;
}


bool identifierList(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID) {
		printf("\n>>>>>Matched identifier in identifierList()");
		(*lookahead)++;
		return X(lookahead, myTokens);
	}
	else
		return false;
}

bool X(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ",")==0) {
		printf("\n>>>>>Matched ,");
		(*lookahead)++;
		return identifierList(lookahead, myTokens);
	}
	else if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "[")==0) {
		printf("\n>>>>>Matched [");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_CONSTANT) {
			printf("\n>>>>>Matched constant");
			(*lookahead)++;
			if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "]")==0) {
				printf("\n>>>>>Matched ]");
				(*lookahead)++;
				return Y(lookahead, myTokens);
			}
		}
		else
			return false;
	}

	else
		return true;
}

bool Y(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ",")==0) {
		printf("\n>>>>>Matched ,");
		(*lookahead)++;
		return identifierList(lookahead, myTokens);
	}
	else
		return true;
}

bool declaration(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(dataType(lookahead, myTokens)&&identifierList(lookahead,myTokens)) {
		printf("\n[DEBUG] Back from dataType() and identifierList()");
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
			printf("\n>>>>>Matched ;");
			(*lookahead)++;
			return declaration(lookahead, myTokens);
		}
		else {
			printf("\nError: Expected semicolon.");
			return false;
		}
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool assignmentStatement(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_ID) {
		printf("\n>>>>>Matched identifier");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\n>>>>>Matched =");
			(*lookahead)++;
			return expression(lookahead, myTokens);
		}
		else
			return false;
	}
	else
		return false;
}

bool statement(int* lookahead, token** myTokens) {
	if(assignmentStatement(lookahead, myTokens)) {
		printf("\n[DEBUG] Back from assignmentStatement()");
		if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
			printf("\n>>>>>Matched ;");
			(*lookahead)++;
			return true;
		}
		else
			return false;
	}
	else if(decisionStatement(lookahead, myTokens)) {
		printf("\n[DEBUG] Back from decisionStatement(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
		return true;
	}
	else if(loopingStatement(lookahead, myTokens)) {
		printf("\n[DEBUG] Back from loopingStatement()");
		return true;
	}
	else
		return false;
}

bool decisionStatement(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_KEYWORD && strcmp(myTokens[*lookahead]->tokenString, "if")==0) {
		printf("\n>>>>>Matched if");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "(")==0) {
			printf("\n>>>>>Matched (");
			(*lookahead)++;
			if(expression(lookahead, myTokens)) {
				printf("\n[DEBUG] Back from expression()");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ")")==0) {
					printf("\n>>>>>Matched )");
					(*lookahead)++;
					if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
						printf("\n>>>>>Matched {");
						(*lookahead)++;
						if(statementList(lookahead, myTokens)) {
							printf("\n[DEBUG] Back from statementList()");
							if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "}")==0) {
								printf("\n>>>>>Matched }");
								(*lookahead)++;
								return decisionStatementPrime(lookahead, myTokens);
							}
						}
					}
				}
			}
		}
	}
	else
		return false;
}

bool decisionStatementPrime(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_KEYWORD && strcmp(myTokens[*lookahead]->tokenString, "else")==0) {
		printf("\n>>>>>Matched else");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
			printf("\n>>>>>Matched {");
			(*lookahead)++;
			if(statementList(lookahead, myTokens)) {
				printf("\n[DEBUG] Back from statementList(), now in decisionStatementPrime()");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "}")==0) {
					printf("\n>>>>>Matched }");
					(*lookahead)++;
					return true;
				}
			}
		}
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool statementList(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(statement(lookahead, myTokens)) {
		printf("\n[DEBUG] Back from statement(), now in statementList()");
		return statementList(lookahead, myTokens);
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool expression(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside expression(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	if(simpleExpression(lookahead, myTokens) && expressionPrime(lookahead, myTokens)) {
		return true;
	}
	else
		return false;
}

bool expressionPrime(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside expressionPrime(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	int* originalLookahead = lookahead;
	if(relOperator(lookahead, myTokens) && simpleExpression(lookahead, myTokens))
		return true;
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool simpleExpression(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside simpleExpression(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	return term(lookahead, myTokens) && simpleExpressionPrime(lookahead, myTokens);
}

bool simpleExpressionPrime(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside simpleExpressionPrime(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	int* originalLookahead = lookahead;
	if(addOperator(lookahead, myTokens) && term(lookahead, myTokens) && simpleExpressionPrime(lookahead, myTokens))
		return true;
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool term(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside term(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	return factor(lookahead, myTokens)&&termPrime(lookahead, myTokens);
}

bool factor(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside factor(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_ID || myTokens[*lookahead]->tokenType==TOKEN_TYPE_CONSTANT) {
		printf("\n>>>>>Matched ID or constant");
		(*lookahead)++;
		return true;
	}
	else return false;
}

bool termPrime(int* lookahead, token** myTokens) {
	printf("\n[DEBUG] Inside termPrime(), type:%d, str:%s",myTokens[*lookahead]->tokenType,myTokens[*lookahead]->tokenString);
	int* originalLookahead = lookahead;
	if(mulOperator(lookahead, myTokens) && factor(lookahead, myTokens) && termPrime(lookahead, myTokens)) {
		return true;
	}
	else {
		lookahead = originalLookahead;
		return true;
	}
}

bool relOperator(int* lookahead, token** myTokens) {
	int* originalLookahead = lookahead;
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
		printf("\n>>>>>Matched =, attempting =");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\n>>>>>Matched ==");
			(*lookahead)++;
			return true;
		}
		else {
			lookahead = originalLookahead;
			return false;
		}
	}
	else if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "!")==0) {
		printf("\n>>>>>Matched !, attempting =");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\n>>>>>Matched !=");
			(*lookahead)++;
			return true;
		}
		else {
			lookahead = originalLookahead;
			return false;
		}
	}
	else if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "<")==0) {
		printf("\n>>>>>Matched <, attempting =");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\n>>>>>Matched <=");
			(*lookahead)++;
			return true;
		}
		else {
			printf("\n>>>>>Matched only <");
			return true;
		}
	}
	else if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, ">")==0) {
		printf("\n>>>>>Matched >, attempting =");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_OPERATOR && strcmp(myTokens[*lookahead]->tokenString, "=")==0) {
			printf("\n>>>>>Matched >=");
			(*lookahead)++;
			return true;
		}
		else {
			printf("\n>>>>>Matched only >");
			return true;
		}
	}
	else
		return false;
}


bool addOperator(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_MATH_OP && (strcmp(myTokens[*lookahead]->tokenString, "+")==0 || strcmp(myTokens[*lookahead]->tokenString, "-")==0)) {
		printf("\n>>>>>Matched + or -");
		(*lookahead)++;
		return true;
	}
	else
		return false;
}

bool mulOperator(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_MATH_OP && (strcmp(myTokens[*lookahead]->tokenString, "*")==0 || strcmp(myTokens[*lookahead]->tokenString, "/")==0 || strcmp(myTokens[*lookahead]->tokenString, "%")==0)) {
		printf("\n>>>>>Matched * or / or MOD_OP");
		(*lookahead)++;
		return true;
	}
	else
		return false;
}

bool loopingStatement(int* lookahead, token** myTokens) {
	if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_KEYWORD && strcmp(myTokens[*lookahead]->tokenString,"while")==0) {
		printf("\n>>>>>Matched while");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "(")==0) {
			printf("\n>>>>>Matched (");
			(*lookahead)++;
			if(expression(lookahead, myTokens)) {
				printf("\n[DEBUG] Back from expression(), in looping statement");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ")")==0) {
					printf("\n>>>>>Matched )");
					(*lookahead)++;
					if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
						printf("\n>>>>>Matched {");
						(*lookahead)++;
						if(statementList(lookahead, myTokens)) {
							printf("\n[DEBUG] Back from statementList()");
							if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "}")==0) {
								printf("\n>>>>>Matched }");
								(*lookahead)++;
								return true;
							}
						}
					}
				}
			}
		}
	}
	else if(myTokens[*lookahead]->tokenType==TOKEN_TYPE_KEYWORD && strcmp(myTokens[*lookahead]->tokenString,"for")==0) {
		printf("\n>>>>>Matched for");
		(*lookahead)++;
		if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "(")==0) {
			printf("\n>>>>>Matched (");
			(*lookahead)++;
			if(assignmentStatement(lookahead, myTokens)) {
				printf("\n[DEBUG] Back from assignmentStatement(), in loopingStatement()");
				if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
					printf("\n>>>>>Matched ;");
					(*lookahead)++;
					if(expression(lookahead, myTokens)) {
						printf("\n[DEBUG] Back from statementList(), in expression()");
						if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ";")==0) {
							printf("\n>>>>>Matched ;");
							(*lookahead)++;
							if(assignmentStatement(lookahead, myTokens)) {
								printf("\n[DEBUG] Back from assignmentStatement(), in loopingStatement()");
								if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, ")")==0) {
									printf("\n>>>>>Matched )");
									(*lookahead)++;
									if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "{")==0) {
										printf("\n>>>>>Matched {");
										(*lookahead)++;
										if(statementList(lookahead, myTokens)) {
											printf("\n[DEBUG] Back from statementList()");
											if(myTokens[*lookahead]->tokenType == TOKEN_TYPE_SPCL_CHAR && strcmp(myTokens[*lookahead]->tokenString, "}")==0) {
												printf("\n>>>>>Matched }");
												(*lookahead)++;
												return true;
											}
										}
									}
								}
							}
						}
					}
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
		printf("\nParsing Error!");
	printf("\n_____________\n");
	return 0;
}
