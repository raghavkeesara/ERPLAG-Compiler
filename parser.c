
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/

#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define len 100


//gives a pointer to new grammar node created
g_nodeptr get_gnode(int tag, char *named)
{
	g_nodeptr neww =(g_nodeptr)malloc(sizeof(g_node));
	neww->tag=tag;
	neww->next=NULL;
	strcpy(neww->name,named);
	return neww;
}

void addGrammar()
{
	char str1[1000], str2[1000];
	FILE *fp;
	
	fp = fopen ("grammar.txt", "r");
	int i=0;
	while(fscanf(fp, "%s -> %s\n", str1, str2)!=EOF)
	{
		//LHS
		g_nodeptr new=NULL;
		if(str1[1]>=65 && str1[1]<=95)
		{
			char temp[100]={"\0"};
			int z=0;
			for(z=1;z<strlen(str1)-1;z++)
				temp[z-1]=str1[z];
			//printf("%s\n",temp );
			new= get_gnode(1,temp);	
		}
		else
		{
			char temp[100]={"\0"};
			int z=0;
			for(z=1;z<strlen(str1)-1;z++)
				temp[z-1]=str1[z];
			new= get_gnode(0,temp);
		}
		g_array[i]=new;
		//printf("%s",g_array[i]->name);

		//RHS
		g_nodeptr curr_t_node=NULL,store=NULL;
		char *token;
		token = strtok(str2, "<>");
		while(token!=NULL)
		{
			//if(strcmp(token,"BC")==0)
			//printf("BC ENCOUNTERD ");
			if(token[1]>=65 && token[1]<=95)
			{
				curr_t_node = get_gnode(1,token);	
			}
			else
			{
				curr_t_node = get_gnode(0,token);
			}
			//printf("%s\n",curr_t_node->name);
			token = strtok(NULL, "<>");
			if(store!=NULL)
			{
				store->next=curr_t_node;
			}
			else
			{
				new->next=curr_t_node;
			}
			store = curr_t_node;
		}
		i++;
	}
}

