/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/
#include "lexer.h"
#include "lexer.c"
#include "parser.h"
#include "parser.c"
#include "codegen.c"
#include "symbolTable.h"
#include "symbolTable.c"
#include "AST.c"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{

	char *inputFile;

	if(argc == 2)
	{
		inputFile = argv[1];
	}
	else
	{
		printf("Please provide input file\n");
	}
	printf("***LEVEL 4: Symbol table/ AST/ Semantic Rules/ Type Checking modules work. Code generation implemented.***\n");		
	printf("Press option for the defined task.\n");
	printf("1 : For printing the token list (on the console) generated by the lexer. Each token appears in a new line alongwith the corresponding lexeme and line number.\n");
	printf("2 : For parsing to verify the syntactic correctness of the input source code and to produce parse tree (On Console)\n");
	printf("3 : For creating the abstract syntax tree and printing it appropriately (Inorder Traversal)\n");
	printf("4 : For displaying the amount of allocated memory and number of nodes to each of parse tree and abstract syntax tree for the test case used.\n");
	printf("5 : For reporting all type checking and semantic errors\n");
	printf("6 : For printing the Symbol Table in appropriate format showing all relevant information along with semantic errors.\n");
	printf("7 : For producing assembly code (only when there is no syntactic, semantic or type mismatch errors)\n");



	int choice=0;
	scanf("%d", &choice);

	if(choice == 1)
	{
		removeComments(inputFile, "cleancode.txt",0);
		printf("Starting lexical analysis. . .\n");
		lexicalOutput(inputFile);
		printf("Lexical analysis complete. . .\n");
		tokeninfo ti = getNextToken();
		int w=1;
		while( ti!=NULL )
		{
			if(strcmp(t_str[ti->tokentype], "NUM")==0)
				printf("%d %s %d\n",w, t_str[ti->tokentype], ti->value.num);
			else if (strcmp(t_str[ti->tokentype], "RNUM")==0)
				printf("%d %s %f\n",w, t_str[ti->tokentype], ti->value.rnum);
			else
				printf("%d %s %s\n",w, t_str[ti->tokentype], ti->value.id);

			ti = getNextToken();
			w++;
		}
		
	}
	else if(choice == 2)
	{
		//removeComments(inputFile, "cleancode.txt",1);
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");

		/*FILE *fp;
		fp = fopen(parseTreeOut,"w");*/
		printTreeConsole(p);
		printf("--------------------------------------------------------------------------------------------\n");
		
	}
	else if(choice == 3)
	{
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");

		node* head = buildAST(p);

		/*FILE *fp;
		fp = fopen(ASTout,"w");*/
		printTreeConsole(head);
		printf("--------------------------------------------------------------------------------------------\n");
		
	}
	else if(choice == 4)
	{
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");

		treeTraverse(p);
		printf("Parse Tree: Number of nodes = %d Allocated Memory = %lu\n", count_nodes, count_nodes* sizeof(p));
		unsigned long s1 = count_nodes* sizeof(p);
		count_nodes =0;
		node* head = buildAST(p);
		treeTraverse(head);
		unsigned long s2 = count_nodes* sizeof(head);
		unsigned long out = ((s1-s2)*100)/s1;
		printf("AST: Number of nodes = %d Allocated Memory = %lu\n", count_nodes, count_nodes* sizeof(head));
		printf("Compression percentage = %lu\n", out);
	}	
	else if(choice == 5)
	{
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");
		node* head = buildAST(p);
		printf("AST Built--------------------------------------------\n");
		populateSymbolTable(head);
	}
	else if(choice == 6)
	{
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");
		node* head = buildAST(p);
		printf("AST Built--------------------------------------------\n");
		populateSymbolTable(head);
		printf("Semantic errors displayed. Printing Symbol Table-----\n");
		printSymbolTable();
	}
	else if(choice == 7)
	{
		addGrammar();
		addFollow();
		addFirst();
		createParseTable();
		parseInputSourceCode("cleancode.txt");
		node* head = buildAST(p);

		FILE *fp;
		fp = fopen("code.asm","w");
		codegenerate(head,fp);
	}	
	else
		printf("Invalid option.\n");

return 0;
}	

	
