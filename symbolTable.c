
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/


#include "symbolTable.h"
#include "Hash.h"
#include "Hash.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int current_scope = -1;	//To maintain scurrent scope

int globalModuleNo = 0;	//To maintain current module no

int globalOffset = 0;	//To maintain offset within a module

int error =1;			//Total number of semantic errors.

node* globalDecl[20];  	//Array of modules defined at the top of the program.

char module_name[30][40]; //Module name for module no.

int reuse =0 ;//Number of module reuse statemenst where module was not declared.

int out = 0;

/** Function to initialize gobalDecl array **/
void initialize_globalDecl()
{
	int i=0;
	for(i=0; i<20; i++)
	{
		globalDecl[i] = NULL;
	}	
}

/** function to return the size of variable **/
int get_width(int type)
{
	if(type == 0)
		return 1; //boolean

	if(type == 1)
		return 2; //integer
	
	if(type == 2)
	 	return 4; //real
}

/** Functions for scope starts **/
void initialize_sarray()
{
	int i=0;
	for(i=0; i<30; i++)
	{
		sarray[i].scope_depth= -1;
		sarray[i].start=-1;
		sarray[i].end=-1;
		sarray[i].moduleNo=-1;
	}	
}

void initialize_mod_reuse_dec()
{
	int i=0;
	for(i=0; i<10; i++)
	{
		mod_reuse_dec[i].mod_reuse = NULL;
		mod_reuse_dec[i].scope = -1;
		mod_reuse_dec[i].moduleno = -1;
	}	
}

void initialize_output()
{
	int i=0;
	for(i=0; i<10; i++)
	{
		strcpy(output[i].name, "");
		output[i].set = -1;
	}	
}

void updateSarray(int start1, int end1, int moduleNo1)
{
	current_scope++;
	sarray[current_scope].scope_depth = current_scope;
	sarray[current_scope].start = start1;
	sarray[current_scope].end = end1;
	sarray[current_scope].moduleNo = moduleNo1;	
}

void updateSarrayDec()
{
	sarray[current_scope].scope_depth = -1;
	sarray[current_scope].start = -1;
	sarray[current_scope].end = -1;
	sarray[current_scope].moduleNo = -1;

	current_scope--;
}
/** Functions for scope ends **/

/** Creating and adding to Hashtables functions starts **/
void create_add_func(node* inp_plist1, node* out_plist1,int scope,int mno, char* nam, int s, int e)
{
	ft_node* check = find_func(nam);

	if(check == NULL)
	{
		ft_node* ft = (ft_node*) malloc(sizeof (ft_node));
		ft->inp_plist = inp_plist1;
		ft->out_plist = out_plist1;
		ft->scope_depth = scope;
		ft->moduleNo = mno;
		strcpy(ft->name, nam);
		ft->start_lineno = s;
		ft->end_lineno = e;

		add_func(ft);
	}
	else
	{
		printf("Error %d at Line %d: Module with name %s already there at line %d. Module redeclaration found at line %d\n",error, s, nam, check->start_lineno, s);//Again declared
		error++;
	}		
}

void create_add_sym(char* nam, int type1, int ifArr1, int lineNo,int start, int end, int Num1, int Num2, int scope, int offset1)
{
	st_node* check = find_Sym(nam, scope,globalModuleNo);

	if(check == NULL)
	{
		st_node* st = (st_node*) malloc(sizeof (st_node));
		strcpy(st->name, nam);
		st->type = type1;	
		st->ifArr = ifArr1;
		st->lineno = lineNo;
		st->num1 = Num1;
		st->num2 = Num2;
		st->start_line = start;
		st->end_line = end;
		st->scope_depth = scope;
		st->offset = offset1;
		st->moduleNo =  globalModuleNo;

		add_sym(st);
	}
	else
	{
		printf("Error %d at Line %d:Identifier with name %s already there at line %d. Identifier redeclaration found at line %d\n",error,lineNo, nam, check->lineno, lineNo);//Again declared
		error++;
	}		
}
/** Creating and adding to Hashtables functions ends **/

/** Function to return type of variable**/
int whichType(char* name)
{
	if(strcmp(name, "BOOLEAN") == 0)
	{
		return 0;
	}
	else if(strcmp(name, "INTEGER") == 0)
	{
		return 1;
	}
	else if(strcmp(name, "REAL") == 0)
	{
		return 2;
	}	
}
/** Function to return type char **/
char* whichTypeChar(int val)
{
	if(val == 0)
	{
		return "BOOLEAN" ;
	}
	else if(val == 1)
	{
		return "INTEGER";
	}
	else if(val == 2)
	{
		return "REAL";
	}	
}
/** Expression Analysis Begins **/

int expr_func(node* expr)
{
	if(strcmp(expr->children->buf_name, "arithmeticOrBooleanExpr") == 0)
	{
		return ABE_func(expr->children);
	}
	else
	{
		int out = arithmeticExpr_func(expr->children->next);
		if(out == 0)
		{
			printf("Error %d at Line %d: Type mismatch, Can't have Boolean\n", error, expr->lineNo);	//PLUS or MINUS can't be done with boolean types
			error++;
		}

		return out;	
			
	}	
}