void addFollow()
{
	int i,j;
	for (i=0;i<51;++i)
	 for (j=0;j<15;++j)
	  strcpy(followSet[i].follow[j],"\0");
	
	//0 program
	i=0;
	strcpy(followSet[i].follow[0],"$");

	// 1 ModuleDeclarations
	
	i++;
	strcpy(followSet[i].follow[0],"DEF");
	strcpy(followSet[i].follow[1],"DRIVERDEF");

	// 2 <moduleDeclaration> 
	i++;
	strcpy(followSet[i].follow[0],"DECLARE");
	strcpy(followSet[i].follow[1],"DEF");
	strcpy(followSet[i].follow[2],"DRIVERDEF");

	// 3 <otherModules> 
	i++;
	strcpy(followSet[i].follow[0],"DRIVERDEF");
	strcpy(followSet[i].follow[1],"$");

	// 4 <driverModule> 
	i++;
	strcpy(followSet[i].follow[0],"DEF");
	strcpy(followSet[i].follow[1],"$");

	// 5 <module>
	i++;
	strcpy(followSet[i].follow[0],"DEF");
	strcpy(followSet[i].follow[1],"DRIVERDEF");
	strcpy(followSet[i].follow[2],"$");

	// 6 <ret>
	i++;
	strcpy(followSet[i].follow[0],"START");

	// 7 <input_plist>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");

	// 8 <N1>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");

	// 9 <output_plist>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");

	// 10 <N2>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");

	// 11 <dataType>
	i++;
	strcpy(followSet[i].follow[0],"COMMA");
	strcpy(followSet[i].follow[1],"SQBC");
	strcpy(followSet[i].follow[2],"SEMICOL");

	// 12 <type>
	i++;
	strcpy(followSet[i].follow[0],"COMMA");
	strcpy(followSet[i].follow[1],"SQBC");
	strcpy(followSet[i].follow[2],"SEMICOL");

	// 13 <moduleDef>
	i++;
	strcpy(followSet[i].follow[0],"DEF");
	strcpy(followSet[i].follow[1],"DRIVERDEF");
	strcpy(followSet[i].follow[2],"$");

	// 14 <statements>
	i++;
	strcpy(followSet[i].follow[0],"END");
	strcpy(followSet[i].follow[1],"BREAK");

	// 15 <statement>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 16 <ioStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 17 <var> 
	i++;
	strcpy(followSet[i].follow[0],"MUL");
	strcpy(followSet[i].follow[1],"DIV");
	strcpy(followSet[i].follow[2],"BC");
	strcpy(followSet[i].follow[3],"LT");
	strcpy(followSet[i].follow[4],"LE");
	strcpy(followSet[i].follow[5],"GT");
	strcpy(followSet[i].follow[6],"GE");
	strcpy(followSet[i].follow[7],"EQ");
	strcpy(followSet[i].follow[8],"NE");
	strcpy(followSet[i].follow[9],"AND");
	strcpy(followSet[i].follow[10],"OR");
	strcpy(followSet[i].follow[11],"SEMICOL");
	strcpy(followSet[i].follow[12],"PLUS");
	strcpy(followSet[i].follow[13],"MINUS");

	// 18 <whichID>
	i++;
	strcpy(followSet[i].follow[0],"MUL");
	strcpy(followSet[i].follow[1],"DIV");
	strcpy(followSet[i].follow[2],"BC");
	strcpy(followSet[i].follow[3],"LT");
	strcpy(followSet[i].follow[4],"LE");
	strcpy(followSet[i].follow[5],"GT");
	strcpy(followSet[i].follow[6],"GE");
	strcpy(followSet[i].follow[7],"EQ");
	strcpy(followSet[i].follow[8],"NE");
	strcpy(followSet[i].follow[9],"AND");
	strcpy(followSet[i].follow[10],"OR");
	strcpy(followSet[i].follow[11],"SEMICOL");
	strcpy(followSet[i].follow[12],"PLUS");
	strcpy(followSet[i].follow[13],"MINUS");

	// 19 <simpleStmt> 
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 20 <assignmentStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 21 <whichStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");

	// 22 <lvalueIDStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 23 <lvalueARRStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 24 <index1>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");

	// 25 <moduleReuseStmt> 
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 26 <optional> 
	i++;
	strcpy(followSet[i].follow[0],"USE");

	// 27 <idList>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");
	strcpy(followSet[i].follow[1],"SEMICOL");
	strcpy(followSet[i].follow[2],"COLON");

	// 28 <N3>
	i++;
	strcpy(followSet[i].follow[0],"SQBC");
	strcpy(followSet[i].follow[1],"SEMICOL");
	strcpy(followSet[i].follow[2],"COLON");

	// 29 <expression>
	i++;
	strcpy(followSet[i].follow[0],"SEMICOL");
	strcpy(followSet[i].follow[1],"BC");

	// 30 <arithmeticOrBooleanExpr>
	i++;
	strcpy(followSet[i].follow[0],"SEMICOL");
	strcpy(followSet[i].follow[1],"BC");
	 
	// 31 <N7>
	i++;
	strcpy(followSet[i].follow[0],"SEMICOL");
	strcpy(followSet[i].follow[1],"BC");
	 
	// 32 <AnyTerm>
	i++;
	strcpy(followSet[i].follow[0],"AND");
	strcpy(followSet[i].follow[1],"OR");
	strcpy(followSet[i].follow[2],"SEMICOL");
	strcpy(followSet[i].follow[3],"BC");
	 
	// 33 <N8>
	i++;
	strcpy(followSet[i].follow[0],"AND");
	strcpy(followSet[i].follow[1],"OR");
	strcpy(followSet[i].follow[2],"SEMICOL");
	strcpy(followSet[i].follow[3],"BC");

	// 34 <arithmeticExpr>
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"LT");
	strcpy(followSet[i].follow[2],"LE");
	strcpy(followSet[i].follow[3],"GT");
	strcpy(followSet[i].follow[4],"GE");
	strcpy(followSet[i].follow[5],"EQ");
	strcpy(followSet[i].follow[6],"NE");
	strcpy(followSet[i].follow[7],"AND");
	strcpy(followSet[i].follow[8],"OR");
	strcpy(followSet[i].follow[9],"SEMICOL");

	// 35 <N4>
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"LT");
	strcpy(followSet[i].follow[2],"LE");
	strcpy(followSet[i].follow[3],"GT");
	strcpy(followSet[i].follow[4],"GE");
	strcpy(followSet[i].follow[5],"EQ");
	strcpy(followSet[i].follow[6],"NE");
	strcpy(followSet[i].follow[7],"AND");
	strcpy(followSet[i].follow[8],"OR");
	strcpy(followSet[i].follow[9],"SEMICOL");

	// 36 <term_1>
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"LT");
	strcpy(followSet[i].follow[2],"LE");
	strcpy(followSet[i].follow[3],"GT");
	strcpy(followSet[i].follow[4],"GE");
	strcpy(followSet[i].follow[5],"EQ");
	strcpy(followSet[i].follow[6],"NE");
	strcpy(followSet[i].follow[7],"AND");
	strcpy(followSet[i].follow[8],"OR");
	strcpy(followSet[i].follow[9],"SEMICOL");
	strcpy(followSet[i].follow[10],"PLUS");
	strcpy(followSet[i].follow[11],"MINUS");

	// 37 <N5> 
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"LT");
	strcpy(followSet[i].follow[2],"LE");
	strcpy(followSet[i].follow[3],"GT");
	strcpy(followSet[i].follow[4],"GE");
	strcpy(followSet[i].follow[5],"EQ");
	strcpy(followSet[i].follow[6],"NE");
	strcpy(followSet[i].follow[7],"AND");
	strcpy(followSet[i].follow[8],"OR");
	strcpy(followSet[i].follow[9],"SEMICOL");
	strcpy(followSet[i].follow[10],"PLUS");
	strcpy(followSet[i].follow[11],"MINUS");

	// 38 <factor> 
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"LT");
	strcpy(followSet[i].follow[2],"LE");
	strcpy(followSet[i].follow[3],"GT");
	strcpy(followSet[i].follow[4],"GE");
	strcpy(followSet[i].follow[5],"EQ");
	strcpy(followSet[i].follow[6],"NE");
	strcpy(followSet[i].follow[7],"AND");
	strcpy(followSet[i].follow[8],"OR");
	strcpy(followSet[i].follow[9],"SEMICOL");
	strcpy(followSet[i].follow[10],"PLUS");
	strcpy(followSet[i].follow[11],"MINUS");
	strcpy(followSet[i].follow[12],"MUL");
	strcpy(followSet[i].follow[13],"DIV");

	// 39 <op1>
	i++;
	strcpy(followSet[i].follow[0],"ID");
	strcpy(followSet[i].follow[1],"NUM");
	strcpy(followSet[i].follow[2],"RNUM");
	strcpy(followSet[i].follow[3],"BO");
	 
	// 40 <op2>
	i++;
	strcpy(followSet[i].follow[0],"ID");
	strcpy(followSet[i].follow[1],"NUM");
	strcpy(followSet[i].follow[2],"RNUM");
	strcpy(followSet[i].follow[3],"BO");
	// 41 <logicalOp>
	i++;
	strcpy(followSet[i].follow[0],"ID");
	strcpy(followSet[i].follow[1],"NUM");
	strcpy(followSet[i].follow[2],"RNUM");
	strcpy(followSet[i].follow[3],"BO");
	// 42 <relationalOp>
	i++;
	strcpy(followSet[i].follow[0],"ID");
	strcpy(followSet[i].follow[1],"NUM");
	strcpy(followSet[i].follow[2],"RNUM");
	strcpy(followSet[i].follow[3],"BO");
	// 43 <declareStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 44 <conditionalStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 45 <caseStmts> 
	i++;
	strcpy(followSet[i].follow[0],"DEFAULT");
	strcpy(followSet[i].follow[1],"END");

	// 46 <N9>
	i++;
	strcpy(followSet[i].follow[0],"DEFAULT");
	strcpy(followSet[i].follow[1],"END");

	// 47 <value>
	i++;
	strcpy(followSet[i].follow[0],"COLON");

	// 48 <default_1>
	i++;
	strcpy(followSet[i].follow[0],"END");

	// 49 <iterativeStmt>
	i++;
	strcpy(followSet[i].follow[0],"GET_VALUE");
	strcpy(followSet[i].follow[1],"PRINT");
	strcpy(followSet[i].follow[2],"ID");
	strcpy(followSet[i].follow[3],"SQBO");
	strcpy(followSet[i].follow[4],"USE");
	strcpy(followSet[i].follow[5],"DECLARE");
	strcpy(followSet[i].follow[6],"SWITCH");
	strcpy(followSet[i].follow[7],"FOR");
	strcpy(followSet[i].follow[8],"WHILE");
	strcpy(followSet[i].follow[9],"END");
	strcpy(followSet[i].follow[10],"BREAK");

	// 50 <range> 
	i++;
	strcpy(followSet[i].follow[0],"BC");
	strcpy(followSet[i].follow[1],"SQBC");
}


