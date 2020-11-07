%{
#include <bits/stdc++.h>
#include "y.tab.h"
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
{NUMBERS}		return INTCONSTANT;
{CHAR}			return CHARCONST;
{IDENTIFIER}	return IDENTIFIER;
.			;
%%
int yywrap(void )
{
	return 1;
}