int ABE_func(node* abe)
{
	if(strcmp(abe->children->buf_name, "anyterm") == 0)
	{
		return anyterm_func(abe->children);
	}
	else
	{
		node* logop = abe->children;
		int val1 = anyterm_func(logop->children);
		int val2 = anyterm_func(logop->children->next);

		if(val1 != 0 || val2 != 0)
		{
			printf("Error %d line %d: Type mismatch, can have only boolean in logical op\n", error, abe->lineNo);//can have only boolean in logical op.
			error++;
		}

		node* temp = logop->next;

		while(temp != NULL)
		{
			int val3 = anyterm_func(temp->children);
			if(val3 != 0)
			{
				printf("Error %d at line %d: Type mismatch, can have only boolean in logical op\n",error, abe->lineNo);
				error++;
			}	

			temp = temp->next;
		}
		return 0;	//logicalOp returns Boolean	
	}
}

int anyterm_func(node* anyterm)
{
	if(strcmp(anyterm->children->buf_name, "arithmeticExpr") == 0)
	{
		return arithmeticExpr_func(anyterm->children);
	}
	else
	{
		node* relop = anyterm->children;
		int val1 = arithmeticExpr_func(relop->children);
		int val2 = arithmeticExpr_func(relop->children->next);

		if(val1 == 0 || val2 == 0)
		{
			printf("Error %d at line %d: Type mismatch, can't have boolean in relation op\n",error, anyterm->lineNo);
			error++;
		}

		node* temp = relop->next;

		while(temp != NULL)
		{
			int val3 = arithmeticExpr_func(temp->children);
			if(val3 == 0)
			{
				printf("Error %d at line %d: Type mismatch, can't have boolean in relation op\n",error, anyterm->lineNo);
				error++;
			}	

			temp = temp->next;
		}

		return 0;	//Should return Boolean.	
	}	
}

int arithmeticExpr_func(node* arithmeticExpr)
{
	if(strcmp(arithmeticExpr->children->buf_name, "term") == 0)
	{
		return term_func(arithmeticExpr->children);
	}
	else
	{
		int check = 0;
		node* op1 = arithmeticExpr->children;
		int val1 = term_func(op1->children);
		int val2 = term_func(op1->children->next);

		if(val1 == 0 || val2 == 0)
		{
			printf("Error %d at Line %d: Type mismatch, can't have boolean in op1\n", error, arithmeticExpr->lineNo);
			error++;
		}
		if(val1 == 2 || val2 == 2)
		{
			check = 1;
		}

		node* temp = op1->next;

		while(temp != NULL)
		{
			int val3 = term_func(temp->children);
			if(val3 == 0)
			{
				printf("Error %d at Line %d: Type mismatch, can't have boolean in op1\n", error, arithmeticExpr->lineNo);
				error++;
			}
			else if(val3 == 2)
				check = 1;	

			temp = temp->next;
		}
		if(check == 1)
			return 2;
		else
			return 1;

	}
}

int term_func(node* term)
{
	if(strcmp(term->children->buf_name, "factor") == 0)
	{
		return factor_func(term->children);
	}
	else
	{
		int check = 0;
		node* op2 = term->children;
		int val1 = factor_func(op2->children);
		int val2 = factor_func(op2->children->next);

		if(val1 == 0 || val2 == 0)
		{
			printf("Error %d at Line %d:Type mismatch, can't have boolean in op2\n",error, term->lineNo);
			error++;
		}
		if(val1 == 2 || val2 == 2)
		{
			check = 1;
		}

		node* temp = op2->next;

		while(temp != NULL)
		{
			int val3 = factor_func(temp->children);
			if(val3 == 0)
			{
				printf("Error %d at Line %d:Type mismatch, can't have boolean in op2\n",error, term->lineNo);
				error++;
			}
			else if(val3 == 2)
				check = 1;	

			temp = temp->next;
		}
		if(check == 1)
			return 2;
		else
			return 1;	
	}
}

int factor_func(node* factor)
{

	if(strcmp(factor->children->buf_name, "arithmeticOrBooleanExpr") == 0)
	{
		return ABE_func(factor->children);
	}
	else
	{
		node* temp = factor->children->children;

		if(strcmp(temp->buf_name, "NUM") == 0)
		{
			return 1;
		}
		else if(strcmp(temp->buf_name, "RNUM") == 0)
		{
			return 2;
		}
		else if (strcmp(temp->buf_name, "ID") == 0)
		{
			int check =0;

			if(temp->children != NULL)
			{
				node* ind_id = temp->children;
				
				int t = findDeclared_scope(ind_id->val.id, current_scope);

				if(t != -1)
				{
					st_node* st = find_Sym(ind_id->val.id,t,globalModuleNo);
					
					if(st->type != 1)
					{
						printf("Error %d at Line %d: Type mismatch,  index has to be NUM\n", error, factor->lineNo);
						error++;
					}	
				}	
				else
				{
					printf("Error %d at Line %d: Identifier %s not declared before use.\n", error,ind_id->lineNo, ind_id->val.id );
					error++;
					check = 1;
				}			

			}
			
			int t = findDeclared_scope(temp->val.id, current_scope);

			if(t != -1)
			{
				st_node* st = find_Sym(temp->val.id,t,globalModuleNo);
				if(temp->children != NULL)
				{
					if(st->ifArr != 1)
					{
						printf("Error %d at Line %d: Identifier %s is not declared as Array.\n",error, temp->lineNo, temp->val.id);
						error++;
						return -1;
					}

				}	
				return st->type;
			}	
			else
			{
				printf("Error %d at Line %d: Identifier %s  not declared before use.\n", error,  temp->lineNo, temp->val.id);
				error++;
				return -1;
			}		
		}
	}			
}