void addFirst()
{
	int i,j;
	for (i=0;i<51;++i)
	 for (j=0;j<15;++j)
	  strcpy(firstSet[i].first[j],"\0");
	
	i=0;
	// i=0, Program
	strcpy(firstSet[i].first[0],"DECLARE");
	strcpy(firstSet[i].first[1],"DEF");
	strcpy(firstSet[i].first[2],"DRIVERDEF");
	
	// 1 <moduleDeclarations>
	 ++i;
	strcpy(firstSet[i].first[0],"DECLARE");
	strcpy(firstSet[i].first[1],"EPSILON");
	
	// 2 <moduleDeclaration> 
	++i;
	strcpy(firstSet[i].first[0],"DECLARE");

	// 3<otherModules> 
	++i;
	strcpy(firstSet[i].first[0],"DEF");
	strcpy(firstSet[i].first[1],"EPSILON");

	// 4 <driverModule> 
	++i;
	strcpy(firstSet[i].first[0],"DRIVERDEF");

	// 5 <module> 
	++i;
	strcpy(firstSet[i].first[0],"DEF");

	// 6 <ret> 
	++i;
	strcpy(firstSet[i].first[0],"RETURNS");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 7 <input_plist> 
	++i;
	strcpy(firstSet[i].first[0],"ID");


	// 8 <N1> 
	++i;
	strcpy(firstSet[i].first[0],"COMMA");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 9 <output_plist> 
	++i;
	strcpy(firstSet[i].first[0],"ID");

	//10 <N2> 
	++i;
	strcpy(firstSet[i].first[0],"COMMA");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 11 <dataType> 
	++i;
	strcpy(firstSet[i].first[0],"INTEGER");
	strcpy(firstSet[i].first[1],"REAL");
	strcpy(firstSet[i].first[2],"BOOLEAN");
	strcpy(firstSet[i].first[3],"ARRAY");
	

	// 12 <type>
	++i;
	strcpy(firstSet[i].first[0],"INTEGER");
	strcpy(firstSet[i].first[1],"REAL");
	strcpy(firstSet[i].first[2],"BOOLEAN");

	// 13 <moduleDef> 
	++i;
	strcpy(firstSet[i].first[0],"START");


	// 14 <statements> 
	++i;
	strcpy(firstSet[i].first[0],"GET_VALUE");
	strcpy(firstSet[i].first[1],"PRINT");
	strcpy(firstSet[i].first[2],"ID");
	strcpy(firstSet[i].first[3],"SQBO");
	strcpy(firstSet[i].first[4],"USE");
	strcpy(firstSet[i].first[5],"DECLARE");
	strcpy(firstSet[i].first[6],"SWITCH");
	strcpy(firstSet[i].first[7],"FOR");
	strcpy(firstSet[i].first[8],"WHILE");
	strcpy(firstSet[i].first[9],"EPSILON");


	// 15 <statement>
	++i;
	strcpy(firstSet[i].first[0],"GET_VALUE");
	strcpy(firstSet[i].first[1],"PRINT");
	strcpy(firstSet[i].first[2],"ID");
	strcpy(firstSet[i].first[3],"SQBO");
	strcpy(firstSet[i].first[4],"USE");
	strcpy(firstSet[i].first[5],"DECLARE");
	strcpy(firstSet[i].first[6],"SWITCH");
	strcpy(firstSet[i].first[7],"FOR");
	strcpy(firstSet[i].first[8],"WHILE");

	 
	// 16 <ioStmt> 
	++i;
	strcpy(firstSet[i].first[0],"GET_VALUE");
	strcpy(firstSet[i].first[1],"PRINT");

	// 17 <var> 
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"NUM");
	strcpy(firstSet[i].first[2],"RNUM");
	//strcpy(firstSet[i].first[3],"BO"); //changed


	// 18 <whichID> 
	++i;
	strcpy(firstSet[i].first[0],"SQBO");
	strcpy(firstSet[i].first[1],"EPSILON");
	
	 
	// 19 <simpleStmt> 
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"SQBO");
	strcpy(firstSet[i].first[2],"USE");

	// 20 <assignmentStmt>
	++i;
	strcpy(firstSet[i].first[0],"ID");

	 
	// 21 <whichStmt>
	++i;
	strcpy(firstSet[i].first[0],"ASSIGNOP");
	strcpy(firstSet[i].first[1],"SQBO");


	// 22 <lvalueIDStmt>
	++i ;
	strcpy(firstSet[i].first[0],"ASSIGNOP");

	 
	// 23 <lvalueARRStmt> 
	++i;
	strcpy(firstSet[i].first[0],"SQBO");


	// 24 <index_1> 
	++i;
	strcpy(firstSet[i].first[0],"NUM");
	strcpy(firstSet[i].first[1],"ID");

	 
	// 25 <moduleReuseStmt>
	++i;
	strcpy(firstSet[i].first[0],"USE");
	strcpy(firstSet[i].first[1],"SQBO");


	// 26 <optional>
	++i;
	strcpy(firstSet[i].first[0],"SQBO");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 27 <idList>
	++i;
	strcpy(firstSet[i].first[0],"ID");
	 
	// 28 <N3>
	++i;
	strcpy(firstSet[i].first[0],"COMMA");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 29 <expression>
	++i;
	strcpy(firstSet[i].first[0],"MINUS");
	strcpy(firstSet[i].first[1],"BO");
	strcpy(firstSet[i].first[2],"ID");
	strcpy(firstSet[i].first[3],"NUM");
	strcpy(firstSet[i].first[4],"RNUM");
	strcpy(firstSet[i].first[5],"PLUS");

	 
	// 30 <arithmeticOrBooleanExpr>
	++i;
	strcpy(firstSet[i].first[0],"BO"); //change
	strcpy(firstSet[i].first[1],"ID");
	strcpy(firstSet[i].first[2],"NUM");
	strcpy(firstSet[i].first[3],"RNUM");


	// 31 <N7>
	++i;
	strcpy(firstSet[i].first[0],"AND");
	strcpy(firstSet[i].first[1],"OR");
	strcpy(firstSet[i].first[2],"EPSILON");


	// 32 <AnyTerm>
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"NUM");
	strcpy(firstSet[i].first[2],"RNUM");
	strcpy(firstSet[i].first[3],"BO");

	 
	// 33 <N8>
	++i;
	strcpy(firstSet[i].first[0],"LT");
	strcpy(firstSet[i].first[1],"LE");
	strcpy(firstSet[i].first[2],"GT");
	strcpy(firstSet[i].first[3],"GE");
	strcpy(firstSet[i].first[4],"EQ");
	strcpy(firstSet[i].first[5],"NE");
	strcpy(firstSet[i].first[6],"EPSILON");

	// 34 <arithmeticExpr>
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"NUM");
	strcpy(firstSet[i].first[2],"RNUM");
	strcpy(firstSet[i].first[3],"BO");

	// 35 <N4>
	++i;
	strcpy(firstSet[i].first[0],"PLUS");
	strcpy(firstSet[i].first[1],"MINUS");
	strcpy(firstSet[i].first[2],"EPSILON");

	// 36 <term>
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"NUM");
	strcpy(firstSet[i].first[2],"RNUM");
	strcpy(firstSet[i].first[3],"BO");

	// 37 <N5>
	++i;
	strcpy(firstSet[i].first[0],"MUL");
	strcpy(firstSet[i].first[1],"DIV");
	strcpy(firstSet[i].first[2],"EPSILON");

	// 38 <factor> 
	++i;
	strcpy(firstSet[i].first[0],"ID");
	strcpy(firstSet[i].first[1],"NUM");
	strcpy(firstSet[i].first[2],"RNUM");
	strcpy(firstSet[i].first[3],"BO");


	// 39 <op1>
	++i;
	strcpy(firstSet[i].first[0],"PLUS");
	strcpy(firstSet[i].first[1],"MINUS");


	// 40 <op2>
	++i;
	strcpy(firstSet[i].first[0],"MUL");
	strcpy(firstSet[i].first[1],"DIV");

	// 41 <logicalOp> 
	++i;
	strcpy(firstSet[i].first[0],"AND");
	strcpy(firstSet[i].first[1],"OR");


	// 42 <relationalOp>
	++i;
	strcpy(firstSet[i].first[0],"LT");
	strcpy(firstSet[i].first[1],"LE");
	strcpy(firstSet[i].first[2],"GT");
	strcpy(firstSet[i].first[3],"GE");
	strcpy(firstSet[i].first[4],"EQ");
	strcpy(firstSet[i].first[5],"NE");

	// 43 <declareStmt>
	++i;
	strcpy(firstSet[i].first[0],"DECLARE");

	// 44 <conditionalStmt>
	++i;
	strcpy(firstSet[i].first[0],"SWITCH");

	 
	// 45 <caseStmts>
	++i;
	strcpy(firstSet[i].first[0],"CASE");
	 
	// 46 <N9>
	++i;
	strcpy(firstSet[i].first[0],"CASE");
	strcpy(firstSet[i].first[1],"EPSILON");

	// 47 <value>
	++i;
	strcpy(firstSet[i].first[0],"NUM");
	strcpy(firstSet[i].first[1],"TRUEs");
	strcpy(firstSet[i].first[2],"TRUEs");
	 
	// 48 <default_1>
	++i;
	strcpy(firstSet[i].first[0],"DEFAULT");
	strcpy(firstSet[i].first[1],"EPSILON");


	// 49 <iterativeStmt>
	++i;
	strcpy(firstSet[i].first[0],"FOR");
	strcpy(firstSet[i].first[1],"WHILE");
	 
	// 50 <range>
	++i;
	strcpy(firstSet[i].first[0],"NUM");
	 

}


