%{
#include<bits/stdc++.h>
using namespace std;
int yylex();
int yyerror(char *);
extern int yylineno;
extern FILE* yyin;
extern char* yytext;
%}
%union{
  int ivalue;
  char cvalue;
};

%token IDENTIFIER INTCONSTANT THIS True False Null CHARCONST IF ELSE WHILE RETURN INT STATIC CLASS VOID
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
start: programstructure {cout<<"completed syntax analysis"<<endl;}; 

programstructure: CLASS IDENTIFIER '{' classdeclarations '}';

classdeclarations: classdeclarations classobjvardec
				|  classdeclarations classobjrotinedec
				|
				;

classobjvardec: STATIC vardec
			  | vardec
			  ;

classobjrotinedec: STATIC rotinedec
				|  rotinedec
				| constructordec
				;

constructordec: IDENTIFIER '(' parameterslist ')' '{' statements '}';
rotinedec: type IDENTIFIER '(' parameterslist ')' '{' statements '}';

parameterslist: parameterslist ',' type IDENTIFIER
			  | type IDENTIFIER
			  |
			  ;
 
statements: statements statement
		|
		;

statement: ifstatement
		|  whilestatement
		|  returnstatement
		|  subrotinecall ';'
		|  assignmentstatement
		|  vardec
		;
vardec: type varlist ';' ;
varlist: varlist ',' lhs
	   | lhs
	   ;
type: INT | VOID;
ifstatement: IF '(' expr ')' '{' statements '}'
		   | IF '(' expr ')' '{' statements '}' ELSE '{' statements '}'
		   ;
whilestatement: WHILE '(' expr ')' '{' statements '}';
returnstatement: RETURN expr ';';
assignmentstatement: lhs '=' expr ';';
lhs: IDENTIFIER
   | lhs '[' expr ']'
   ;
expr: expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr OR expr
	| expr AND expr
	| expr '<' expr
	| expr '>' expr
	| expr EQ expr
	| '-' expr %prec UMINUS
	| '!' expr %prec UMINUS
	| '(' expr ')'
	| term;
term: IDENTIFIER
	| IDENTIFIER '[' expr ']'
	| subrotinecall
	| INTCONSTANT
	| CHARCONST
	| keywordconstant;
subrotinecall: IDENTIFIER '(' expr_list ')' 
			|  IDENTIFIER '.' IDENTIFIER '(' expr_list ')';
expr_list: expr_list ',' expr
		|  expr
		|
		;
keywordconstant: THIS | True | False | Null;

%%
int main(int argc , char** argv){
    yyin = fopen(argv[1],"r");
    yyparse();
}
int yyerror(char *s){
  printf("\n\nError: %s\nnot accepted\nline no:%d\n\n", s,yylineno);
  printf("%s\n\n",yytext);
}