/** Expression Analysis Ends **/

/** Function to find scope of a variable name **/
int findDeclared_scope(char* sym, int current) 
{
	st_node* temp = find_Sym(sym, current,globalModuleNo);

	if(temp != NULL)
	{
		return current;
	}
	else
	{
		int i=0;
		for(i=current-1;i>=0;i--)
		{
			if(sarray[i].moduleNo != sarray[current].moduleNo)
			{
				break;
			}

			if(sarray[i].end > sarray[current].start)
			{
				temp = find_Sym(sym, i,globalModuleNo);
				if(temp != NULL)
				{
					return i;
				}
			}								
		}
	}
	return -1; //never declared, default case, if not returned above
}

/** Function for otherModules **/
void otherModulescase(node* otherModules)
{
	node* ID = otherModules->children;
	//printf("%s\n", ID->val.id);

	while(ID != NULL)		//ID->ID->ID......
	{
		node* inp_plist1 = ID->children;

		node* try = ID->children->next;

		if(strcmp(try->buf_name, "output_plist") == 0)
		{
			initialize_output();
			out =0;
			node* out_plist1 = try;
			
			node* start = out_plist1->next->children;
			node* check = out_plist1->next->children->next;

			if(strcmp(check->buf_name, "END") == 0)
			{
				updateSarray( start->lineNo, check->lineNo, globalModuleNo);
				addInputPlist(inp_plist1);
				addOutputPlist(out_plist1);
				create_add_func(inp_plist1, out_plist1, current_scope, globalModuleNo, ID->val.id, start->lineNo, check->lineNo);
				updateSarrayDec();
				
				printf("Error %d at Line %d: Output parameters not being set because module is empty.\n",error,  start->lineNo);//ouput parameters not being set.
				error++;
			}
			else
			{
				node* statements = check;
				node* end = out_plist1->next->children->next->next;
				updateSarray( start->lineNo, end->lineNo, globalModuleNo);
				addInputPlist(inp_plist1);
				addOutputPlist(out_plist1);

				create_add_func(inp_plist1, out_plist1, current_scope, globalModuleNo, ID->val.id, start->lineNo, end->lineNo);

				if(statements != NULL)
				{
					node* temp = statements->children;
					while(temp != NULL)
					{
						stmts(temp);
						temp = temp->next;	
					}	
				}
				updateSarrayDec();
			}

			int i=0;
			for(i=0; i<out; i++)
			{
				if(output[i].set == -1)
				{
					printf("Error %d at Line %d: The output variable %s is not assigned a value in module %s \n", error, ID->lineNo, output[i].name, ID->val.id );
					error++;
				}	
			}
		}
		else
		{
			node* moduleDef = ID->children->next;
			if(strcmp(moduleDef->children->next->buf_name, "END") != 0)
			{	
				node* statements = moduleDef->children->next;
				node* start = moduleDef->children;
				node* end = moduleDef->children->next->next;
				updateSarray( start->lineNo, end->lineNo, globalModuleNo);
				addInputPlist(inp_plist1);
				create_add_func(inp_plist1, NULL,current_scope, globalModuleNo, ID->val.id, start->lineNo, end->lineNo);
				

				if(statements != NULL)
				{
					node* temp = statements->children;
					while(temp != NULL)
					{
						stmts(temp);
						temp = temp->next;	
					}	
				}
				updateSarrayDec();
			}
			else
			{
				node* start = moduleDef->children;
				node* end = moduleDef->children->next;
				updateSarray( start->lineNo, end->lineNo, globalModuleNo);
				addInputPlist(inp_plist1);
				create_add_func(inp_plist1, NULL, current_scope, globalModuleNo, ID->val.id, start->lineNo, end->lineNo);
				updateSarrayDec();
				
			}	
		}	

		globalOffset =0;
		strcpy(module_name[globalModuleNo],	ID->val.id);
		globalModuleNo++;	
		ID = ID->next;
	}
}

void addInputPlist(node* inp_plist1)
{
	node* ID = inp_plist1->children;

	while(ID != NULL)
	{
		node* ch = ID->children;

		if(strcmp(ch->buf_name, "ARRAY") == 0)
		{
			node* type = ch->children;
			int t = whichType(type->children->buf_name);
			int Num1 = type->next->val.num;
			int Num2 = type->next->children->val.num;

			create_add_sym(ID->val.id, t, 1, ID->lineNo,sarray[current_scope].start, sarray[current_scope].end, Num1, Num2, current_scope, globalOffset);

			int inter1 = (Num2-Num1)*get_width(t);
			globalOffset = globalOffset + inter1; 
		}
		else
		{
			int t = whichType(ch->buf_name);
			create_add_sym(ID->val.id, t, 0, ID->lineNo,sarray[current_scope].start, sarray[current_scope].end, -1, -1, current_scope, globalOffset);

			int inter1 = get_width(t);
			globalOffset = globalOffset + inter1; 
		}

		ID = ID->next;	
	} 
}

