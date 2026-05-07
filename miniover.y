%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "type_checker.h"
#include "interpreter.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);

ASTNode* root; 
SymbolTable* globalTable;
Environment* globalEnv;
%}

%union {
    char* str_val;
    struct ASTNode* node;
}

%token <str_val> IDENT TYPE_INT TYPE_FLOAT TYPE_BOOL INT_LIT FLOAT_LIT TRUE FALSE
%token FN LET GLOBAL IF ELSE WHILE RETURN PRINT
%token PLUS MINUS MULT DIV EQ NE LT GT AND OR NOT ASSIGN
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA ARROW

%type <str_val> type_spec
%type <node> program external_list external_decl stmt_list stmt expr var_decl


%left OR
%left AND
%left EQ NE LT GT
%left PLUS MINUS
%left MULT DIV
%right NOT

%%

program:
    external_list { 
        root = $1; 
        printf("\n--- AST Construction Complete ---\n");
        printAST(root, 0); 
    }
    ;

external_list:
    external_decl { $$ = $1; }
    | external_decl external_list { $1->next = $2; $$ = $1; }
    ;

external_decl:
    var_decl { $$ = $1; }
    | FN IDENT LPAREN RPAREN LBRACE stmt_list RBRACE { 
        $$ = createNode(NODE_FUNCTION, $2, "function"); 
        $$->left = $6; 
    }
    ;

stmt_list:
    stmt { $$ = $1; }
    | stmt stmt_list { $1->next = $2; $$ = $1; }
    ;

stmt:
    var_decl { $$ = $1; }
    | PRINT LPAREN expr RPAREN SEMICOLON {
        $$ = createNode(NODE_PRINT, NULL, NULL);
        $$->right = $3;
    }
    | RETURN expr SEMICOLON {
        $$ = createNode(NODE_RETURN, NULL, NULL);
        $$->right = $2;
    }
    | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        $$ = createNode(NODE_IF, NULL, NULL);
        $$->condition = $3;
        $$->left = $6;
    }
    | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE {
        $$ = createNode(NODE_IF, NULL, NULL);
        $$->condition = $3;
        $$->left = $6;
        $$->else_body = $10;
    }
    | WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {
        $$ = createNode(NODE_WHILE, NULL, NULL);
        $$->condition = $3;
        $$->left = $6;
    }
    ;


var_decl:
    LET IDENT type_spec ASSIGN expr SEMICOLON {
        $$ = createNode(NODE_VAR_DECL, $2, $3); 
        $$->right = $5;
        addSymbol(globalTable, $2, $3, 0);
    }
    | GLOBAL IDENT type_spec ASSIGN expr SEMICOLON {
        $$ = createNode(NODE_VAR_DECL, $2, $3); 
        $$->value = "global";
        $$->right = $5;
        addSymbol(globalTable, $2, $3, 1);
    }
    ;
type_spec:
    TYPE_INT    { $$ = $1; }
    | TYPE_FLOAT  { $$ = $1; }
    | TYPE_BOOL   { $$ = $1; }
    ;

expr:
    INT_LIT     { $$ = createNode(NODE_INT_LIT, $1, "int"); }
    | FLOAT_LIT { $$ = createNode(NODE_FLOAT_LIT, $1, "float"); }
    | TRUE      { $$ = createNode(NODE_BOOL_LIT, "true", "bool"); }
    | FALSE     { $$ = createNode(NODE_BOOL_LIT, "false", "bool"); }
    | IDENT     { $$ = createNode(NODE_IDENT, $1, NULL); }
    | expr PLUS expr  { $$ = createNode(NODE_BINOP, "+", NULL); $$->left = $1; $$->right = $3; }
    | expr MINUS expr { $$ = createNode(NODE_BINOP, "-", NULL); $$->left = $1; $$->right = $3; }
    | expr MULT expr  { $$ = createNode(NODE_BINOP, "*", NULL); $$->left = $1; $$->right = $3; }
    | expr DIV expr   { $$ = createNode(NODE_BINOP, "/", NULL); $$->left = $1; $$->right = $3; }
    | expr EQ expr    { $$ = createNode(NODE_COMPARE, "==", NULL); $$->left = $1; $$->right = $3; }
    | expr NE expr      { $$ = createNode(NODE_COMPARE, "!=", NULL); $$->left = $1; $$->right = $3; }
    | expr LT expr      { $$ = createNode(NODE_COMPARE, "<", NULL);  $$->left = $1; $$->right = $3; }
    | expr GT expr      { $$ = createNode(NODE_COMPARE, ">", NULL);  $$->left = $1; $$->right = $3; }
    | expr AND expr     { $$ = createNode(NODE_BINOP, "&&", NULL);   $$->left = $1; $$->right = $3; }
    | expr OR expr      { $$ = createNode(NODE_BINOP, "||", NULL);   $$->left = $1; $$->right = $3; }
    | LPAREN expr RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "PARSER ERROR at line %d: %s near '%s'\n", yylineno, s, yytext);
}

int main() {
    globalTable = createTable(NULL);
    yyparse();
    if (root) {
        printf("\n--- Starting Type Checking ---\n");
        typeCheck(root, globalTable);
        printf("Type Checking Passed Successfully!\n");
    }
    printf("\n--- Starting Program Execution ---\n");
        globalEnv = createEnv(NULL);
        interpret(root, globalEnv);
        printf("\n--- Execution Finished ---\n");
    return 0;
}