int get_index(char* str,int tag)
{
	//if(strcmp(str,"BC")==0)
	//printf("im here\n");
	int siz=0,i=0;
	if(tag==1)//T
	{
		siz=59;
		int i=0;
		for(i=0;i<siz;i++)
		{
			if(strcmp(str,t_str[i])==0){
				return i;
			}
		}
	}
	else
	{
		siz=51;
		int i=0;
		for(i=0;i<siz;i++)
		{
			if(strcmp(str,nt_str[i])==0){
				return i;
			}
		}	
	}
	//printf("error  for %s, with tag %d\n", str, tag);
	return -1;
	
}

void createParseTable()
{
	int i,j;
	for(i=0;i<51;i++)
		for(j=0;j<59;j++)
			parseTable[i][j] = -1;


	for(i=0;i<95;i++)
	{
		int m = get_index(g_array[i]->name,0);
		
		if(g_array[i]->next->tag == 1)
		{
			int n = get_index(g_array[i]->next->name,1);
			parseTable[m][n] = i;

			if(strcmp(g_array[i]->next->name, "EPSILON")==0)
			{
				int z=0;

				int ind = get_index(g_array[i]->name,0);
				while(z<15)
				{
					if(strcmp(followSet[ind].follow[z],"\0")!=0)
					{
						n = get_index(followSet[ind].follow[z],1);
						//printf("here 1\n");
						parseTable[m][n] = i;
					}
					else
						break;

					z++;		
				}
				
			}
		}
		else//NT
		{
			int z=0;

			while(z<15)
			{
				int ind = get_index(g_array[i]->next->name,0);
				if(strcmp(firstSet[ind].first[z],"\0") != 0)
				{

					if(strcmp(firstSet[ind].first[z],"EPSILON") == 0)
					{
						g_node* temp = g_array[i]->next;
						int check = 1;
						int termFound = 0;
						while(temp->next != NULL)
						{
							check = 1;
							temp = temp->next;

							if(temp->tag == 1)
							{
								termFound = 1;
								int n = get_index(temp->name, 1);
								//printf("here 2\n");
								parseTable[m][n] = i;
								break;
							}

							int c=0;
							while(c<15)
							{
								int ind1 = get_index(temp->name,temp->tag);
								//printf("here 3a\n");
								if(strcmp(firstSet[ind1].first[c],"\0") != 0)
								{
									int n = get_index(firstSet[ind1].first[c],1);
									//printf("here 3\n");
									parseTable[m][n] = i;

									if(strcmp(firstSet[ind1].first[c],"EPSILON")==0)
									{
										check =0;
									}
									c++;
								}
								else
									break;
							}

							if(check == 1)
								break;

						}

						if(temp->next == NULL && check == 0)
						{
							int z=0;

							while(z<15)
							{
								int ind1 = get_index(g_array[i]->name,g_array[i]->tag);
								//printf("here 4\n");
								if(strcmp(followSet[ind1].follow[z],"\0")!=0)
								{
									int n = get_index(followSet[ind1].follow[z],1);
									parseTable[m][n] = i;
								}
								else
									break;

								z++;		
							}
						}

					}
					else
					{
						int ind1 = get_index(g_array[i]->next->name,g_array[i]->next->tag);
						int n = get_index(firstSet[ind1].first[z],1);
						//printf("here 5\n");
						parseTable[m][n] = i;
					}
				}
				else
					break;
				z++;		

			}	
		}

	}

	
}