void addOutputPlist(node* out_plist1)
{
	node* ID = out_plist1->children;

	while(ID != NULL)
	{
		node* ch = ID->children->children;		
		int t = whichType(ch->buf_name);
		
		create_add_sym(ID->val.id, t, 0, ID->lineNo,sarray[current_scope].start, sarray[current_scope].end, -1, -1, current_scope, globalOffset);

		int inter1 = get_width(t);
		globalOffset = globalOffset + inter1; 

		strcpy(output[out].name, ID->val.id); 		
		out++;

		ID = ID->next;	
	} 
}

void stmts(node* temp)
{
	if(strcmp(temp->buf_name, "declareStmt") == 0)
	{
		//printf("------------------------------%s %s %s \n", temp->children->val.id,temp->children->next->val.id,temp->children->next->val.id);
		node* ID = temp->children;
		node* type = ID->children;

		while(ID != NULL)
		{
			if(strcmp(type->buf_name, "ARRAY") == 0)
			{
				int t = whichType(type->children->children->buf_name);	
				int Num1 = type->children->next->val.num;
				int Num2 = type->children->next->children->val.num;

				create_add_sym(ID->val.id, t, 1, ID->lineNo,sarray[current_scope].start, sarray[current_scope].end, Num1, Num2, current_scope, globalOffset);

				int inter1 = (Num2-Num1)*get_width(t);
				globalOffset = globalOffset + inter1; 
			}
			else
			{
				int t = whichType(type->buf_name);	
				create_add_sym(ID->val.id, t, 0, ID->lineNo,sarray[current_scope].start, sarray[current_scope].end, -1, -1, current_scope, globalOffset);

				int inter1 = get_width(t);
				//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %s %d %d %d \n", ID->val.id, t, inter1, globalOffset);
				globalOffset = globalOffset + inter1;
				
			}

			ID = ID->next;
		}	
	}
	else if(strcmp(temp->buf_name, "ioStmt") == 0)
	{
		node* fir = temp->children;

		if(strcmp(fir->buf_name, "GET_VALUE") == 0)
		{
			node* ID = fir->children;

			int t = findDeclared_scope(ID->val.id, current_scope);

			if(t == -1)
			{
				//printf("Identifier %s not declared before use at line %d %d\n", ID->val.id,ID->lineNo, current_scope);//Not declared
				printf("Error %d at Line %d: Identifier %s not declared before use\n",error,ID->lineNo,  ID->val.id);
				error++;
			}
			else
			{
				st_node* st = find_Sym(ID->val.id, t, globalModuleNo);
				//later
			}				

		}
		else
		{
			node* var = fir->children;

			if(strcmp(var->children->buf_name, "ID") == 0)
			{
				int t = findDeclared_scope(var->children->val.id, current_scope);
				
				if(t == -1)
				{
					printf("Error %d at Line %d: Identifier %s not declared before use\n", error, var->children->lineNo, var->children->val.id);//Not declared
					error++;
				}	
				else
				{
					//might have to use the value
					st_node* ID1 = find_Sym(var->children->val.id, t, globalModuleNo);

					if(var->children->children != NULL)
					{
						//Array print
						int t1 = findDeclared_scope(var->children->children->val.id, current_scope);

						if(t1 == -1)
						{
							printf("Error %d at Line %d: Identifier %s not declared before use\n", error,var->children->children->lineNo, var->children->children->val.id);//Not declared
							error++;
						}
						else
						{
							st_node* ID2 = find_Sym(var->children->children->val.id, t1, globalModuleNo);
							if(ID2->type == 1)
							{
								//get value
							}
							else
							{
								printf("Error %d at Line %d: Type mismatch, Array index should be num\n",error,ID2->lineno);//Array index should be num.
								error++;
							}	
						}	
					}
					else
					{
						//later print ID
					}	
				}	
			}
			else
			{
				//later print num/rnum
			}	

		}	
	}
	else if(strcmp(temp->buf_name, "conditionalStmt") == 0)
	{
		node* ID = temp->children;
		int t = findDeclared_scope(ID->val.id, current_scope);

		if(t == -1)
		{
			printf("Error %d at Line %d: Identifier %s not declared before use\n",error, ID->lineNo, ID->val.id);//Not declared
			error++;
		}	
		else
		{
			st_node* st = find_Sym(ID->val.id, t, globalModuleNo);

			node* cs = ID->next->next;
			node* default1 = cs->next;

			if(st->type == 0 )//BOOLEAN
			{
				if(strcmp(default1->buf_name, "DEFAULT") == 0)
				{
					printf("Error %d at Line %d: Can't have a default statement with BOOLEAN switch statement\n", error, default1->lineNo);
					error++;
				}
				else
				{
					case_Stmts(cs,0);
				}	
			}
			else
			{
				if(st->type == 1)
				{	
					case_Stmts(cs,1);

					if(default1 != NULL)
					{
						node* statements = default1->children;
						node* brk = statements->next;
						updateSarray(default1->lineNo, brk->lineNo, globalModuleNo);//globalModuleNo not dealt

						node* ch = statements->children;
						while(ch != NULL)
						{
							stmts(ch);
							ch = ch->next;
						}
						updateSarrayDec();
						
					}	
					else
					{
						printf("Error %d at Line %d: Must have a default statement with switch statement (Num type)\n",error, st->lineno);
						error++;
					}
				}
				else
				{
					printf("Error %d at Line %d: The variable for switch statement has to be integer or boolean\n",error, st->lineno);//NOT integer or boolean
					error++;
				}	

			}	

		}	
	}
	else if(strcmp(temp->buf_name, "iterativeStmt") == 0)
	{
		node* ch = temp->children;
		if(strcmp(ch->buf_name, "FOR") == 0)
		{
			node* ID = ch->next;

			int t = findDeclared_scope(ID->val.id, current_scope);

			if(t == -1)
			{
				printf("Error %d at Line %d: Identifier %s not declared before use\n",error,ID->lineNo, ID->val.id);//Not declared
				error++;
			}
			else
			{
				st_node* st = find_Sym(ID->val.id, t, globalModuleNo);

				if(st->type == 1)
				{
					node* Num1 = ID->children;
					node* Num2 = Num1->children;

					node* strt = Num1->next;
					node* check = strt->next;
					
					if(strcmp(Num2->buf_name, "NUM") == 0 && strcmp(Num1->buf_name, "NUM") == 0)
					{
						if(Num2->val.num > Num1->val.num)
						{
							if(strcmp(check->buf_name, "END") == 0)
							{
								//updateSarray(strt->lineNo, check->lineNo, globalModuleNo);//globalModuleNo not dealt
							}	
							else
							{
								node* end = check->next;
								
								node* ch = check->children;
								updateSarray(strt->lineNo, end->lineNo, globalModuleNo);//globalModuleNo not dealt
								while(ch != NULL)
								{
									stmts(ch);
									ch = ch->next;
								}
								updateSarrayDec();
								
							}	
							
						}
						else
						{
							printf("Error %d at Line %d: NUM1 > NUM2\n",error, Num1->lineNo);//Num1 > Num2
							error++;
						}	
					}
					else
					{
						printf("Error %d at Line %d: Num1 or/and Num2 not integer\n",error,  Num1->lineNo);	
						error++;
					}	
						
				}
				else
				{
					printf("Error %d at Line %d: Identifier %s must be integer to be used in for loop\n",error, st->lineno, st->name);	//Not integer in for loop
					error++;
				}	

			}	
		}
		else
		{
			node* abe = ch->next;
			int j = ABE_func(abe);

			if(j != 0)
			{
				printf("Error %d at Line %d: Expression inside While loop must be Boolean type.\n",error, abe->lineNo);//Not boolean type
				error++;
			}	

			node* strt = abe->next;
			node* check = strt->next; //can be statements or end

			if(strcmp(check->buf_name, "END") == 0)
			{
				//updateSarray(strt->lineNo, check->lineNo, globalModuleNo);//globalModuleNo not dealt
			}
			else
			{
				node* end = check->next;
				updateSarray(strt->lineNo, end->lineNo, globalModuleNo);//globalModuleNo not dealt
				node* ch = check->children;
				while(ch != NULL)
				{
					stmts(ch);
					ch = ch->next;
				}
				updateSarrayDec();
				
			}	
		}	
	}	
	else if(strcmp(temp->buf_name, "assignmentStmt") == 0)
	{
		node* ID = temp->children;
		int t = findDeclared_scope(ID->val.id, current_scope);

		int i=0;
		for(i=0; i<out; i++)
		{
			if(strcmp(output[i].name, ID->val.id) == 0)
			{
				output[i].set = 1;
				break;
			}	
		}	
		
		if(t == -1)
		{
			printf("Error %d at Line %d: Identifier %s not declared before use\n",error, ID->lineNo, ID->val.id);//Not declared
			error++;
		}
		else
		{
			st_node* st = find_Sym(ID->val.id, t, globalModuleNo);

			if(st->ifArr == 1)
			{
				if(strcmp(ID->next->buf_name, "NUM") == 0 || strcmp(ID->next->buf_name, "ID") == 0)
				{
					if(strcmp(ID->next->buf_name, "ID") == 0)
					{
						int t1 = findDeclared_scope(ID->next->val.id, current_scope);

						if(t1 != -1)
						{	
							st_node* st2 = find_Sym(ID->next->val.id, t1, globalModuleNo);
							if(st2->type == 1)
							{
								int out = expr_func(ID->next->next);
								if(out != st->type)
								{
									printf("Error %d at Line %d: Type mismatch, between variable and expression\n",error,temp->lineNo);	//Type mismatch between variable and expression.
									error++;
								}	
							}
							else
							{
								printf("Error %d at Line %d: Index can only be of integer types\n", error, temp->lineNo);//Index can only be of integer types
								error++;
							}
						}
						else
						{
							printf("Error %d at Line %d: index variable not declared\n", error, temp->lineNo);//index variable not declared
							error++;
						}		
					}
					else
					{
						int out = expr_func(ID->next->next);
						if(out != st->type)
						{
							printf("Error %d at Line %d: Type mismatch, between variable and expression\n",error,  temp->lineNo);	//Type mismatch between variable and expression.
							error++;
						}	
					}	
				}
				else
				{
					printf("Error %d at Line %d: can't assign the value directly to the arr id\n",error, temp->lineNo); //can't assign the value directly to the arr id, int arr[10]; arr = expr; wrong
					error++;
				}	
			}
			else
			{
				if(strcmp(ID->next->buf_name, "NUM") == 0 || strcmp(ID->next->buf_name, "ID") == 0)
				{
					printf("Error %d at Line %d: It is not array, still accessing as an array\n",error,  temp->lineNo); //It is not array, still accessing as an array.	
					error++;
				}
				else
				{
					int out = expr_func(ID->next);
					if(out != st->type)
					{
						printf("Error %d at Line %d: Type mismatch between variable and expression\n",error, temp->lineNo);	//Type mismatch between variable and expression.
						error++;
					}	
				}
			}

		}				
	}
	else if(strcmp(temp->buf_name, "moduleReuseStmt") == 0)
	{
		node* ID= temp->children;
		node* inp_list = ID->children;
		node* out_list = ID->next;

		ft_node* ft =  find_func(ID->val.id);
		//printf("in module reuse statements\n");

		if(ft == NULL)
		{
			int i=0;

			for(i=0; i< 20; i++)
			{
				if(globalDecl[i] != NULL)
				{
					if(strcmp(ID->val.id, globalDecl[i]->val.id) == 0)
					{
						//check its inpultplist andoutput plist when it is declared
						mod_reuse_dec[reuse].mod_reuse = temp;
						mod_reuse_dec[reuse].scope = current_scope;
						mod_reuse_dec[reuse].moduleno = globalModuleNo;
						reuse++;
						break;
					}
				}
				else
				{
					printf("Error %d at Line %d: Module %s not declared or defined before use. \n",error, inp_list->lineNo, ID->val.id);//Using it before defining or declaring the func.
					error++;
					break;
				}		
			}
		}
		else
		{
			if(ft->moduleNo == globalModuleNo)
			{
				printf("Error %d at line %d: Recursive call not supported \n", error, ID->lineNo);
				error++;
			}	


			if(ft->out_plist != NULL)
			{
				node* formal = ft->out_plist->children;
				node* actual = out_list;

				if(out_list != NULL)
				{
					while(formal != NULL && actual != NULL)
					{
						st_node* st1 = find_Sym(formal->val.id,ft->scope_depth, ft->moduleNo);//formal parameters

						int t = findDeclared_scope(actual->val.id, current_scope);

						if(t != -1)
						{
							st_node* st2 = find_Sym(actual->val.id,t, globalModuleNo);//actual parameters

							if(st2->type != st1->type)
							{
								printf("Error %d at Line %d: Expecting ouput of type %s instead of %s for variable %s\n",error,inp_list->lineNo, whichTypeChar(st1->type), whichTypeChar(st2->type),st2->name);//type doesn't match.
								error++;
							}	
						}	
						else
						{
							printf("Error %d at Line %d: Input parameter %s not declared.\n",error, inp_list->lineNo, actual->val.id);//Not declared
							error++;
						}	

						actual = actual->next;
						formal = formal->next;
					}

					if(formal != NULL || actual != NULL)
					{
						printf("Error %d at Line %d: Number of ouput actual and formal parameters not same.\n",error, inp_list->lineNo);//No of parameters not same.
						error++;
					}	
				}	
				else
				{
					printf("Error %d at Line %d: There are no formal ouput parameters\n",error, inp_list->lineNo);//No outputs from function.
					error++;
				}	
			}


			node* formal_input = ft->inp_plist->children;
			node* actual_input = inp_list;
			if(inp_list != NULL)
			{
				while(formal_input != NULL && actual_input != NULL)
				{
					st_node* st1 = find_Sym(formal_input->val.id,ft->scope_depth, ft->moduleNo);//formal parameters

					int t = findDeclared_scope(actual_input->val.id, current_scope);

					if(t != -1)
					{
						st_node* st2 = find_Sym(actual_input->val.id,t, globalModuleNo);//actual parameters

						if(st2->type != st1->type)
						{
							printf("Error %d at Line %d: Expecting input of type %s instead of %s for variable %s\n", error,inp_list->lineNo, whichTypeChar(st1->type), whichTypeChar(st2->type),st2->name);//type doesn't match.
							error++;
						}	
					}	
					else
					{
						printf("Error %d at Line %d: Input parameter %s not declared.\n",error, inp_list->lineNo, actual_input->val.id);//Not declared
						error++;
					}	

					actual_input = actual_input->next;
					formal_input = formal_input->next;
				}

				if(formal_input != NULL || actual_input != NULL)
				{
					printf("Error %d at Line %d: Number of input actual and formal parameters not same.\n",error,  inp_list->lineNo);//No of parameters not same.
					error++;
				}	
			}	
			else
			{
				printf("Error %d at Line %d: There are no formal input parameters\n",error, inp_list->lineNo);//No outputs from function.
				error++;
			}	
		}	
	}
}

