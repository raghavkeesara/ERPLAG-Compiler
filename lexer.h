/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

#ifndef lexer
#define lexer
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum{
INTEGER,
REAL,
BOOLEAN,
OF,
ARRAY,
START,
END,
DECLARE,
MODULE,
DRIVER,
PROGRAM,
GET_VALUE,
PRINT,
USE,
WITH,
PARAMETERS,
TRUEs,
FALSEs,
TAKES,
INPUT,
RETURNS,
AND,
OR,
FOR,
IN,
SWITCH,
CASE,
BREAK,
DEFAULT,
WHILE,
PLUS,
MINUS,
MUL,
DIV,
LT,
LE,
GE,
GT,
EQ,
NE,
DEF,
ENDDEF,
COLON,
RANGEOP,
SEMICOL,
COMMA,
ASSIGNOP,
SQBO,
SQBC,
BO,
BC,
COMMENTMARK,
DRIVERDEF,
DRIVERENDDEF,
ID,
NUM,
RNUM,
EPSILON,
$
} Tokens;

char t_str[59][15] = 
{
"INTEGER",
"REAL",
"BOOLEAN",
"OF",
"ARRAY",
"START",
"END",
"DECLARE",
"MODULE",
"DRIVER",
"PROGRAM",
"GET_VALUE",
"PRINT",
"USE",
"WITH",
"PARAMETERS",
"TRUEs",
"FALSEs",
"TAKES",
"INPUT",
"RETURNS",
"AND",
"OR",
"FOR",
"IN",
"SWITCH",
"CASE",
"BREAK",
"DEFAULT",
"WHILE",
"PLUS",
"MINUS",
"MUL",
"DIV",
"LT",
"LE",
"GE",
"GT",
"EQ",
"NE",
"DEF",
"ENDDEF",
"COLON",
"RANGEOP",
"SEMICOL",
"COMMA",
"ASSIGNOP",
"SQBO",
"SQBC",
"BO",
"BC",
"COMMENTMARK",
"DRIVERDEF",
"DRIVERENDDEF",
"ID",
"NUM",
"RNUM",
"EPSILON",
"$"
};

typedef union
{
	int num;
	float rnum;
	char id[40];
}data; 

struct Token;
typedef struct Token *tokeninfo;

struct Token{
	int lineNo;	
	int cnt;
	Tokens tokentype;
	data value;
	tokeninfo next;
	tokeninfo prev;
};


#endif

int isDigit(char ch);
int isLowerChar(char ch);
int isUpperChar(char ch);
int isAlpha(char ch);
int isWhitespace(char ch);
int ignoreComment(FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile,int check);
void charMatcher(FILE *fp);
void addtoList(int caseno);
void addNUMtoList(int val, int count);
void addRNUMtoList(float val, int count);
void addIDtoList(char *val);
void lexicalOutput(char *filename);
int power(int n);
tokeninfo getNextToken();