void printParseTable()
{
	int i,j;
	for(i=0;i<51;i++)
	{	
		for(j=0;j<59;j++)
		{
			printf("%d ",parseTable[i][j]);
		}
		printf("\n");	
	}
}


//--------------------------------------------------
node *p ;
void parseInputSourceCode(char *testcaseFile)
{
	initialize_parserStack();
	initialize_treeStack();
	initialize_parentStack();

	lexicalOutput(testcaseFile);

	//g_node* dd = g_array[96]->next;
	//printf("%s\n", dd->name);

	push("$");
	push("program");
	tokeninfo sm = getNextToken();
	int error = 0;
	int count = 0;

	while(1)
	{
		
		//printf("%s %s\n", top(), t_str[sm->tokentype]);

		if( (strcmp(top(),"$") == 0) && (strcmp(t_str[sm->tokentype],"$") == 0) )
		{
			if(error == 0)
				printf("Input source code is syntactically correct...........\n");
			else
				printf("There are %d error(s) in syntax analsis\n", error);
			break;
		}
		else if( get_index(top(),1) != -1 )
		{
			
			if( strcmp(top(),t_str[sm->tokentype]) == 0 )
			{
				int childnum = -10;
				g_node* others = (g_node *) malloc(100*sizeof(g_node));
				others = popTreeNode(&childnum);
				
				node* others_parent = (node *) malloc(100*sizeof(node));
				others_parent = popParentNode();
				node* out_node = addChildren(others_parent, others->name, others->tag, 0, sm->tokentype, sm->lineNo, sm->value, 1);

				/*printf("%s\n", others->name);
				printf("%s\n", t_str[sm->tokentype]);*/
				

				sm = getNextToken();
				pop();
			}
			else
			{
				printf("ERROR_5: The token %s does not match at line %d. The expected token here is %s",t_str[sm->tokentype],sm->lineNo, top());
				pop();
				error++;
			}
		}
		else if( get_index(top(),0) != -1 )
		{
			char *x = pop();
			int q = get_index(x,0);
			int v = get_index(t_str[sm->tokentype],1);
			//printf("%d %d\n",q,v );
			int ruleno = parseTable[q][v];
			//printf("rule no %d at code line no %d\n", ruleno, sm->lineNo);
			
			if(ruleno == -1)
			{
				printf("Error. Rule No not there in parse table for row %d and col %d. \n",q,v);
				error++;
				continue;
				//break;
			}	

			g_node* temp = g_array[ruleno]->next;

			if(strcmp(temp->name,"EPSILON")==0)
			{
				int childnum = -10;
				g_node* others = (g_node *) malloc(100*sizeof(g_node));
				others = popTreeNode(&childnum);

				node* others_parent = (node *) malloc(100*sizeof(node));
				others_parent = popParentNode();
				node* out_node = addChildren(others_parent, others->name, others->tag, 0, sm->tokentype, sm->lineNo, sm->value, 0);

				continue;
			}	

			int count = 0;
			while( temp != NULL)
			{
				temp = temp->next;
				count++;
			}
			int input_child = count;
			int i;
			for(i=count;i >0;i--)
			{
				g_node* trial = g_array[ruleno];
				int j=0;	
				while(j < i)
				{
					trial = trial->next;
					j++; 
				}
				push(trial->name);
			}	

			/** Tree part starts**/

			if(ruleno == 0)
			{
				//printf("Rule No 0 start\n");
				//printf("hehheh  %s %s\n",g_array[ruleno]->name,  sm->value.id);
				p = createNode(g_array[ruleno]->name, 0, input_child, sm->tokentype, sm->lineNo, sm->value, 0);
				//printf("%s ", g_array[ruleno]->name);

				temp = g_array[ruleno]->next;
				int i=0;

				for(i = input_child ; i >0 ; i--)
				{
					g_node* ins;
					int j = 1;
					ins = temp;
					while(j<i)
					{	
						ins = ins->next;
						j++;
					}

					pushTreeNode(ins,i);
					pushParentNode(p);
				}
				//printf("Rule No 0 end\n");
			} 
			else
			{
				int childnum = -10;
				g_node* others = (g_node *) malloc(100*sizeof(g_node));
				others = popTreeNode(&childnum);

				node* others_parent = (node *) malloc(100*sizeof(node));
				others_parent = popParentNode();

				//others->isLeafNode = 0;
				temp = g_array[ruleno]->next; 

				int i=0;
				node* out_node = addChildren(others_parent, others->name, others->tag, 0, sm->tokentype, sm->lineNo, sm->value, 0);

				/*while( temp != NULL && i < input_child)
				{
					node* ins;
					//printf("%s %s\n", temp->name, sm_n->value.id);
					ins = addChildren(others, temp->name, temp->tag, 0, sm->tokentype, sm->lineNo, sm->value, 1);
					//printf("%s ", temp->name);
					temp = temp->next;
					i++;
				}*/
				//printf("\n");

				for(i = input_child ; i >0 ; i--)
				{
					g_node* ins;
					int j = 1;
					ins = temp;
					while(j<i)
					{	
						ins = ins->next;
						j++;
					}
					
					pushTreeNode(ins,i);
					pushParentNode(out_node);
				}
				//printf("else 4\n");

			}	
			
			/** Tree part ends **/		

			
		}
		else if((strcmp(top(),"$") == 0) && (strcmp(t_str[sm->tokentype],"$") != 0))
		{
			printf("The stack is empty but input is still left............\n");
			break;
		}
		else if((strcmp(top(),"$") != 0) && (strcmp(t_str[sm->tokentype],"$") == 0))
		{
			printf("The Input buffer is empty but elements in stack is still left............\n");
			break;
		}

	}

}

