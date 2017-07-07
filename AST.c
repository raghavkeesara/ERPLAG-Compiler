
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/


#include "parser.h"
#include "lexer.h"
#include "AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* moduleDeclarationFunc(node* p)
{
	//<moduleDeclarations> -> <moduleDeclaration><moduleDeclarations>
	//<moduleDeclarations> -> <EPSILON>
	if(p->children != NULL)
	{
		node* temp = p->children;		//moduleDeclaration
		node* MD = buildAST(temp);

		node* temp2 = p->children->next;//moduleDeclarations
		MD->next = moduleDeclarationFunc(temp2);

		free(p);
		p = NULL;

		return MD;
	}	
	else
	{
		free(p);
		p = NULL;

		return NULL;	
	}

}
node* otherModulesFunc(node* p)
{
	//<otherModules> -> <module><otherModules>
	//<otherModules> -> <EPSILON>
	if(p->children != NULL)
	{
		node* temp = p->children;		//module
		//printf("%s\n", temp->buf_name);
		node* module = buildAST(temp);

		node* temp2 = p->children->next;//otherModules
		module->next = otherModulesFunc(temp2);

		free(p);
		p = NULL;

		return module;
	}	
	else
	{
		free(p);
		p = NULL;

		return NULL;	
	}
}

node* statementFunc(node* p)
{
	//<statements> -> <statement><statements>
	//<statements> -> <EPSILON>
	if(p->children != NULL)
	{
		node* temp = p->children;		//STATEMENT
		//printf("%s\n", temp->children->buf_name);
		node* statement = buildAST(temp);
		//printf("After statement\n");

		node* temp2 = p->children->next;//STATEMENTS

		node* out = statementFunc(temp2);

		if(out != NULL)
			statement->next = out;
		//printf("after recursive statements\n");
		//printf("%s\n", p->buf_name);
		free(p);
		p = NULL;

		//printf("%s\n", statement->buf_name);
		//printf("donw with stat func....\n");
		return statement;
	}
	else
	{
		free(p);
		p = NULL;

		return NULL;	
	}	
	
}

