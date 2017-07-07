

/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

#ifndef hash
#define hash
#include "symbolTable.h"
#include "parser.h"


typedef struct h1 hashnode_ft; // for function hashnode


struct h1{
	ft_node* hnode;
	hashnode_ft* next;
};


typedef struct h2 hashnode_st; //for symbol hash node

struct h2{
	st_node* hnode;
	hashnode_st* next;
};


hashnode_st* hasharr_st[15]; //array of hash_node ptrs
hashnode_ft* hasharr_ft[15]; //array of hash_node ptrs

#endif
int getHashValue(char *keyword);

st_node* find_Sym(char* name, int scope_depth, int modno);
ft_node* find_func(char* name);

void add_sym(st_node* n);
void add_func(ft_node* n);

void initialize_HashTables();