int count_nodes =0;

/** Function for inorder traversal of n-ary Tree **/
void printParseTree(node* temp, FILE* fp)
{
	if(temp == NULL)
		return;
	printParseTree(temp->children,fp);

	if(temp->isLeafNode == 1 && strcmp(temp->buf_name,"NUM")==0)
		fprintf(fp,"%d %d %s %d %s yes %s\n",temp->val.num, temp->lineNo, t_str[temp->tk], temp->val.num, temp->parent->buf_name, temp->buf_name);
	else if(temp->isLeafNode == 1 && strcmp(temp->buf_name,"RNUM")==0)
		fprintf(fp,"%f %d %s %f %s yes %s\n",temp->val.rnum, temp->lineNo, t_str[temp->tk], temp->val.rnum, temp->parent->buf_name, temp->buf_name);
	else
	{
		if(temp->isLeafNode == 1)
			fprintf(fp,"%s %d %s %s yes %s\n",temp->val.id, temp->lineNo, t_str[temp->tk] , temp->parent->buf_name, temp->buf_name);
		else
		{
			if(temp->parent != NULL)
				fprintf(fp,"--- %d %s %s no %s\n", temp->lineNo, t_str[temp->tk] , temp->parent->buf_name, temp->buf_name);
			else
				fprintf(fp,"--- %d %s ROOT no %s\n", temp->lineNo, t_str[temp->tk], temp->buf_name);
		}	
	}	

	if(temp->children != NULL)
	{
		node* first =  temp->children->next;
		while(first!=NULL)
		{
			printParseTree(first,fp);
			first = first->next; 
		}	

	}	
	
}

