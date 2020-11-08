%{
#include <bits/stdc++.h>
#include "./codegen/ast.h"
#include "y.tab.h"
int yyerror(char*);
using namespace std;
%}
%option yylineno
DIGITS [0-9]
LETTERS [a-zA-Z]
IDENTIFIER ({LETTERS}|[_])({LETTERS}|{DIGITS}|[_])*
NUMBERS [0-9]+
CHAR ('.'|'\\.')
STRINGVALUE ["].*["]
%%
[ \n\t]+	;
"//".*		;
"=="		return EQ;
"||"		return OR;
"&&"		return AND;
[-;+*/<>!]	return *yytext;
[,(){}.]	return *yytext;
"["		return *yytext;
"]"		return *yytext;
"="		return *yytext;
"int"		return INT;
"char"		return CHAR;
"boolean"	return BOOL;
"new"		return NEW;
"this"		return THIS;
"true"		return True;
"false"		return False;
"null"		return Null;
"if"		return IF;
"else"		return ELSE;
"return"	return RETURN;
"while"		return WHILE;
"class"		return CLASS;
"static"	return STATIC;
"void"		return VOID;
{NUMBERS}		{
	yylval.ivalue = atoi(yytext);
	return INTCONSTANT;
}
{CHAR}			{
	yylval.ivalue = *yytext;
	return CHARCONST;
}
{IDENTIFIER}	{
	// memset(yylval.name,'\0',sizeof(yylval.name));
	int len = yyleng;
	if(yyleng >= sizeof(yylval.name))
	{
		len = sizeof(yylval.name)-1;
	}
	strncpy(yylval.name,yytext,len);
	yylval.name[len] = '\0';
	return IDENTIFIER;
}
.			{yyerror("unknown charecter\n");}
%%
int yywrap(void )
{
	return 1;
}