void driverModulecase(node* driverModule)
{
	node* moduleDef = driverModule->children;
	node* check = moduleDef->children->next;

	//printf("%s %s\n", moduleDef->buf_name, check->buf_name);

	if(strcmp(check->buf_name, "END") != 0)
	{
		node* statements = check;
		node* start = moduleDef->children;
		node* end = moduleDef->children->next->next;
		updateSarray( start->lineNo, end->lineNo, globalModuleNo);
		create_add_func(NULL, NULL,current_scope, globalModuleNo,"driver", start->lineNo, end->lineNo);
		
		
		node* temp = statements->children;
		while(temp != NULL)
		{
			stmts(temp);
			//printf("inside Driver module --- done with %s up next %s\n", temp->buf_name, temp->next->buf_name);
			temp = temp->next;
		}
		updateSarrayDec();
				
	}	
	else
	{
		node* start = moduleDef->children;
		node* end = moduleDef->children->next;
		updateSarray( start->lineNo, end->lineNo, globalModuleNo);
		create_add_func(NULL, NULL,current_scope, globalModuleNo, "driver" , start->lineNo, end->lineNo);
		updateSarrayDec();
		
	}
	strcpy(module_name[globalModuleNo],	"driver");
	globalOffset =0;
	globalModuleNo++;	
}