/** Function for inorder traversal of n-ary Tree **/
void printTreeConsole(node* temp)
{
	if(temp == NULL)
		return;
	printTreeConsole(temp->children);

	if(temp->isLeafNode == 1 && strcmp(temp->buf_name,"NUM")==0)
		printf("%d %d %s %d %s yes %s\n",temp->val.num, temp->lineNo, t_str[temp->tk], temp->val.num, temp->parent->buf_name, temp->buf_name);
	else if(temp->isLeafNode == 1 && strcmp(temp->buf_name,"RNUM")==0)
		printf("%f %d %s %f %s yes %s\n",temp->val.rnum, temp->lineNo, t_str[temp->tk], temp->val.rnum, temp->parent->buf_name, temp->buf_name);
	else
	{
		if(temp->isLeafNode == 1)
			printf("%s %d %s %s yes %s\n",temp->val.id, temp->lineNo, t_str[temp->tk] , temp->parent->buf_name, temp->buf_name);
		else
		{
			if(temp->parent != NULL)
				printf("--- %d %s %s no %s\n", temp->lineNo, t_str[temp->tk] , temp->parent->buf_name, temp->buf_name);
			else
				printf("--- %d %s ROOT no %s\n", temp->lineNo, t_str[temp->tk], temp->buf_name);
		}	
	}
		

	if(temp->children != NULL)
	{
		node* first =  temp->children->next;
		while(first!=NULL)
		{
			printTreeConsole(first);
			first = first->next; 
		}	

	}	
	
}

