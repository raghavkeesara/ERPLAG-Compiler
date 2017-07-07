
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

#ifndef parser
#define parser
#include "lexer.h"
#include <string.h>

#define MAX 2000

typedef Tokens terminals;

typedef enum {
program,
moduleDeclarations,
moduleDeclaration,
otherModules,
driverModule,
module,
ret,
input_plist,
n1,
output_plist,
n2,
dataType,
type,
moduleDef,
statements,
statement,
ioStmt,
var,
whichId,
simpleStmt,
assignmentStmt,
whichStmt,
lvalueIDStmt,
lvalueARRStmt,
index1,
moduleReuseStmt,
optional,
idList,
n3,
expression, 
arithmeticOrBooleanExpr,
n7,
anyterm,
n8,
arithmeticExpr,
n4,
term,
n5,
factor,
op1,
op2,
logicalOp,
relationalOp,
declareStmt,
conditionalStmt,
caseStmts,
n9,
value,
default1,
iterativeStmt,
range

} nonterminals;


char nt_str[51][55] = {
"program",
"moduleDeclarations",
"moduleDeclaration",
"otherModules",
"driverModule",
"module",
"ret",
"input_plist",
"n1",
"output_plist",
"n2",
"dataType",
"type",
"moduleDef",
"statements",
"statement",
"ioStmt",
"var",
"whichId",
"simpleStmt",
"assignmentStmt",
"whichStmt",
"lvalueIDStmt",
"lvalueARRStmt",
"index1",
"moduleReuseStmt",
"optional",
"idList",
"n3",
"expression", 
"arithmeticOrBooleanExpr",
"n7",
"anyterm",
"n8",
"arithmeticExpr",
"n4",
"term",
"n5",
"factor",
"op1",
"op2",
"logicalOp",
"relationalOp",
"declareStmt",
"conditionalStmt",
"caseStmts",
"n9",
"value",
"default1",
"iterativeStmt",
"range"
};

/*typedef enum{
DRIVERDEF,
ENDDRIVERDEF,
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
ID,
NUM,
RNUM,
EPSILON,
$
} Tokens;
*/
/*char t_str[59][15] = 
{
"DRIVERDEF",
"ENDDRIVERDEF",
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
"ID",
"NUM",
"RNUM",
"EPSILON",
"$"
};*/

int parseTable[51][59];

/** Grammar data structures **/
struct g_node;
typedef struct g_node* g_nodeptr;

//a general grammar node
typedef struct g_node{
	char name[200];
	int tag;//0 for NT,1 for T
	g_nodeptr next;
}g_node;

g_node* g_array[100];

/** Grammar data structures ends**/

/** First and Follow data structures **/
typedef struct FirstSet
{
	char first[15][15]; //each firstset hopefully has atmax 15terminals.
}first1;

first1 firstSet[51]; //we have to create first set for 51 nonterminals.

typedef struct FollowSet
{
	char follow[15][15];
}follow;

follow followSet[51];

/** First and Follow data structures ends**/

/** stack ADT starts**/

struct stack
{
	int top;
	char* arr[MAX];
}parserStack;

/** stack ADT ends**/

/** Tree ADT starts**/

typedef struct Node node;

struct Node
{
	char buf_name[30];
	int lineNo;
	Tokens tk;
	data val;
	int isLeafNode;
	int t_or_nt;
	int numofchild;
	node* parent;
	node* children;
	node* next;
};

node* createNode(char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf);
node* addSibling(node* n, char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf);
node* addChildren(node* n, char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf);


struct treeStack
{
	int top;
	g_node* arr[MAX];
	int childno[MAX];
}ts;

struct parentStack
{
	int top;
	node* arr[MAX];
}ps;



/** Tree ADT Ends **/

#endif

//gives a pointer to new grammar node created
g_nodeptr get_gnode(int tag, char *named);
void addFirst();
void addFollow();
void createParseTable();
void printParseTable();

void initialize_parserStack();
void push(char* ele);
char* pop();
char* top();

void  parseInputSourceCode(char *testcaseFile);
void printParseTree(node* temp, FILE* fp);
void printTreeConsole(node* temp);
void treeTraverse(node* temp);
int get_index(char* str,int tag);

void initialize_treeStack();
void pushTreeNode(g_node* temp, int childno);
g_node* popTreeNode(int *childout);
g_node* topTreeNode(int *childout);

void initialize_parentStack();
void pushParentNode(node* temp);
node* popParentNode();
node* topParentNode();