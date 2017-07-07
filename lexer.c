/*
Name : Pranesh Anubhav and Sai Raghav Reddy
ID : 2013B5A7417P and 2013B4A7097P
Batch 16
*/
//Lexical Analysis
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lexer.h"

int isDigit(char ch)
{
	if( ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

int isLowerChar(char ch)
{
	if( ch >= 'a' && ch <= 'z')
		return 1;
	else
		return 0;
}

int isUpperChar(char ch)
{
	if( ch >= 'A' && ch <= 'Z')
		return 1;
	else
		return 0;
}

int isAlpha(char ch)
{
	if( ch >= 'A' && ch <= 'Z')
		return 1;
	if( ch >= 'a' && ch <= 'z')
		return 1;
	else
		return 0;
}

int isWhitespace(char ch)
{
	if( ch == ' ' || ch == '\t')
		return 1;
	else
		return 0;
}

int ignoreComment(FILE *fp)
{
	char t;
	while( (t = fgetc(fp)) != '*') {}
	if(fgetc(fp) != '*')
		ignoreComment(fp);
}

void removeComments(char *testcaseFile, char *cleanFile,int check)
{
	FILE *fw;
	FILE *fr;
	fr=fopen(testcaseFile,"r");
	fw=fopen(cleanFile,"w");
	char c;
	char n;

	while ((c = fgetc(fr)) != EOF)
	{
		if( c == '*')
		{
			if((n = fgetc(fr)) == '*')
			{
				ignoreComment(fr);
			}
			else
			{
				fprintf(fw, "%c", c);
				if(check == 0)
					printf("%c",c );
				fprintf(fw, "%c", n);
				if(check == 0)
					printf("%c",n );
			}
		}
		else
		{
			fprintf(fw, "%c", c);
			if(check == 0)
				printf("%c",c );
		}
	}

}

int lineNo = 1;
static num_no = 1;
static rnum_no = 1;
static id_no = 1;
tokeninfo first;
tokeninfo last;
tokeninfo curr;

char name[30][15] = {"integer","real","boolean","of","array","start","end","declare","module","driver","program","get_value","print","use","with","parameters","true","false","takes","input","returns","AND","OR","for","in","switch","case","break","default","while"};
char token[30][15] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE"};

void charMatcher(FILE *fp)
{
	char c;
	FILE *fwr;
	fwr = fopen("lexicalAnlaysisOutput.txt","w");
	
	while((c = fgetc(fp)) != EOF)
	{
		if( c == '*')
		{
			if(fgetc(fp) == '*')
			{
				ignoreComment(fp);
				fprintf(fwr,"COMMENTMARK ");
				addtoList(51);
			}
			else
			{
				fprintf(fwr,"MUL ");
				addtoList(32);
				fseek(fp, -1, SEEK_CUR);
			}
		}
		else if( c == '\n')
		{
			lineNo++;
		}	
		
		else if( c == '+')
		{
			fprintf(fwr,"PLUS ");
			addtoList(30);
		}
		
		else if( c == '-')
		{
			fprintf(fwr,"MINUS ");
			addtoList(31);
		}
		
		else if( c == '/')
		{
			fprintf(fwr,"DIV ");
			addtoList(33);
		}
		
		else if( c == ';')
		{
			fprintf(fwr,"SEMICOL ");
			addtoList(44);
		}
		
		else if( c == ',')
		{
			fprintf(fwr,"COMMA ");
			addtoList(45);
		}
		
		else if( c == '(')
		{
			fprintf(fwr,"BO ");
			addtoList(49);
		}
		
		else if( c == ')')
		{
			fprintf(fwr,"BC ");
			addtoList(50);
		}
		
		else if( c == '[')
		{
			fprintf(fwr,"SQBO ");
			addtoList(47);
		}
		
		else if( c == ']')
		{
			fprintf(fwr,"SQBC ");
			addtoList(48);
		}
		
		else if( c == '!')
		{
			char t = fgetc(fp);
			if( t == '=')
			{
				fprintf(fwr,"NE ");
				addtoList(39);
			}	
			else
				printf("Invalid symbol at line %d ", lineNo);
		}
		
		else if( c == '<')
		{
			char t = fgetc(fp);
			if( t == '<')
			{
				char h = fgetc(fp);
				if( h == '<')
				{
					fprintf(fwr,"DRIVERDEF ");
					addtoList(52);
				}
				else
				{
					fprintf(fwr,"DEF ");
					addtoList(40);
					fseek(fp, -1, SEEK_CUR);
				}
			}	
			else if ( t == '=')
			{	
				fprintf(fwr,"LE ");
				addtoList(35);
			}	
			else
			{
				fprintf(fwr,"LT ");
				addtoList(34);
				fseek(fp, -1, SEEK_CUR);
			}
		}
		
		else if( c == '>')
		{
			char t = fgetc(fp);
			if( t == '>')
			{
				char h = fgetc(fp);
				if( h == '>')
				{
					fprintf(fwr,"DRIVERENDDEF ");
					addtoList(53);
				}
				else
				{
					fprintf(fwr,"ENDDEF ");
					addtoList(41);
					fseek(fp, -1, SEEK_CUR);
				}
			}	
			else if ( t == '=')
			{
				fprintf(fwr,"GE ");
				addtoList(36);
			}	
			else
			{
				fprintf(fwr,"GT ");
				fseek(fp, -1, SEEK_CUR);
				addtoList(37);
			}
		}
		
		else if( c == ':')
		{
			char t = fgetc(fp);
			if( t == '=')
			{
				fprintf(fwr,"ASSIGNOP ");
				addtoList(46);
			}
			else
			{
				fprintf(fwr,"COLON ");
				addtoList(42);
				fseek(fp, -1, SEEK_CUR);
			}
		}
		
		else if( c == '=')
		{
			char t = fgetc(fp);
			if( t == '=')
			{
				fprintf(fwr,"EQ ");
				addtoList(38);
			}	
			else
			{
				printf("ERROR_3: Unknown pattern '=' at %d", lineNo);
				fseek(fp, -1, SEEK_CUR);
			}
		}
		
		else if( c == '.')
		{
			char t = fgetc(fp);
			if( t == '.')
			{
				fprintf(fwr,"RANGEOP ");
				addtoList(43);
			}
			else
			{
				printf("ERROR_3: Unknown pattern '.' at %d", lineNo);
				fseek(fp, -1, SEEK_CUR);	
			}	
		
		}
		
		else if(isAlpha(c))
		{
			char value[15]={"\0"};
			int i=0;
			int pr =0;
			value[i++] = c;
			int length = 1;
			char t ;
			while(t = fgetc(fp))
			{
				if(isAlpha(t) || isDigit(t) || t == '_')  
				{
					value[i++] = t;
					length++;
				}
				else
				{
					int j = 0;					
					for(j=0;j<30;j++)
					{
						if(strcmp(value,name[j])==0)
							{
								fprintf(fwr,"%s ", token[j]);
								addtoList(j);
								pr = 1;
								break;
							}
					}
					if(pr == 0)
					{
						if(length <= 8)
						{
							fprintf(fwr,"ID ");
							//printf("id: %s\n", value);
							addIDtoList(value);
						}	
						else
						{
							printf("\nERROR_1 : Identifier at line %d is longer than the prescribed length .", lineNo);
							fprintf(fwr, "ERROR_1");
						}	
					}	
					fseek(fp, -1, SEEK_CUR);
					break;
				}
			}
			
		}
		else if( isWhitespace(c) )
		{
			char m ;
			while(m = fgetc(fp))
			{
				if(isWhitespace(m)==0)
					break;
			}
			fseek(fp, -1, SEEK_CUR);
		}
		else if( isDigit(c) )
		{
			char value[20]={"\0"};
			int i=0;
			value[i++] = c;
			int length =1;
			char n;
			while(n = fgetc(fp))
			{
				if( isDigit(n))
				{
					length++;
					value[i++] = n;
				}
				else if (n == '.')
				{
					char m;
					m = fgetc(fp);
					int br = 0;
					if(m == '.')
					{
						fprintf(fwr,"NUM%d ", num_no);
						addNUMtoList(atoi(value),num_no);
						//printf("%d", atoi(value));//1
						num_no++;
						fseek(fp, -2, SEEK_CUR);
						break;
					}
					else
					{
						length++;
						value[i++] = '.';
						value[i++] = m;
						char expnent[20]={"\0"};
						int j=0;

						while(m = fgetc(fp))
						{
							int br1 =0;
							if(isDigit(m))
							{	
								length++;
								value[i++] = m;
							}	
							else if ( m == 'e' || m == 'E')
							{
								//value[i++] = m;
								char t;
								t = fgetc(fp);
								
								if( t == '+' || t == '-' )
								{
									//value[i++] = t;
									char sign_check = t;
									while(t = fgetc(fp))
									{
										if(isDigit(t))
										{
											//length++;
											expnent[j++] = t;
										}
										else
										{
											fprintf(fwr,"RNUM%d ", rnum_no);
											int out = power(atoi(expnent));
											float inp;
											if(sign_check == '+')
											 	inp = atof(value)*out;
											else
												inp = atof(value)/out;

											addRNUMtoList(inp,rnum_no);
											//printf(" %f",inp);
											rnum_no++;
											fseek(fp, -1, SEEK_CUR);
											br = 1;
											br1 = 1;
											break;
										}
									}
								}
								else if (isDigit(t))
								{
									//length++;
									//value[i++] = t;
									expnent[j++] = t;
									while(t = fgetc(fp))
									{
										if(isDigit(t))
										{
											//length++;
											expnent[j++] = t;
										}
										else
										{
											fprintf(fwr,"RNUM%d ", rnum_no);
											int out = power(atoi(expnent));
											float inp = atof(value)*out;
											addRNUMtoList(inp,rnum_no);
											rnum_no++;
											//printf(" %f",inp);
											fseek(fp, -1, SEEK_CUR);
											br = 1;
											br1 = 1;
											break;
										}
									}
								}
								
								else
								{
									printf("ERROR4: Invalid symbol after E/e at line no %d ", lineNo);
									fprintf(fwr, "ERROR4 ");
									break;
								}
								
							if( br1 ==1)	
								break;
							}
							else
							{
								fprintf(fwr,"RNUM%d ", rnum_no);
								addRNUMtoList(atof(value),rnum_no);
								rnum_no++;
								//printf(" %f",atof(value));
								fseek(fp, -1, SEEK_CUR);
								br = 1;
								break;
							}
						}
					if( br ==1)
						break;
					}
				
				}
				else
				{
					fprintf(fwr,"NUM%d ", num_no);
					addNUMtoList(atoi(value),num_no);
					num_no++;
					//printf(" %d", atoi(value));
					fseek(fp, -1, SEEK_CUR);
					break;
				}	
			
			}
		
		}
			
	}

}

int power(int n)
{
	int i = 0;
	int out = 1;
	if(n==0)
		return out;
	else
	{
		for(i=0;i<n;i++)
			out = out * 10;
		return out;
	}
	
}

void addtoList(int caseno)
{
	tokeninfo new = (tokeninfo) malloc (sizeof(struct Token));
    
	switch(caseno)
	{
		case 0: new->tokentype = INTEGER;
				strcpy(new->value.id, "integer");
			break;
		case 1: new->tokentype = REAL;
				strcpy(new->value.id, "real");
			break;
		case 2: new->tokentype = BOOLEAN;
				strcpy(new->value.id, "boolean");
			break;
		case 3: new->tokentype = OF;
				strcpy(new->value.id, "of");
			break;
		case 4: new->tokentype = ARRAY;
				strcpy(new->value.id, "array");
			break;
		case 5: new->tokentype = START;
				strcpy(new->value.id, "start");
			break;
		case 6: new->tokentype = END;
				strcpy(new->value.id, "end");
			break;
		case 7: new->tokentype = DECLARE;
				strcpy(new->value.id, "declare");
			break;
		case 8: new->tokentype = MODULE;
				strcpy(new->value.id, "module");
			break;
		case 9: new->tokentype = DRIVER;
				strcpy(new->value.id, "driver");
			break;
		case 10:new->tokentype = PROGRAM;
				strcpy(new->value.id, "program");
			break;
		case 11:new->tokentype = GET_VALUE;
				strcpy(new->value.id, "get_value");
			break;
		case 12:new->tokentype = PRINT;
				strcpy(new->value.id, "print");
			break;
		case 13:new->tokentype = USE;
				strcpy(new->value.id, "use");
			break;	
		case 14:new->tokentype = WITH;
				strcpy(new->value.id, "with");
			break;
		case 15:new->tokentype = PARAMETERS;
				strcpy(new->value.id, "parameters");
			break;
		case 16:new->tokentype = TRUEs;
				strcpy(new->value.id, "true");
			break;
		case 17:new->tokentype = FALSEs;
				strcpy(new->value.id, "false");
			break;
		case 18:new->tokentype = TAKES;
				strcpy(new->value.id, "takes");
			break;
		case 19:new->tokentype = INPUT;
				strcpy(new->value.id, "input");
			break;
		case 20:new->tokentype = RETURNS;
				strcpy(new->value.id, "returns");
			break;
		case 21:new->tokentype = AND;
				strcpy(new->value.id, "and");
			break;
		case 22:new->tokentype = OR;
				strcpy(new->value.id, "or");
			break;
		case 23:new->tokentype = FOR;
				strcpy(new->value.id, "for");
			break;
		case 24:new->tokentype = IN;
				strcpy(new->value.id, "in");
			break;
		case 25:new->tokentype = SWITCH;
				strcpy(new->value.id, "switch");
			break;
		case 26:new->tokentype = CASE;
				strcpy(new->value.id, "case");
			break;
		case 27:new->tokentype = BREAK;
				strcpy(new->value.id, "break");
			break;
		case 28:new->tokentype = DEFAULT;
				strcpy(new->value.id, "default");
			break;
		case 29:new->tokentype = WHILE;
				strcpy(new->value.id, "while");
			break;
		case 30:new->tokentype = PLUS;
				strcpy(new->value.id, "+");
			break;
		case 31:new->tokentype = MINUS;
				strcpy(new->value.id, "-");
			break;
		case 32:new->tokentype = MUL;
				strcpy(new->value.id, "*");
			break;
		case 33:new->tokentype = DIV;
				strcpy(new->value.id, "/");
			break;
		case 34:new->tokentype = LT;
				strcpy(new->value.id, "<");
			break;
		case 35:new->tokentype = LE;
				strcpy(new->value.id, "<=");
			break;
		case 36:new->tokentype = GE;
				strcpy(new->value.id, ">=");
			break;
		case 37:new->tokentype = GT;
				strcpy(new->value.id, ">");
			break;
		case 38:new->tokentype = EQ;
				strcpy(new->value.id, "==");
			break;
		case 39:new->tokentype = NE;
				strcpy(new->value.id, "!=");
			break;
		case 40:new->tokentype = DEF;
				strcpy(new->value.id, "<<");	
			break;
		case 41:new->tokentype = ENDDEF;
				strcpy(new->value.id, ">>");
			break;
		case 42:new->tokentype = COLON;
				strcpy(new->value.id, ":");
			break; 
		case 43:new->tokentype = RANGEOP;
				strcpy(new->value.id, "..");
			break;
		case 44:new->tokentype = SEMICOL;
				strcpy(new->value.id, ";");
			break;
		case 45:new->tokentype = COMMA;
				strcpy(new->value.id, ",");
			break;
		case 46:new->tokentype = ASSIGNOP;
				strcpy(new->value.id, ":=");
			break;	
		case 47:new->tokentype = SQBO;
				strcpy(new->value.id, "[");
			break;
		case 48:new->tokentype = SQBC;
				strcpy(new->value.id, "]");
			break;
		case 49:new->tokentype = BO;
				strcpy(new->value.id, "(");
			break;
		case 50:new->tokentype = BC;
				strcpy(new->value.id, ")");	
			break;
		case 51:new->tokentype = COMMENTMARK;
				strcpy(new->value.id, "**");
			break;	
		case 52:new->tokentype = DRIVERDEF;
				strcpy(new->value.id, "<<<");
			break;
		case 53:new->tokentype = DRIVERENDDEF;	
				strcpy(new->value.id, ">>>");
			break;		
		default: {
			 FILE *er;
			 er=fopen("Lexical_error_Report.txt","a");
			 fprintf(er,"\nSymbol number %d case not present in addtoList",caseno);
			 fclose(er);
			 }
	}

	new->lineNo = lineNo;
	new->next = NULL;
	new->prev = last;
	last->next = new;
	last = last->next;
}

tokeninfo getNextToken()
{
	if(curr==NULL)
	{
		curr = first;
		return curr;
	}	
	else
	{
		curr = curr->next;
		return curr;
	}
		
}

void addNUMtoList(int val, int count)
{
	tokeninfo new = (tokeninfo) malloc (sizeof(struct Token));
	new->lineNo = lineNo;
	new->value.num = val;
	new->cnt = count;
	new->tokentype = NUM;
	new->next = NULL;
	new->prev = last;
	last->next = new;
	last = last->next;
}

void addRNUMtoList(float val, int count)
{
	tokeninfo new = (tokeninfo) malloc (sizeof(struct Token));
	new->lineNo = lineNo;
	new->value.rnum = val;
	new->cnt = count;
	new->tokentype = RNUM;
	new->next = NULL;
	new->prev = last;
	last->next = new;
	last = last->next;
}

void addIDtoList(char *val)
{
	tokeninfo new = (tokeninfo) malloc (sizeof(struct Token));
	new->lineNo = lineNo;
	strcpy(new->value.id,val);
	new->tokentype = ID;
	new->next = NULL;
	new->prev = last;
	last->next = new;
	last = last->next;
}

void lexicalOutput(char *filename)
{
	first = (tokeninfo) malloc (sizeof(struct Token));
	last = (tokeninfo) malloc (sizeof(struct Token));
	first->cnt = -9;
	last = first;

	FILE *fp;
	fp =  fopen(filename,"r");
	if(fp == NULL)
	{
		printf("Can't open the source file.\n");
		exit(1);
	}
	else
	{
		printf("Source file opened.\n");
	}
	charMatcher(fp);
	curr = first;

	tokeninfo dollar = (tokeninfo) malloc (sizeof(struct Token));
	dollar->tokentype = $;
	char inp[40] = {"\0"};
	strcpy(dollar->value.id,inp);
	strcpy(inp,"$");
	strcpy(dollar->value.id,inp);
	dollar->next = NULL;
	dollar->prev = last;
	last->next = dollar;
	last = dollar;

	fclose(fp);

}


	