void case_Stmts(node* temp, int typeCheck)
{
	node* cas = temp->children;
	int cnt =0;

	while(cas!= NULL)
	{
		node* value = cas->children;

		if(strcmp(value->next->buf_name, "BREAK") != 0)
		{
			node* brk = value->next->next;
			if(typeCheck == 0 && (strcmp(value->children->buf_name, "TRUEs") == 0 || strcmp(value->children->buf_name, "FALSEs") == 0))
			{
				updateSarray( cas->lineNo, brk->lineNo, globalModuleNo);
				node* ch = value->next->children;
				while(ch != NULL)
				{
					stmts(ch);
					ch = ch->next;
				}	
			}
			else if(typeCheck == 1 && (strcmp(value->children->buf_name, "NUM") == 0))
			{
				updateSarray( cas->lineNo, brk->lineNo, globalModuleNo);
				node* ch = value->next->children;
				while(ch != NULL)
				{
					stmts(ch);
					ch = ch->next;
				}
			}
			else
			{
				printf("Error %d at Line %d: Type mismatch between the ID in Switch case and the case value.\n",error, cas->lineNo);
				error++;
			}	

		}	
			
		cas = cas->next;
		cnt++;
	}

	int i =0;
	for(i=0; i < cnt; i++)
	{
		updateSarrayDec();
	}	
}

