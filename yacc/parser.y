%{
#include<bits/stdc++.h>
#include "./codegen/ast.h"
using namespace std;
int yylex();
int yyerror(char *);
extern int yylineno;
extern FILE* yyin;
extern char* yytext;
%}
%union{
	nodeType *nptr;
  	int ivalue;
  	char name[32];
};
%token ASS CLASSDEC STATIC_VARDEC VARDEC STATIC_ROTINE ROTINE CONSTRUCTOR
%token PARAMLIST STATEMENTS VARLIST OBJDEC ARRAY FUNCTIONCALL METHODCALL EXPRLIST
%token THIS True False Null IF ELSE WHILE RETURN INT STATIC CLASS VOID CHAR BOOL NEW
%token <ivalue> INTCONSTANT
%token <name> IDENTIFIER
%token <ivalue> CHARCONST
%type <nptr> start programstructure classdeclarations classobjvardec classobjrotinedec 
%type <nptr> constructordec rotinedec parameterslist 
%type <nptr> statements statement vardec varlist ifstatement whilestatement returnstatement assignmentstatement 
%type <nptr> lhs term subrotinecall keywordconstant type expr expr_list
%left ','
%right '='
%left AND OR
%left EQ
%left '<' '>'
%left '+' '-' 
%left '*' '/'
%left '[' ']' '(' ')' '.' 
%right '!'
%nonassoc UMINUS

%%
start: programstructure {$$=$1;compile($1);cout<<"completed syntax analysis"<<endl;}
		; 

programstructure: CLASS IDENTIFIER '{' classdeclarations '}' {$$ = opr(CLASS, 2, id($2), $4);}
				;

classdeclarations: classdeclarations classobjvardec    {$$ = opr(CLASSDEC, 2, $1, $2);}
				|  classdeclarations classobjrotinedec {$$ = opr(CLASSDEC, 2, $1, $2);}
				|  {$$ = NULL;}
				;

classobjvardec: STATIC vardec {$$ = opr(STATIC_VARDEC, 1, $2);}
			  | vardec        {$$ = $1;}
			  ;

classobjrotinedec: STATIC rotinedec {$$ = opr(STATIC_ROTINE, 1, $2);}
				|  rotinedec  {$$ = $1;}
				| constructordec {$$ = $1;}
				;

constructordec: IDENTIFIER '(' parameterslist ')' '{' statements '}'  {$$ = opr(CONSTRUCTOR, 3, id($1), $3, $6);}
				;
rotinedec: type IDENTIFIER '(' parameterslist ')' '{' statements '}'  {$$ = opr(ROTINE, 4, $1, id($2), $4, $7);}
			;

parameterslist: parameterslist ',' type IDENTIFIER  {$$=opr(PARAMLIST, 3, $1, $3, id($4));}
			  | type IDENTIFIER     {$$ = opr(PARAMLIST, 3, NULL, $1, id($2));}
			  |    {$$=NULL;}
			  ;
 
statements: statements statement {$$ = opr(STATEMENTS, 2, $1, $2);}
		|    {$$ = NULL;}
		;

statement: ifstatement			{$$=$1;}
		|  whilestatement		{$$=$1;}
		|  returnstatement		{$$=$1;}
		|  subrotinecall ';'	{$$=$1;}
		|  assignmentstatement	{$$=$1;}
		|  vardec				{$$=$1;}
		;
vardec: type varlist ';' {$$ = opr(VARDEC, 2, $1, $2);}
		;
varlist: varlist ',' lhs {$$ = opr(VARLIST, 2, $1, $3);}
	   | lhs			 {$$ = $1;}
	   ;
type: INT   {$$=opr(INT,0);  } 
	| CHAR  {$$=opr(CHAR,0); }
	| BOOL  {$$=opr(BOOL,0); }
	| VOID  {$$=opr(VOID,0); }
	;
ifstatement: IF '(' expr ')' '{' statements '}' {$$=opr(IF, 3, $3, $6, NULL);}
		   | IF '(' expr ')' '{' statements '}' ELSE '{' statements '}' {$$=opr(IF, 3, $3, $6, $10);}
		   ;
whilestatement: WHILE '(' expr ')' '{' statements '}' {$$=opr(WHILE, 2, $3, $6);};
returnstatement: RETURN expr ';' {$$ = opr(RETURN,1,$2);}
				| RETURN ';' {$$ = opr(RETURN,0);}
				;
assignmentstatement: lhs '=' expr ';' {$$ = opr(ASS, 2, $1, $3);}
				   | lhs '=' NEW subrotinecall {$$ = opr(OBJDEC, 2, $1, $4);}
				   ;
lhs: IDENTIFIER    {$$ = id($1);}
   | IDENTIFIER '[' expr ']'  {$$ = opr(ARRAY, 2, id($1), $3);}
   ;
expr: expr '+' expr	{ $$ = opr('+', 2, $1, $3); }
	| expr '-' expr	{ $$ = opr('-', 2, $1, $3); }
	| expr '*' expr	{ $$ = opr('*', 2, $1, $3); }
	| expr '/' expr { $$ = opr('/', 2, $1, $3); }
	| expr OR expr  { $$ = opr(OR, 2, $1, $3); }
	| expr AND expr { $$ = opr(AND, 2, $1, $3); }
	| expr '<' expr { $$ = opr('<', 2, $1, $3); }
	| expr '>' expr { $$ = opr('>', 2, $1, $3); }
	| expr EQ expr  { $$ = opr(EQ, 2, $1, $3); }
	| '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
	| '!' expr %prec UMINUS { $$ = opr('!', 1, $2); }
	| '(' expr ')' { $$ = $2; }
	| term { $$ = $1;}
	;
term: IDENTIFIER    { $$ = id($1); }
	| IDENTIFIER '[' expr ']'  { $$ = opr(ARRAY, 2, id($1), $3); }
	| subrotinecall { $$ = $1; }
	| INTCONSTANT    { $$ = constint($1); }
	| CHARCONST      { $$ = constchar($1); }
	| keywordconstant { $$ = $1; }
	;
subrotinecall: IDENTIFIER '(' expr_list ')'  { $$ = opr(FUNCTIONCALL, 2, id($1), $3); }
			|  IDENTIFIER '.' IDENTIFIER '(' expr_list ')' { $$ = opr(METHODCALL, 3, id($1), id($3), $5); }
			;
expr_list: expr_list ',' expr { $$ = opr(EXPRLIST, 2, $1, $3); }
		|  expr  { $$ = $1; }
		|  { $$ = NULL; }
		;
keywordconstant: THIS { $$ = opr(THIS,0); }
				| True { $$ = opr(True,0); }
				| False { $$ = opr(False,0); }
				| Null { $$ = opr(Null,0); }
				;

%%
int main(int argc , char** argv){
    yyin = fopen(argv[1],"r");
    yyparse();
}
int yyerror(char *s){
  printf("\n\nError: %s\nnot accepted\nline no:%d\n\n", s,yylineno);
  printf("%s\n\n",yytext);
}