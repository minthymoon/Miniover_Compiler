#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "ast.h"
#include "symbol_table.h"

char* checkExpression(ASTNode* node, SymbolTable* table);

void typeCheck(ASTNode* node, SymbolTable* table);

#endif