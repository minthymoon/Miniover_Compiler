#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_VAR_DECL,   //for the global too
    NODE_ASSIGN,
    NODE_BINOP,     
    NODE_COMPARE,   
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_RETURN,
    NODE_INT_LIT,     //integer lits
    NODE_FLOAT_LIT,   //float lits
    NODE_BOOL_LIT,    //the true and false ones
    NODE_IDENT,
    NODE_BLOCK 
} NodeType;


typedef struct ASTNode {
    NodeType type;
    char* value;
    char* dataType;
    struct ASTNode* left; 
    struct ASTNode* right;
    struct ASTNode* condition;
    struct ASTNode* else_body;
    struct ASTNode* next;
} ASTNode;

ASTNode* createNode(NodeType type, char* value, char* dataType);
void addChild(ASTNode* parent, ASTNode* child);
void printAST(ASTNode* node, int level);

#endif