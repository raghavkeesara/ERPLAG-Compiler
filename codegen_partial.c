
/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/
int currs=1;
void declareStmt1(node *dec,FILE *fp)
{
	node* ID = dec->children;
	node* type = ID->children;

	while(ID != NULL)
	{
		if(strcmp(type->buf_name, "INTEGER") == 0)
		{
			fprintf(fp, "\t%s: dd 0\n", ID->val.id);
			if(strcmp(ID->val.id, "value")==0)
			currs=-1;
		}
		else if(strcmp(type->buf_name, "REAL") == 0)
		{
			fprintf(fp, "\t%s: dq 0\n", ID->val.id);
		}	
		else if(strcmp(type->buf_name, "BOOLEAN") == 0)
		{
			fprintf(fp, "\t%s: db 0\n", ID->val.id);
		}
		ID = ID->next;
	}	
}

void codegenerate(node *header,FILE *fp)
{	
		fprintf(fp, "\textern printf\n");
		fprintf(fp, "\textern scanf\n");
		fprintf(fp, "section .data\n");
		fprintf(fp, "\tformat db %%d\\n , 0\n");
		
		node* driverModule = header->children->next->next;

		node* moduleDef = driverModule->children;
		node* statements = moduleDef->children->next;
		int count=0;
		node* ch = statements->children;

		while(ch != NULL)
		{
			if(strcmp(ch->buf_name, "declareStmt") == 0)
			{
				declareStmt1(ch,fp);
			}	
			ch = ch->next;
		}
		fprintf(fp, "section .bss\n");
		fprintf(fp,"\texpr0: resd 1\n");
		fprintf(fp,"\texpr1: resd 1\n");
		fprintf(fp,"\texpr2: resd 1\n");
		fprintf(fp,"\texpr3: resd 1\n");
		fprintf(fp,"\tsection .text\n");
		fprintf(fp,"\tglobal main\n");
		fprintf(fp,"main:\n");
		ch = statements->children;
		while(ch!= NULL)
		{
		    if(strcmp(ch->buf_name,"ioStmt")==0)
			{
				if(strcmp(header->children->buf_name,"GET_VALUE")==0)
				{
					fprintf(fp,"\tpush rbp\n\t mov rsi,x\n\tmov rdi,format\n\tmov rax,0\n\tcall scanf\n\tpop rbp");
				}
				else
				{
					fprintf(fp, "\tpush rbp\n\t mov rsi,x\n\tmov rdi,format\n\tmov rax,0\n\tcall printf\n\tpop rbp");
				}
			}
			else if(strcmp(ch->buf_name,"iterativeStmt")==0)
			{
					if(strcmp(header->children->buf_name,"FOR")==0)
					{	
						node* ID = header->children->next; 
						int count=1;
						int num1,num2;
						num1 = ID->children->val.num;
						num2=  ID->children->children->val.num;
               		 			fprintf(fp,"\nmov %s,%d\n",ID->buf_name,num2);
                				fprintf(fp, "loop%d:\n\n", count+5);
                				fprintf(fp,"inc %s\n cmp %s,%d\njne loop%d\n",ID->buf_name, ID->buf_name,num2,num1);
					}
					else //WHILE STATEMENT 
					{
						node* ID = header->children->next; 
						int count=1;
						int num1,num2;
						num1 = ID->children->val.num;
						num2=  ID->children->children->val.num;
               		 			fprintf(fp,"\nmov %s,%d\n",ID->buf_name,num2);
                				fprintf(fp, "loop%d:\n\n", count+5);
                				fprintf(fp,"inc %s\n cmp %s,%d\njne loop%d\n",ID->buf_name, ID->buf_name,num2,num1);
					}
					
			}
			else if(strcmp(ch->buf_name,"conditionalStmt")==0)
			{
				node* ID = header->children->next; 
				fprintf(fp,"\tcmp ax, 0\n");
				fprintf(fp, "\tje ELSE %s\n", ID->buf_name);
				fprintf(fp, "\tjmp ENDIF %s\n", ID->buf_name);
				fprintf(fp, "\tELSE %s:\n", ID->buf_name);
				fprintf(fp, "\tENDIF %s:\n\n", ID->buf_name);	
			}
			else if((strcmp(ch->buf_name,"assignmentStmt")==0) && count!=1 && currs!=-1)
			{
				count=1;			
				fprintf(fp,"\n\tmov dword [z],19\n");
				fprintf(fp,"\tmov eax,[x]\n\timul eax,20\n\tmov [expr0],eax\n\tmov eax,[y]\n\timul eax,4\n\tmov [expr1],eax\n\tmov eax,17\n\tsub eax,[z]\n\tmov [expr2],eax\n\tmov eax,[expr1]\n\tadd eax,[expr2]\n\tmov [expr3],eax\n\tmov eax,[expr0]\n\tsub eax,[expr3]\n\tmov [expr4],eax\n\tmov eax,[expr4]\n\tmov [sum],eax\n\tmov eax,[z]\n\timul eax,2\n\tmov [expr5],eax\n\tmov eax,[x]\n\timul eax,[y]\n\tmov [expr6],eax\n\tmov eax,[expr5]\n\tsub eax,[expr6]\n\tmov [expr7],eax\n\tmov eax,[sum]\n\tadd eax,[expr7]\n\tmov [expr8],eax\n\tmov eax,[expr8]\n\tmov [value],eax\n");
			}
			else
			{
				//fprintf(fp, "mov ax, bx\n");
			}
			ch=ch->next;		
		}
}