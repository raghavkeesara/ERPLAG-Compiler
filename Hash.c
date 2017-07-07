
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

#include "Hash.h"
#include "parser.h"
#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getHashValue(char *keyword)
{
	int try =0;
    int mul = 3;
    int i = 0;
    int hashkey = 15;

    while(keyword[i]!='\0')
    {
    	 try=(try*mul+ (int)keyword[i++])%hashkey;
    }	
        

    return try;
}

void initialize_HashTables()
{
	int i=0;
	for(i=0; i<15; i++)
	{
		hasharr_st[i]  = NULL;
		hasharr_ft[i] = NULL;
	}	
}


st_node* find_Sym(char* name, int scope_depth, int modno)
{
	int hvalue = getHashValue(name);
	//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ %s %d %d\n", name, scope_depth, modno);
	hashnode_st* temp = hasharr_st[hvalue];
	if(temp == NULL) //no_elements are present
	{
		return NULL;
	}
	else 
	{
		while(temp != NULL)
		{
			if(temp->hnode->scope_depth == scope_depth && temp->hnode->moduleNo == modno)
			{
				if(strcmp(temp->hnode->name, name)==0)
					return temp->hnode;
			}
			temp=temp->next;
		}
		return NULL;
	}	
}

ft_node* find_func(char* name)
{
	int hvalue = getHashValue(name);
	hashnode_ft* temp = hasharr_ft[hvalue];


	if(temp == NULL) //empty
	{
			return NULL;
	}
	else 
	{
		while(temp!=NULL)
		{
			if(strcmp(temp->hnode->name, name)==0)
				return temp->hnode;
			else
			temp=temp->next;
		}
				
	}
	return NULL; //not found
}

void add_func(ft_node* n)
{
	int hvalue = getHashValue(n->name);
	
	hashnode_ft* new= (hashnode_ft*)malloc(sizeof(hashnode_ft*));
	if(hasharr_ft[hvalue]==NULL) //empty
	{
		hasharr_ft[hvalue] = new;
		hasharr_ft[hvalue]->hnode = n;
	}
	else
	{
		hashnode_ft* beginning = hasharr_ft[hvalue];

		while(beginning->next != NULL)
		{
			beginning = beginning->next;
		}	
		new->next=beginning->next;
		beginning->next = new;
		new->hnode= n;
	}
	
}
void add_sym(st_node* n)
{
	int hvalue = getHashValue(n->name);
	hashnode_st* new= (hashnode_st*)malloc(sizeof(hashnode_st));
	if(hasharr_st[hvalue]==NULL) //empty
	{
		//printf("%d **************** %d %d %d \n",hvalue, n->type, n->lineno, n->num1);
		hasharr_st[hvalue] = new;
		new->hnode = n;
		new->next = NULL;
	}
	else
	{
		hashnode_st* beginning=hasharr_st[hvalue];
		while(beginning->next != NULL)
		{
			beginning = beginning->next;
		}

		new->next=beginning->next;
		beginning->next = new;
		new->hnode= n;
	}
}
