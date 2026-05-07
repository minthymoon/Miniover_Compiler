%{
#include "miniover.tab.h"
#include <string.h>
#include <stdio.h>
%}

%option yylineno

DIGIT    [0-9]
LETTER   [a-zA-Z]
ID       {LETTER}({LETTER}|{DIGIT}|_)*
INT      {DIGIT}+
FLOAT    {DIGIT}+"."{DIGIT}+

%%
"fn"        { return FN; }
"let"       { return LET; }
"global"    { return GLOBAL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"return"    { return RETURN; }
"print"     { return PRINT; }

"int"       { yylval.str_val = strdup(yytext); return TYPE_INT; }
"float"     { yylval.str_val = strdup(yytext); return TYPE_FLOAT; }
"bool"      { yylval.str_val = strdup(yytext); return TYPE_BOOL; }
"true"      { return TRUE; }
"false"     { return FALSE; }

{FLOAT}     { yylval.str_val = strdup(yytext); return FLOAT_LIT; }
{INT}       { yylval.str_val = strdup(yytext); return INT_LIT; }
{ID}        { yylval.str_val = strdup(yytext); return IDENT; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"=="        { return EQ; }
"!="        { return NE; }
"<"         { return LT; }
">"         { return GT; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }
"="         { return ASSIGN; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
";"         { return SEMICOLON; }
","         { return COMMA; }
"->"        { return ARROW; }

[ \t\n\r]+  {  }
"//".*      {  }
.           { printf("SCANNER ERROR at line %d: %s\n", yylineno, yytext); }

%%

int yywrap() {
    return 1;
}