node* buildAST(node* head)
{
	node* p = head;

	if(strcmp(p->buf_name, "program") == 0)
	{
		buildAST(p->children);					//moduleDeclarations
		//printf("%s %s %s %s \n", p->children->children->val.id, p->children->children->next->val.id, p->children->children->next->next->val.id, p->children->children->next->next->next->buf_name);
		buildAST(p->children->next);			//otherModules
		/*node* temp = p->children->next;
		printf("%s\n", );*/
		buildAST(p->children->next->next);	//driverModule
		buildAST(p->children->next->next->next);//otherModules

		return p;
	}
	else if(strcmp(p->buf_name, "moduleDeclarations") == 0)
	{
		//<moduleDeclarations> -> <moduleDeclaration><moduleDeclarations>
		//<moduleDeclarations> -> <EPSILON>
		node* temp = p->children;				//moduleDeclaration

		if(temp != NULL)
		{
			//printf("inside MD^^^^^^^\n");
			node* out = buildAST(temp);
			node* temp1 = moduleDeclarationFunc(temp->next);

			p->children = out;
			p->children->parent = p;

			p->children->next = temp1;

			node* set = p->children->next;
			while(set!=NULL)
			{
				set->parent = p;
				set = set->next;
			}	

			//printf("after func call ^^^^^^^^^^\n");
			return p;
		}
		else
		{
			return NULL;
		}	
	}
	else if(strcmp(p->buf_name, "moduleDeclaration") == 0)
	{
		//<moduleDeclaration> -> <DECLARE><MODULE><ID><SEMICOL>
		node* temp = p->children->next->next;	//ID

		free(p->children->next->next->next);	//SEMICOL
		p->children->next->next->next = NULL;

		free(p->children->next);				//MODULE
		p->children->next = NULL;

		free(p->children);						//DECLARE
		p->children = NULL;

		free(p);
		p = NULL;
		//printf("inside MD:  %s\n", temp->val.id);

		return temp;
	}
	else if(strcmp(p->buf_name, "otherModules") == 0)
	{
		//<otherModules> -> <module><otherModules>
		//<otherModules> -> <EPSILON>
		node* temp = p->children;				//module

		if(temp != NULL)
		{
			//printf("inside OM first time\n");
			node* out = buildAST(temp);

			node* temp1 = otherModulesFunc(temp->next);

			p->children = out;
			p->children->parent = p;

			p->children->next = temp1;

			node* set = p->children->next;
			while(set!=NULL)
			{
				set->parent = p;
				set = set->next;
			}	
			return p;

		}
		else
			return NULL;
	}		
	else if(strcmp(p->buf_name, "module") == 0)
	{
		//<module> -> <DEF><MODULE><ID><ENDDEF><TAKES><INPUT><SQBO><input_plist><SQBC><SEMICOL><ret><moduleDef>
		node* temp = p->children;

		while(strcmp(temp->buf_name, "ID") != 0)
		{
			node* t = temp;
			temp = temp->next;
			free(t);
			t = NULL;
		}

		node* ID = temp;
		temp = temp->next;
		//printf("after ID\n");

		while(strcmp(temp->buf_name, "input_plist") != 0)
		{
			node* t = temp;
			temp = temp->next;
			free(t);
			t = NULL;
		}
		//printf("before input_plist\n");

		ID->children = buildAST(temp);	//input_plist
		ID->children->parent = ID;
		temp = temp->next;

		//printf("after input_plist\n");

		while(strcmp(temp->buf_name, "ret") != 0)
		{
			node* t = temp;
			temp = temp->next;
			free(t);
			t = NULL;
		}

		//printf("after return\n");

		node* out_pli = buildAST(temp);

		if(out_pli != NULL)
		{
			ID->children->next = out_pli;	//output_plist		
			ID->children->next->parent = ID;	
			temp = temp->next;
			node* out1 = buildAST(temp);

			if(out1 != NULL)
			{
				ID->children->next->next = out1;//moduleDef  (statements)
				ID->children->next->next->parent = ID;
			}

			free(p);
			p = NULL;

			return ID;

		}	
		else
		{
			temp = temp->next;
			node* out1 = buildAST(temp);

			if(out1 != NULL)
			{
				//printf("%s\n", ID->buf_name);
				//printf("%s\n", ID->children->buf_name);
				
				ID->children->next = out1;//moduleDef  (statements)
				ID->children->next->parent = ID;
				//printf("%s\n", ID->children->next->buf_name);
			}
			else
			{
				free(ID->children->next);
				ID->children->next = NULL;
			}	

			free(p);
			p = NULL;

			//printf("done with MODULE\n");
			//printf("&&&&&&&&&&&&&&&&&&&&&& %s %s %s\n", ID->val.id, ID->children->buf_name, ID->children->next->buf_name);
			return ID;
		}	
	}	
	else if(strcmp(p->buf_name, "moduleDef") == 0)
	{
		//<moduleDef> -> <START><statements><END>
		node* start = p->children;
		node* stmts = p->children->next;
		node* end = p->children->next->next;

		node* out = buildAST(stmts);
		//printf("inside moduleDef: done with stmts\n");
		//printf("%s\n", out->buf_name);

		if(out != NULL)
		{
			//printf("inside moduleDef: not null part\n");
			start->next = out;
			start->next->parent = p;
			start->next->next = end;
			start->next->next->parent = p;
			//printf("inside moduleDef: not null part done-----\n");
			//printf("1212121212121212 212 %s %s %s %s \n", p->buf_name, p->children->buf_name, p->children->next->buf_name, p->children->next->next->buf_name);
			return p;
		}
		else
		{
			//printf("inside null part of moduleDef......\n");

			start->next = end;
			start->next->parent = p;
			//printf("inside moduleDef: not null part done-----\n");
			//printf("3434343434343434 %s %s %s \n", p->buf_name, p->children->buf_name, p->children->next->buf_name);
			return p;
		}	
	}
	else if(strcmp(p->buf_name, "statements") == 0)
	{
		//<statements> -> <statement><statements>
		//<statements> -> <EPSILON>
		node* temp = p->children;		//statement
		if(temp != NULL)
		{
			node* one = buildAST(temp);
			p->children = one;

			//printf("Inside statements\n");
			one->next = statementFunc(temp->next);
			//printf("@@@@@@@@@@ %s\n", temp1->buf_name);


			node* set = p->children;

			while(set != NULL)
			{
				//printf("@@@@@@@@@@ %s\n", set->buf_name);
				set->parent = p;
				set = set->next;
			}


			//printf("Done with statements\n");
			//printf("%s\n", p->buf_name);
			return p;
		}
		else
		{
			free(p);
			p = NULL;

			return NULL;		
		}			
	}
	else if(strcmp(p->buf_name, "statement") == 0)
	{
		node* temp = p->children;
		node* temp1 = buildAST(temp);
		//printf("%s\n", temp1->buf_name);

		free(p);
		p = NULL;
		
		return temp1;
	}
	else if(strcmp(p->buf_name, "ioStmt") == 0)
	{
		//<ioStmt> -> <GET_VALUE><BO><ID><BC><SEMICOL>
		//<ioStmt> -> <PRINT><BO><var><BC><SEMICOL>
		//printf("inside ioStmt\n");
		node* temp = p->children;

		if(strcmp(temp->buf_name, "GET_VALUE") == 0)
		{
			node* id = temp->next->next; 
			free(temp->next->next->next->next); //freeing semicolon
			temp->next->next->next->next = NULL;

			free(temp->next->next->next); //freeing BC
			temp->next->next->next = NULL;

			free(temp->next); //freeing BO
			temp->next = NULL;

			temp->children = id;
			temp->children->parent = temp;
			p->children = temp;
			p->children->parent = p;

			return p;
		}
		else if(strcmp(temp->buf_name, "PRINT") == 0)
		{
			node* var = temp->next->next; //this is var case
			
			free(temp->next->next->next->next); //freeing semicolon
			temp->next->next->next->next = NULL;

			free(temp->next->next->next); //freeing BC
			temp->next->next->next = NULL;

			free(temp->next); //freeing BO*/
			temp->next = NULL;


			temp->children = buildAST(var);
			temp->children->parent = temp;
			p->children = temp;
			p->children->parent = p;

			return p;
		}		
	}
	else if(strcmp(p->buf_name, "var") == 0)
	{
		node* temp = p->children;
		//<var> -> <ID><whichId>
		//<var> -> <NUM>
		//<var> -> <RNUM>

		if(strcmp(temp->buf_name, "ID") == 0)
		{
			//<whichId> -> <SQBO><ID><SQBC>
			//<whichId> -> <EPSILON>
			node* whichId = temp->next;
			if(whichId->children != NULL)
			{
				//printf("inside whichId indise var\n");
				node* ID = whichId->children->next;		//ID

				free(whichId->children->next->next);	//SQBC				
				whichId->children->next->next = NULL;

				free(whichId->children);				//SQBO
				whichId->children = NULL;

				temp->children = ID;
				temp->children->parent = temp;
				temp->next = NULL;

				return p;

			}
			else
			{
				free(whichId);
				whichId = NULL;
				temp->next = NULL;
				return p;
			}		
		}
		else
		{
			return p;	
		}			
	}
	else if(strcmp(p->buf_name, "simpleStmt") == 0)
	{
		//printf("inside simpleStmt\n");
		node* temp = p->children;
		//printf("%s\n", temp->buf_name);
		free(p);
		p = NULL;

		return buildAST(temp);
	}
	else if(strcmp(p->buf_name, "declareStmt") == 0)
	{
		//<declareStmt> -> <DECLARE><idList><COLON><dataType><SEMICOL>
		//printf("inside declare stmt\n");
		node* idlist = p->children->next;
		node* dataType = idlist->next->next;

		free(dataType->next);//SEMICOL
		dataType->next = NULL;

		free(idlist->next);//COLON
		idlist->next = NULL;

		free(p->children);//DECLARE
		p->children = NULL;

		//printf("before idList\n");
		p->children = buildAST(idlist);
		//p->children->parent = p;

		node* set = p->children;
		while(set != NULL)
		{
			//printf("ZZZZZZZZZZZZZ: %s\n", set->val.id);
			set->parent = p;
			set = set->next;
		}	

		//printf("beofre dataType\n");

		p->children->children  = buildAST(dataType);
		p->children->children->parent = p->children;

		//printf("done with declareStmt\n");

		return p;
	}
	else if(strcmp(p->buf_name, "idList") == 0)
	{
		//<idList> -> <ID><n3>
		//<n3> -> <COMMA><ID><n3>
		//<n3> -> <EPSILON>
		//printf("inside idList\n");
		node* ID = p->children;
		//printf("inside idList: %s\n", ID->buf_name);
		node* n3 = ID->next;
		//printf("%s %s\n", n3->buf_name, n3->children->buf_name);

		if(n3->children != NULL)
		{
			node* ID1 = buildAST(n3);

			ID->next = NULL;
			ID->next = ID1;
		}
		else
		{
			free(p->children->next);
			p->children->next = NULL;

		}		

		free(p);
		p = NULL;

		return ID;
	}
	else if(strcmp(p->buf_name, "n3") == 0)
	{
		//<n3> -> <COMMA><ID><n3>
		//<n3> -> <EPSILON>
		if(p->children != NULL)
		{
			node* ID = p->children->next;
			node* n3 = ID->next;

			free(p->children);
			p->children = NULL;

			node* ID1 = buildAST(n3);

			ID->next = NULL;
			ID->next = ID1;

			free(p);
			p = NULL;

			return ID;

		}
		else
		{
			free(p);
			p= NULL;

			return NULL;
		}	
	}	
	else if(strcmp(p->buf_name, "conditionalStmt") == 0)
	{
		//printf("inside conditionalStmt\n");
		//<conditionalStmt> -> <SWITCH><BO><ID><BC><START><caseStmts><default1><END>
		node* ID = p->children->next->next;
		node* caseStmt = ID->next->next->next;
		node* default1 = caseStmt->next;

		node* end = default1->next;	//END
		node* start = ID->next->next;	//START

		free(ID->next);			//BC
		ID->next = NULL;

		free(p->children->next);//BO
		p->children->next = NULL;

		free(p->children);		//SWITCH
		p->children = NULL;

		ID->next = start;
		start->next = buildAST(caseStmt);
		//printf("inside conditionalStmt : done with stmts\n");

		node* out = buildAST(default1);

		if(out != NULL)
		{
			start->next->next = buildAST(default1);
			//printf("inside conditionalStmt: done with default1\n");
			start->next->next->next = end;
		}	
		else
		{
			start->next->next = end;
		}	

		p->children = ID;

		node* set = p->children;

		while(set != NULL)
		{
			set->parent = p;
			set = set->next;
		}	

		return p;
	}	
	else if(strcmp(p->buf_name, "caseStmts") == 0)
	{
		//printf("inside caseStmts\n");
		//<caseStmts> -> <CASE><value><COLON><statements><BREAK><SEMICOL><n9>
		node* cas = p->children;
		node* value = p->children->next;
		node* stmts = value->next->next;
		node* brk = stmts->next;
		node* n9 = stmts->next->next->next;

		free(stmts->next->next);//SEMICOL
		stmts->next->next = NULL;

		free(value->next);		//COLON
		value->next = NULL;

		p->children = cas;
		cas->children = value;
		node* hj = buildAST(stmts);

		if(hj != NULL)
		{
			value->next = hj;
			value->next->next = brk;

			node* set1 = cas->children;

			while(set1 != NULL)
			{
				set1->parent = cas;
				//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@---%s %s\n", set1->buf_name, set1->parent->buf_name);
				set1 = set1->next;
			}
		
			cas->next = buildAST(n9);

			node* set = p->children;

			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}	
		}
		else
		{
			value->next = brk;
			brk->next = NULL;
		}	
		

		//printf("done with case stmts\n");
		//printf("%s\n", p->parent->buf_name);

		return p;
	}
	else if(strcmp(p->buf_name, "default1") == 0)
	{
		//<default1> -> <DEFAULT><COLON><statements><BREAK><SEMICOL> 
		//<default1> -> <EPSILON>
		node* def = p->children;

		if(def != NULL)
		{
			node* stmts = def->next->next;
			node* brk = stmts->next;

			free(stmts->next->next);//SEMICOL
			stmts->next->next = NULL;

			free(def->next);		//COLON
			def->next = NULL;

			node* out = buildAST(stmts);
			def->children = out;
			out->next = brk;
			brk->next = NULL;

			free(p);
			p = NULL;

			return def;
		}
		else
		{
			free(p);
			p = NULL;

			return NULL;	
		}	
	}
	else if(strcmp(p->buf_name, "n9") == 0)
	{
		//<n9> -> <CASE><value><COLON><statements><BREAK><SEMICOL><n9> 
		//<n9> -> <EPSILON>
		node* temp = p->children;

		if(temp != NULL)
		{
			node* cas = p->children;
			node* value = p->children->next;//left
			node* stmts = value->next->next;
			node* brk = stmts->next;
			node* n9 = stmts->next->next->next;

			free(stmts->next->next);//SEMICOL
			stmts->next->next = NULL;

			free(value->next);		//COLON
			value->next = NULL;

			cas->children = value;
			value->next = buildAST(stmts);
			value->next->next = brk;
			brk->next = NULL;

			node* set1 = cas->children;

			while(set1 != NULL)
			{
				set1->parent = cas;
				set1 = set1->next;
			}

		
			cas->next = buildAST(n9);
			
			free(p);
			p = NULL;

			return cas;
		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}		
	}	
	else if(strcmp(p->buf_name, "iterativeStmt") == 0)
	{
		//printf("inside iterativeStmt\n");
		//<iterativeStmt> -> <FOR><BO><ID><IN><range><BC><START><statements><END>
		node* temp = p->children;

		if(strcmp(temp->buf_name, "FOR") == 0)
		{
			node* ID = temp->next->next;
			node* range = ID->next->next;
			node* stmts = range->next->next->next;

			node* end = stmts->next;	//END

			node* start = range->next->next;//START

			free(range->next);	//BC
			range->next = NULL;

			free(ID->next);	//IN
			ID->next = NULL;

			free(temp->next);	//BO
			temp->next = NULL;

			ID->children = buildAST(range);
			ID->children->parent = ID;
			ID->children->next = start;
			ID->children->next->parent = ID;

			node* out = buildAST(stmts);

			if(out != NULL)
			{
				start->next = out;
				start->next->next = end;

				start->next->parent = ID;
				start->next->next->parent = ID;
			}
			else
			{
				start->next = end;
				start->next->parent = ID;					
			}	
			
			p->children->next = ID;
			p->children->next->parent = p;
	
			return p;
		}
		else//<iterativeStmt> -> <WHILE><BO><arithmeticOrBooleanExpr><BC><START><statements><END>	
		{
			node* abe = temp->next->next;
			node* stmts = abe->next->next->next;

			node* end = stmts->next;	//END
			node* start = abe->next->next;//START

			free(abe->next);	//BC
			abe->next = NULL;

			free(temp->next);	//BO
			temp->next = NULL;

			p->children->next = buildAST(abe);
			//printf("after abe inside while\n");

			p->children->next->next = start;
			node* out = buildAST(stmts);

			if(out != NULL)
			{
				start->next = out;
				start->next->next = end;
			}
			else
			{
				start->next = end;				
			}	

			node* set = p->children;
			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}	

			return p;
		}	
	}	
	else if(strcmp(p->buf_name, "moduleReuseStmt") == 0)	
	{
		//printf("inside moduleReuseStmt\n");
		//<moduleReuseStmt> -> <optional><USE><MODULE><ID><WITH><PARAMETERS><idList><SEMICOL>
		node* option = p->children;		
		node* ID = option->next->next->next;
		node* IDlist = ID->next->next->next;

		free(IDlist->next);
		IDlist->next = NULL;

		free(ID->next->next);
		ID->next->next = NULL;

		free(ID->next);
		ID->next = NULL;

		free(option->next->next);
		option->next->next = NULL;

		free(option->next);
		option->next = NULL;

		p->children = ID;
		p->children->parent = p;
		//printf("single ID inside moduleReuseStmt\n");	

		p->children->children = buildAST(IDlist);

		node* set = p->children->children;

		while(set != NULL)
		{
			set->parent = p->children;
			set = set->next;
		}	

		node* out = buildAST(option);

		if(out != NULL)
		{
			p->children->next = out;
			node* set1 = p->children->next;

			while(set1 != NULL)
			{
				set1->parent = p;
				set1 = set1->next;
			}	


			//printf("after option inside moduleReuseStmt\n");
		}
		else
		{
			free(p->children->next);
			p->children->next = NULL;
		}	

		//printf("done with moduleReuseStmt\n");

		return p;
	}	
	else if(strcmp(p->buf_name, "optional") == 0)	
	{
		//<optional> -> <SQBO><idList><SQBC><ASSIGNOP>
		//<optional> -> <EPSILON>
		node* temp = p->children;

		if(temp != NULL)
		{
			node* IDlist = temp->next;
			node* assignop = IDlist->next->next;

			free(IDlist->next);
			IDlist->next = NULL;

			free(temp);
			temp=NULL;

			node* out1 = buildAST(IDlist);
			out1->children = assignop;
			out1->children->parent = out1;
			//printf("done with optional\n");

			free(p);
			p= NULL;
			return out1;
		}
		else
		{
			//printf("done with optional null\n");
			free(p);
			p= NULL;
			return NULL;
		}	
	}	
	else if(strcmp(p->buf_name, "assignmentStmt") == 0)
	{
		//printf("inside assignmentStmt\n");
		//<assignmentStmt> -> <ID><whichStmt>

		node* temp = p->children;			//ID
		node* temp1 = p->children->next;	//WHichStmt
		
		node* temp2 = buildAST(temp1);
		//printf("%s^&^&^&&^&^&^&^&^&^&^&^&^&^&^&^&\n", temp2->buf_name);
		temp->next = temp2;

		node* set = temp;

		while(set != NULL)
		{
			set->parent = p;
			set = set->next;
		}	

		return p;
	}
	else if(strcmp(p->buf_name, "whichStmt") == 0)
	{
		//<whichStmt> -> <lvalueIDStmt>
		//<whichStmt> -> <lvalueARRStmt>
		node* temp = p->children;
		//printf("%s\n", temp->buf_name);
		free(p);
		p= NULL;
		return buildAST(temp);
	}
	else if(strcmp(p->buf_name, "lvalueIDStmt") == 0)
	{
		//<lvalueIDStmt> -> <ASSIGNOP><expression><SEMICOL>	
		node* expr = p->children->next;
		//printf("%s\n", expr->buf_name);
		free(expr->next);
		expr->next = NULL;

		free(p->children);
		p->children = NULL;

		free(p);
		p= NULL;

		return buildAST(expr);
	}
	else if(strcmp(p->buf_name, "lvalueARRStmt") == 0)
	{
		//<lvalueARRStmt> -> <SQBO><index1><SQBC><ASSIGNOP><expression><SEMICOL>
		node* index1 = p->children->next;
		node* expr = index1->next->next->next;

		free(expr->next);
		expr->next = NULL;

		free(index1->next->next);
		index1->next->next = NULL;

		free(index1->next);
		index1->next = NULL;

		free(p->children);
		p->children = NULL;

		node* temp1 = buildAST(index1);
		node* temp2 = buildAST(expr);

		temp1->next = temp2;	// NUM/ID->expr	//NOT COMPLETE: //EXPRE left index1 left
		free(p);
		p = NULL;

		return temp1;
	}
	else if(strcmp(p->buf_name, "index1") == 0)
	{
		node* temp = p->children;
		free(p);
		p = NULL;

		return temp;
	}
	else if(strcmp(p->buf_name, "expression") == 0)
	{
		//<expression> -> <arithmeticOrBooleanExpr>
		//<expression> -> <MINUS><BO><arithmeticExpr><BC>
		//<expression> -> <PLUS><BO><arithmeticExpr><BC>

		node* temp = p->children;

		if(strcmp(temp->buf_name, "arithmeticOrBooleanExpr") == 0)
		{
			node* out = buildAST(temp);
			p->children = out;
			out->parent = p;

			node* set = out->next;
			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}	

			return p;
		}
		else if(strcmp(temp->buf_name, "MINUS") == 0)
		{
			node* a_expr = temp->next->next; 
			free(a_expr->next);
			a_expr->next = NULL;

			free(temp->next);
			temp->next = NULL;

			temp->next = buildAST(a_expr);
			temp->next->parent = p;

			return p;
		}
		else
		{
			node* a_expr = temp->next->next; 
			free(a_expr->next);
			a_expr->next = NULL;

			free(temp->next);
			temp->next = NULL;

			temp->next = buildAST(a_expr);
			temp->next->parent = p;

			return p;
		}	
	}
	else if(strcmp(p->buf_name, "arithmeticOrBooleanExpr") == 0)
	{
		//<arithmeticOrBooleanExpr> -> <anyterm><n7>
		//printf("inside AoB\n");
		node* n7 = buildAST(p->children->next);
		p->children->next = NULL;									//NOT SURE

		node* anyterm = buildAST(p->children);
		p->children = NULL;											//NOT SURE
		
		if(n7 != NULL)
		{
			n7->children->next = anyterm;
			n7->children->next->parent = n7;
			n7->children->parent = n7;

			p->children = n7;

			node* set = p->children;

			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}

			return p;
		}
		else
		{
			p->children = anyterm;
			p->children->parent = p;

			return p;
		}	
	}
	else if(strcmp(p->buf_name, "n7") == 0)
	{
		//<n7> -> <logicalOp><anyterm><n7>
		//<n7> -> <EPSILON>
		if(p->children != NULL)
		{
			//printf("Inside N7\n");
			node* logi = p->children;
			node* logicalOp = p->children->children;
			node* anyterm = logi->next;
			node* n7 = anyterm->next;

			node* out_n7 = buildAST(n7);
			anyterm->next = NULL;

			logicalOp->children = buildAST(anyterm);
			logicalOp->next = NULL;

			logicalOp->children->parent = logicalOp;

			//printf("After anyterm build inside n7\n");

			logicalOp->next = out_n7;

			//printf("After build N7\n");

			free(logi);
			logi = NULL;

			free(p);
			p = NULL;

			return logicalOp;
		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}	
	}
	else if(strcmp(p->buf_name, "anyterm") == 0)
	{
		//<anyterm> -> <arithmeticExpr><n8>
		//printf("inside anyterm \n");
		node* a_expr = p->children;
		node* n8 = p->children->next;

		node* out_n8 = buildAST(n8);
		a_expr->next = NULL;

		node* temp = buildAST(a_expr);
		p->children = NULL;

		if(out_n8 != NULL)
		{
			out_n8->children->next = temp;
			out_n8->children->next->parent = out_n8;
			out_n8->children->parent = out_n8;

			p->children = out_n8;

			node* set = p->children;

			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}

			return p;
		}
		else
		{
			p->children = temp;
			p->children->parent = p;

			return p;
		}	
	}
	else if(strcmp(p->buf_name, "n8") == 0)
	{
		//<n8> -> <relationalOp><arithmeticExpr><n8> 
		//<n8> -> <EPSILON>
		if(p->children != NULL)
		{
			node* rela = p->children;
			node* relationalOp = p->children->children;
			node* a_expr = rela->next;
			node* n8 = a_expr->next;

			node* out_n8 = buildAST(n8);
			a_expr->next = NULL;															//NOT SURE

			relationalOp->children = buildAST(a_expr);
			relationalOp->next = NULL;														//NOT SURE

			relationalOp->children->parent = relationalOp;
			relationalOp->next = out_n8;

			free(rela);
			rela = NULL;

			free(p);
			p = NULL;

			return relationalOp;
		}
		else
		{
			free(p);
			p = NULL;
			return NULL;
		}			
	}
	else if(strcmp(p->buf_name, "arithmeticExpr") == 0)
	{
		//<arithmeticExpr> -> <term><n4>
		node* term = p->children;
		node* n4 = p->children->next;
		//printf("inside arithmeticExpr\n");

		node* out_n4 = buildAST(n4);
		//printf("after n4 inside arithmeticExpr\n");
		term->next = NULL;

		node* out = buildAST(term);
		//printf("after term inside arithmeticExpr\n");

		if(out_n4 != NULL)
		{
			//printf("1\n");
			out_n4->children->next = out;
			out_n4->children->next->parent = out_n4;
			out_n4->children->parent = out_n4;

			p->children = out_n4;
			//printf("2\n");
			node* set = p->children;

			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}
			//printf("3\n");

			return p;
		}	
		else
		{
			//printf("wtf\n");
			p->children = out;
			p->children->parent = p;
			//printf("%s %s\n", p->buf_name,p->parent->buf_name);

			return p;
		}	
	}
	else if(strcmp(p->buf_name, "n4") == 0)
	{
		//<n4> -> <op1><term><n4>
		//<n4> -> <EPSILON>
		if(p->children != NULL)
		{
			node* val = p->children;
			node* op1 = p->children->children;
			node* term = val->next;
			node* n4 = term->next;

			node* out_n4 = buildAST(n4);
			term->next = NULL;													//NOT SURE --------------------

			op1->children = buildAST(term);
			op1->children->parent = op1;
			op1->next = NULL;													//NOT SURE --------------------

			op1->next = out_n4;

			free(val);
			val = NULL;

			free(p);
			p = NULL;

			return op1;

		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}							
	}
	else if(strcmp(p->buf_name, "term") == 0)
	{
		//<term> -> <factor><n5>
		node* factor = p->children;
		node* n5 = p->children->next;

		node* out_n5 = buildAST(n5);
		p->children->next = NULL;
		node* out = buildAST(factor);

		if(out_n5 != NULL)
		{
			out_n5->children->next = out;
			out_n5->children->next->parent = out_n5;
			out_n5->children->parent = out_n5;

			p->children = out_n5;

			node* set = p->children;

			while(set != NULL)
			{
				set->parent = p;
				set = set->next;
			}	

			return p;
		}
		else
		{
			p->children = out;
			p->children->parent = p;

			return p;
		}	
	}
	else if(strcmp(p->buf_name, "factor") == 0)
	{
		//<factor> -> <BO><arithmeticOrBooleanExpr><BC>
		//<factor> -> <var>
		if(strcmp(p->children->buf_name, "BO") == 0)
		{
			node* bo = p->children;
			node* abe = p->children->next;
			node* bc = abe->next;

			node* out = buildAST(abe);

			free(bo);
			bo = NULL;

			free(bc);
			bc = NULL;

			p->children = out;
			p->children->parent = p;
			p->children->next = NULL;

			return p;
		}
		else
		{
			return p;
		}
	}
	else if(strcmp(p->buf_name, "n5") == 0)
	{
		//<n5> -> <op2><factor><n5>
		//<n5> -> <EPSILON>
		if(p->children != NULL)
		{
			node* val = p->children;	
			node* op2 = p->children->children;
			node* factor = val->next;
			node* n5 = factor->next;

			//printf("inside n5\n");

			node* out_n5 = buildAST(n5);
			factor->next = NULL;											//NOT SURE *******************************

			op2->children = buildAST(factor);
			op2->children->parent = op2;
			op2->next = out_n5;
			//printf(" after n5 inside n5\n");

			free(p->children);
			p->children = NULL;

			free(p);
			p = NULL;

			return op2;
		}
		else
		{
			//printf(" n5 ka null\n");

			free(p);
			p = NULL;

			return NULL;
		}	
	}	
	else if(strcmp(p->buf_name, "ret") == 0)
	{
		if(p->children != NULL)							//<ret> -> <RETURNS><SQBO><output_plist><SQBC><SEMICOL>
		{
			node* temp = p->children->next->next;
			free(p->children->next->next->next->next);
			p->children->next->next->next->next = NULL;

			free(p->children->next->next->next);
			p->children->next->next->next = NULL;

			free(p->children->next);
			p->children->next = NULL;

			free(p->children);
			p->children = NULL;

			free(p);
			p = NULL;

			return buildAST(temp);
		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}	
	}
	else if(strcmp(p->buf_name, "input_plist") == 0)
	{
		//<input_plist> -> <ID><COLON><dataType><n1>
		//printf("inside input_plist\n");

		node* ID = p->children;					//ID
		node* datatype = p->children->next->next;		//datatype
		node* n1 = p->children->next->next->next;//n1

		free(p->children->next);					//COLON
		p->children->next = NULL;

		ID->children = buildAST(datatype);			//ID->datatype (child)
		//printf("adter datatype\n");
		ID->children->parent = ID;
		//printf("datatype parent\n");
		ID->next = buildAST(n1);				//ID->ID(n1)  (next)

		node* set = ID;

		while(set != NULL)
		{
			set->parent = p;
			set = set->next;
		}	

		//printf("adter n1\n");

		return p;
	}
	else if(strcmp(p->buf_name, "output_plist") == 0)		//<output_plist> -> <ID><COLON><type><n2>
	{
		node* ID = p->children;					//ID
		node* type = p->children->next->next;		//type
		node* n2 = p->children->next->next->next;//n2

		free(p->children->next);					//COLON
		p->children->next = NULL;

		ID->children = type;			//ID->type (child)
		ID->children->parent = ID;

		if(n2->children != NULL)
		{
			node* out = buildAST(n2);
			type->next = NULL;
			ID->next = NULL;
			ID->next = out;

			node* set = ID;

			while(set != NULL)
			{
				set->parent = p;
				set->children->next = NULL;
				set = set->next;
			}
		}
		else
		{
			free(n2);
			n2 = NULL;

			ID->next = NULL;
			type->next = NULL;
		}			

		return p;
	}
	else if(strcmp(p->buf_name, "dataType") == 0)
	{
		node* temp = p->children;					//ARRAY

		//<dataType> -> <INTEGER>
		//<dataType> -> <REAL>
		//<dataType> -> <BOOLEAN>
		if(strcmp(temp->buf_name, "ARRAY") != 0)
		{
			//printf("inside data clause 1\n");
			free(p);
			p =NULL;

			//printf("done with datatype\n");
			return temp;							//Other than ARRAY
		}
		else
		{
			node* temp1 = p->children->next->next; 	//range
			node* temp2 = temp1->next->next->next;  //type
			node* arr = p->children;

			//printf("inside arr part of dataType\n");

			node* type = temp2;
			//printf("after type inside datatype\n");
			type->next = buildAST(temp1);
			//printf("after range inside datatype\n");

			arr->children = type;
			arr->children->parent = arr;
			arr->children->next->parent = arr;

			free(temp1->next->next);				//<dataType> -> <ARRAY><SQBO><range><SQBC><OF><type>
			temp1->next->next = NULL;

			free(temp1->next);
			temp1->next = NULL;

			free(p->children->next);
			p->children->next = NULL;

			free(p);
			p = NULL;

			return arr;			//type
		}		
	}
	else if(strcmp(p->buf_name, "n1") == 0)
	{
		//<n1> -> <COMMA><ID><COLON><dataType><n1>
		//<n1> -> <EPSILON>
		if(p->children != NULL)
		{
			node* temp = p->children->next;		//ID
			node* temp1 = temp->next->next;		//dataType
			node* temp2 = temp1->next;			//n1

			free(temp->next);
			temp->next = NULL;

			free(p->children);
			p->children = NULL;

			temp->children = buildAST(temp1);
			temp->children->parent = temp;
			//printf("n1 datatype\n");

			temp->next = buildAST(temp2);

			//printf("n1 again\n");

			free(p);
			p = NULL;

			return temp;

		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}		
	}
	else if(strcmp(p->buf_name, "n2") == 0)
	{
		//<n2> -> <COMMA><ID><COLON><type><n2>
		//<n2> -> <EPSILON>
		if(p->children != NULL)
		{
			node* ID = p->children->next;		//ID
			node* type = ID->next->next;		//type
			node* n2 = type->next;			//n2

			free(ID->next);
			ID->next = NULL;

			free(p->children);
			p->children = NULL;

			ID->children = type;
			ID->children->parent = ID;
			//printf("n2 type\n");

			node* ID1 = buildAST(n2);
			ID->next = NULL;
			ID->next = ID1;

			//printf("n2 again\n");

			free(p);
			p = NULL;

			return ID;

		}
		else
		{
			free(p);
			p = NULL;

			return NULL;
		}		
	}
	else if(strcmp(p->buf_name, "range") == 0)
	{
		//<range> -> <NUM><RANGEOP><NUM>
		node* temp1 = p->children;				//NUM
		node* temp2 = p->children->next->next;	//NUM

		free(p->children->next);
		p->children->next = NULL;

		temp1->children = temp2;				//NUM->NUM
		temp2->parent = temp1;

		free(p);
		p = NULL;

		return temp1;
	}	
	else if(strcmp(p->buf_name, "driverModule") == 0)
	{
		//<driverModule> -> <DRIVERDEF><DRIVER><PROGRAM><DRIVERENDDEF><moduleDef>
		node* moduleDef = p->children->next->next->next->next;//moduleDef
		free(p->children->next->next->next);//DRIVERENDDEF
		p->children->next->next->next = NULL;

		free(p->children->next->next);//PROGRAM
		p->children->next->next = NULL;

		free(p->children->next);//DRIVER
		p->children->next = NULL;

		free(p->children);//DRIVERDEF
		p->children = NULL;

		p->children = buildAST(moduleDef);

		return p;
	}
	else
	{
		printf("Error in AST gen %s  %s\n", p->buf_name, p->parent->buf_name);
	}	

}

