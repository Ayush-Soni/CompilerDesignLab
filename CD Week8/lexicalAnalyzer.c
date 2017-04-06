#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define TOKEN_TYPE_SPCL_CHAR 1
#define TOKEN_TYPE_CONSTANT 2
#define TOKEN_TYPE_OPERATOR 3
#define TOKEN_TYPE_MATH_OP 4
#define TOKEN_TYPE_KEYWORD 5
#define TOKEN_TYPE_ID 6

typedef struct {
	int tokenType;
	char *tokenString;
}token;

bool isKeyword(char *buffer) {
	char  *keywords[] = { "auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};
	for(int i=0; i<32; i++) {
		if(strcmp(buffer,keywords[i])==0)
			return true;
	}
	return false;
}

bool isAlphabet(char c) {
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

bool isDigit(char c) {
	return (c>='0' && c<='9');
}

bool isAlphanumeric(char c) {
	return (isAlphabet(c) || isDigit(c));
}

bool isOperator(char c) {
	char operators[] = {'<','>','=','!'};
	for(int i=0; i<4; i++) {
		if(c==operators[i])
			return true;
	}
	return false;
}

bool isMathOperator(char c) {
	char mathOps[] = {'+','-','*','/','%'};
	for(int i=0; i<5; i++) {
		if(c==mathOps[i])
			return true;
	}
	return false;
}

bool isSpecialCharacter(char c) {
	char specialCharacters[] = {'[',']','(',')','{','}',',',';','\t','\n'}; //removed space
	for(int i=0; i<8; i++) {
		if(c==specialCharacters[i])
			return true;
	}
	return false;
}

bool isNumber(char* buffer) {
	for(int i=0;i<(int)strlen(buffer);i++) {
		if(!isDigit(buffer[i]))
			return false;
	}
	return true;
}

bool isIdentifier(char *buffer) {
	if(strlen(buffer)>0 && isAlphanumeric(buffer[0])) {
		int length = strlen(buffer);
		int i=0;
		while(i<length) {
			if(!isAlphanumeric(buffer[i++]))
				return false;
		}
		return true;
	}
	else
		return false;
}

token* formToken(char* buffer, int* tokenCount, int* i) {
	token* newToken = (token*)calloc(1,sizeof(token));
	newToken->tokenString = (char*)calloc(sizeof(buffer),sizeof(char));
	strcpy(newToken->tokenString, buffer);
	if(strlen(buffer)==0||strcmp(buffer,"")==0) {
		*i=0;
		return NULL;
	}
	if(strlen(buffer)==1 && (buffer[0]!=' ' && buffer[0]!='\t')) {
		if(isSpecialCharacter(buffer[0])) {
			newToken->tokenType = TOKEN_TYPE_SPCL_CHAR;
		}
		else if(isMathOperator(buffer[0])) {
			newToken->tokenType = TOKEN_TYPE_MATH_OP;
		}
		else if(isOperator(buffer[0])) {
			newToken->tokenType = TOKEN_TYPE_OPERATOR;
		}
	}
	if(isKeyword(buffer)) {
		newToken->tokenType = TOKEN_TYPE_KEYWORD;
	}
	else if(isNumber(buffer)) {
		newToken->tokenType = TOKEN_TYPE_CONSTANT;
	}
	else if(isIdentifier(buffer)) {
		newToken->tokenType = TOKEN_TYPE_ID;
	}
	bzero(buffer,sizeof(buffer));
	(*tokenCount)++;
	*i=0;
	return newToken;
}

void printTokens(token** tokens, int tokenCount) {
	printf("\nTokenCount: %d",tokenCount);
	printf("\nType\tString\n");
	int i=0;
	while(i<tokenCount) {
		printf(" %d\t%s\n",tokens[i]->tokenType,tokens[i]->tokenString);
		i++;
	}
}

token** lexicalAnalysis(FILE* myFile) {
	char* buffer = (char*)calloc(100, sizeof(char));
	token** tokens = (token**)calloc(1000,sizeof(token*));
	char characterRead;
	int i=0, tokenCount=0;
	while((characterRead=getc(myFile))!=EOF) {
		if(characterRead=='#') {
			while((characterRead=getc(myFile))!='\n');
		}
		else if(characterRead=='/') {
			char nextChar = getc(myFile);
			if(nextChar=='/') {
				while((characterRead=getc(myFile))!='\n');
			}
			else if(nextChar=='*') {
				char previousCharacter;
				while(true) {
					previousCharacter = characterRead;
					characterRead=getc(myFile);
					if(characterRead=='/'&&previousCharacter=='*')
						break;
				}
			}/* WTF
			else {
				tokens[tokenCount] = formToken(buffer, &tokenCount, &i);
				tokens[tokenCount] = formToken(&characterRead, &tokenCount, &i);
				fseek(myFile,-1,SEEK_CUR);
			}*/
		}
		else if(characterRead=='"') {
			while((characterRead=getc(myFile))!='"');
		}
		else if(characterRead==' ') {
			tokens[tokenCount] = formToken(buffer, &tokenCount, &i);
		}
		else if(isSpecialCharacter(characterRead)||isOperator(characterRead)||isMathOperator(characterRead)) {
			tokens[tokenCount] = formToken(buffer, &tokenCount, &i);
			buffer[0]=characterRead; buffer[1]='\0';
			tokens[tokenCount] = formToken(buffer, &tokenCount, &i);
		}
		else if(isAlphabet(characterRead)) {
			if(isDigit(buffer[0])) {
				tokens[tokenCount] = formToken(buffer, &tokenCount, &i);
			}
			buffer[i]=characterRead;
			i+=1;
		}
		else if(isDigit(characterRead)) {
			buffer[i++]=characterRead;
		}
	}
	printTokens(tokens, tokenCount);
	return tokens;
}

token** lexicalAnalyzer() {
	FILE* myFile = NULL;
	char* str = NULL;
	do {
		str=(char*)calloc(20,sizeof(char));
		printf("\nEnter the name of the file to be analyzed: ");
		scanf("%s",str);
		myFile = fopen(str,"r");
	}while(myFile==NULL);
	return lexicalAnalysis(myFile);
}
/*
int main() {
	token** lexicalAnalysisOutput = lexicalAnalyzer();
	return 0;
}
*/