void populateSymbolTable(node* AST)
{
	//<program> -> <moduleDeclarations><otherModules><driverModule><otherModules>

	initialize_sarray();
    initialize_HashTables();
    initialize_globalDecl();
    initialize_mod_reuse_dec();
    initialize_output();

    //moduleDeclarations
	node* moduleDecs = AST->children;
	node* ID = moduleDecs->children;
	int cnt = 0;

	while(ID != NULL)		//ID->ID->ID......
	{
		globalDecl[cnt] = ID;
		ID = ID->next;
		cnt++;
	}
	//printf("after moduleDecs\n");	

	//OtherModules1 begins 
	node* otherModules1 = AST->children->next;

	if(otherModules1->children != NULL)
		otherModulescase(otherModules1);

	//printf("after otherModules1\n");	

	//driverModule begins
	node* driverModule = AST->children->next->next;
	driverModulecase(driverModule);

	//printf("after driverModule\n");	

	//OtherModules2 begins 
	node* otherModules2 = AST->children->next->next->next;
	if(otherModules2->children != NULL)
		otherModulescase(otherModules2);
	//printf("after otherModules2\n");

	/** Module reuse statement where module was not declared starts**/
	int final_global_moduleno = globalModuleNo;
	int final_current_scope = current_scope;

	if(reuse >= 1)
	{
		int j=0;
		for(j=0; j < reuse; j++)
		{
			ft_node* ft =  find_func(mod_reuse_dec[j].mod_reuse->children->val.id);
			
			if(ft == NULL)
			{
				printf("Error %d at Line %d: Module %s not declared.\n",error, mod_reuse_dec[j].mod_reuse->children->lineNo,mod_reuse_dec[j].mod_reuse->children->val.id);
				error++;
			}
			else
			{
				globalModuleNo = mod_reuse_dec[j].moduleno;
				current_scope = mod_reuse_dec[j].scope;
				module_reuse_check(mod_reuse_dec[j].mod_reuse, ft);
			}	

		}	
	}	

	/** Module reuse statement where module was not declared ends**/

	/*int i=0;
	for(i=0; i< globalModuleNo; i++)
		printf("%s\n", module_name[i]);	*/

	printf("Semantic analysis done with *%d* errors detected\n", error-1);
}