void treeTraverse(node* temp)
{
	if(temp == NULL)
		return;
	treeTraverse(temp->children);

	count_nodes++;		

	if(temp->children != NULL)
	{
		node* first =  temp->children->next;
		while(first!=NULL)
		{
			treeTraverse(first);
			first = first->next; 
		}	

	}	
	
}

/** Parser stack functions **/
void initialize_parserStack()
{
	parserStack.top = -1;
}

void push(char* ele)
{
	parserStack.top++;
	parserStack.arr[parserStack.top] = ele;
}

char* top()
{
	char* temp;
	temp = parserStack.arr[parserStack.top];
	return temp;
}

char* pop()
{
	char* temp;
	temp = parserStack.arr[parserStack.top];
	parserStack.top--;
	return temp;
}

/** Parser stack functions ends**/

/** Tree functions start **/
node* createNode(char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf)
{
	node* n = (node*) malloc(sizeof(node));
	strcpy(n->buf_name,temp);
	n->t_or_nt = t_or_nt;
	n->numofchild = numofchild;
	n->tk = tk;
	n->lineNo = line;
	n->isLeafNode = isleaf;
	n->val = val;
	n->children = NULL;
	n->next = NULL;
	n->parent = NULL;

	return n;
}

node* addSibling(node* n, char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf)
{
	if(n == NULL)
		return NULL;
	while(n->next != NULL)
		n = n->next;
	n->next = createNode(temp, t_or_nt, numofchild, tk, line, val, isleaf);
	n->next->parent = n->parent;

	return n->next;
}

node* addChildren(node* n, char* temp, int t_or_nt, int numofchild, Tokens tk, int line, data val, int isleaf)
{
	if(n != NULL)
	{
		if(n->children != NULL)
		{
			node* t = addSibling(n->children,temp,t_or_nt, numofchild, tk, line, val, isleaf);
			t->parent = n;
			return t;
		}
		else
		{
			n->children = createNode(temp,t_or_nt, numofchild, tk, line, val, isleaf);
			n->children->parent = n;
			return n->children;
		}
	}
	else
		return NULL;
}

/** Tree functions end **/

/** Tree Stack functions**/

void initialize_treeStack()
{
	ts.top = -1;
}

void pushTreeNode(g_node* temp, int childno)
{
	ts.top++;
	ts.arr[ts.top] = temp;
	ts.childno[ts.top] = childno;
}

g_node* popTreeNode(int *childout)
{
	g_node* temp = ts.arr[ts.top];
	*childout = ts.childno[ts.top];
	ts.top--;
	return temp;
}

g_node* topTreeNode(int *childout)
{
	g_node* temp = ts.arr[ts.top];
	*childout = ts.childno[ts.top];
	return temp;
}



/** Parent stack begins **/

void initialize_parentStack()
{
	ps.top = -1;
}

void pushParentNode(node* temp)
{
	ps.top++;
	ps.arr[ps.top] = temp;
}

node* popParentNode()
{
	node* temp = ps.arr[ps.top];
	ps.top--;
	return temp;
}

node* topParentNode()
{
	node* temp = ps.arr[ps.top];
	return temp;
}
