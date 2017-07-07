
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

//SymTable.h
#include "parser.h"

#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED


typedef struct st 
{
	char name[40];
	int type; //bool = 0,integer = 1, real = 2 
	int ifArr;
	int lineno;
	int start_line;
	int end_line;
	int offset;
	int scope_depth;
	int moduleNo;
	int num1;//used if_array_data
	int num2;//used_if_array

} st_node;

typedef struct ft
{
	char name[40];
	node* inp_plist;
	node* out_plist;
	int scope_depth;
	int moduleNo;
	int start_lineno;
	int end_lineno;
}ft_node;


typedef struct s1
{
	int scope_depth;
	int start;
	int end;
	int moduleNo;

}snode;

typedef struct s2
{
	node* mod_reuse;
	int scope;
	int moduleno;

}reuse_struc;

typedef struct s3
{
	char name[40];
	int set;

}out1;

snode sarray[30];
reuse_struc mod_reuse_dec[10];
out1 output[10];

#endif

void initialize_globalDecl();
int get_width(int type);
void initialize_sarray();
void initialize_mod_reuse_dec();
void initialize_output();
void updateSarray(int start1, int end1, int moduleNo1);
void updateSarrayDec();
void create_add_func(node* inp_plist1, node* out_plist1,int scope,int mno, char* nam, int s, int e);
void create_add_sym(char* nam, int type1, int ifArr1, int lineNo,int start, int end, int Num1, int Num2, int scope, int offset1);
int whichType(char* name);
char* whichTypeChar(int val);

int expr_func(node* expr);
int ABE_func(node* abe);
int anyterm_func(node* anyterm);
int arithmeticExpr_func(node* arithmeticExpr);
int term_func(node* term);
int factor_func(node* factor);

int findDeclared_scope(char* sym, int current) ;

void populateSymbolTable(node* AST);
void otherModulescase(node* otherModules);
void addInputPlist(node* inp_plist1);
void addOutputPlist(node* out_plist1);
void stmts(node* temp);
void case_Stmts(node* temp, int typeCheck);
void driverModulecase(node* driverModule);

void module_reuse_check(node* reuse_mod, ft_node* actual_mod);
void printSymbolTable();