void module_reuse_check(node* reuse_mod, ft_node* ft)
{
	node* ID= reuse_mod->children;
	node* inp_list = ID->children;
	node* out_list = ID->next;

	//printf("%s %d\n", ID->val.id, ID->lineNo);

	if(ft->out_plist != NULL)
	{
		node* formal = ft->out_plist->children;
		node* actual = out_list;

		if(out_list != NULL)
		{
			while(formal != NULL && actual != NULL)
			{
				st_node* st1 = find_Sym(formal->val.id,ft->scope_depth, ft->moduleNo);//formal parameters

				int t = findDeclared_scope(actual->val.id, current_scope);

				if(t != -1)
				{
					st_node* st2 = find_Sym(actual->val.id,t, globalModuleNo);//actual parameters

					if(st2->type != st1->type)
					{
						printf("Error %d at Line %d: Expecting ouput of type %s instead of %s for variable %s\n",error, inp_list->lineNo, whichTypeChar(st1->type), whichTypeChar(st2->type),st2->name);//type doesn't match.
						error++;
					}	
				}	
				else
				{
					printf("Error %d at Line %d: Input parameter %s not declared.\n",error, inp_list->lineNo, actual->val.id);//Not declared
					error++;
				}	

				actual = actual->next;
				formal = formal->next;
			}

			if(formal != NULL || actual != NULL)
			{
				printf("Error %d at Line %d: Number of ouput actual and formal parameters not same.\n",error, inp_list->lineNo);//No of parameters not same.
				error++;
			}	
		}	
		else
		{
			printf("Error %d at Line %d: There are no formal ouput parameters\n",error, inp_list->lineNo);//No outputs from function.
			error++;
		}	
	}

	node* formal_input = ft->inp_plist->children;
	node* actual_input = inp_list;
	if(inp_list != NULL)
	{
		while(formal_input != NULL && actual_input != NULL)
		{
			st_node* st1 = find_Sym(formal_input->val.id,ft->scope_depth, ft->moduleNo);//formal parameters

			int t = findDeclared_scope(actual_input->val.id, current_scope);

			if(t != -1)
			{
				st_node* st2 = find_Sym(actual_input->val.id,t, globalModuleNo);//actual parameters

				if(st2->type != st1->type)
				{
					printf("Error %d at Line %d: Expecting input of type %s instead of %s for variable %s\n",error,inp_list->lineNo, whichTypeChar(st1->type), whichTypeChar(st2->type),st2->name);//type doesn't match.	
					error++;
				}	
			}	
			else
			{
				printf("Error %d at Line %d: Input parameter %s not declared.\n",error, inp_list->lineNo, actual_input->val.id);//Not declared
				error++;
			}	

			actual_input = actual_input->next;
			formal_input = formal_input->next;
		}

		if(formal_input != NULL || actual_input != NULL)
		{
			printf("Error %d at Line %d: Number of input actual and formal parameters not same.\n",error,  inp_list->lineNo);//No of parameters not same.
			error++;
		}	
	}	
	else
	{
		printf("Error %d at Line %d: There are no formal input parameters\n",error,  inp_list->lineNo);//No outputs from function.
		error++;
	}
}

void printSymbolTable()
{
	hashnode_st* printer; 
	int i = 0;
	int cnt = 1;
	printf("sno\t\tLexeme\t\ttype\t\t\tModule\t\t\tlineno\t\tscope\t\twidth\t\toffset\n\n");
	for(i=0;i<15;i++)
	{
		if(hasharr_st[i] != NULL)
		{
			printer = hasharr_st[i];
			
			while(printer!=NULL)
			{
				printf("%d\t\t", cnt);										//sno
				printf("%s\t\t",printer->hnode->name);						//name
				
				if(printer->hnode->ifArr==1)								//type
				{
					int numm1= printer->hnode->num1;
					int numm2= printer->hnode->num2;
					int typee=printer->hnode->type;
					printf("array(%d,%s)\t\t", numm2-numm1+1, whichTypeChar(typee) );
					
				}
				else
				{
					printf("%s\t\t\t",whichTypeChar(printer->hnode->type));	
				}

				int scope= printer->hnode->scope_depth;
				//module-name
				char* nam = module_name[printer->hnode->moduleNo];
				printf("%s\t\t\t", nam);
				//line-nos
				printf("%d to %d\t\t", printer->hnode->start_line,printer->hnode->end_line);
				//level
				printf("%d\t\t", scope);
				
				if(printer->hnode->ifArr==1)									//width
				{
					int numm1= printer->hnode->num1;
					int numm2= printer->hnode->num2;
					int width= (numm2-numm1)*get_width(printer->hnode->type);
					printf("%d\t\t", width);
				}
				else
				{
					printf("%d\t\t",get_width(printer->hnode->type));	
				}

				printf("%d\n", printer->hnode->offset);

				printer=printer->next;
				cnt++;
			}
					
		}
	}	
}



