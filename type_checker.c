#include "type_checker.h"
#include <stdio.h>
#include <string.h>

char* checkExpression(ASTNode* node, SymbolTable* table) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_INT_LIT: return "int";
        case NODE_FLOAT_LIT: return "float";
        case NODE_BOOL_LIT: return "bool";
        
        case NODE_IDENT: {
            Symbol* sym = lookup(table, node->value);
            if (!sym) {
                printf("SEMANTIC ERROR: Variable '%s' undefined.\n", node->value);
                exit(1);
            }
            return sym->type;
        }

        case NODE_CALL: {
            Symbol* sym = lookup(table, node->value);
            if (!sym) {
                printf("SEMANTIC ERROR: Function '%s' undefined.\n", node->value);
                exit(1);
            }
            return sym->type; 
        }

       case NODE_BINOP: {
         char* leftT = checkExpression(node->left, table);
         char* rightT = checkExpression(node->right, table);

         if (strcmp(leftT, rightT) == 0) return leftT;

         if ((strcmp(leftT, "int") == 0 && strcmp(rightT, "float") == 0) ||
           (strcmp(leftT, "float") == 0 && strcmp(rightT, "int") == 0)) {
            return "float";
        }

         printf("TYPE ERROR: Invalid types %s and %s for operation %s.\n", leftT, rightT, node->value);
         exit(1);
        }

        case NODE_COMPARE: {
            char* leftT = checkExpression(node->left, table);
            char* rightT = checkExpression(node->right, table);
            if (strcmp(leftT, rightT) != 0) {
                printf("TYPE ERROR: Cannot compare %s with %s.\n", leftT, rightT);
                exit(1);
            }
            return "bool";
        }

        default: return NULL;
    }
}

void typeCheck(ASTNode* node, SymbolTable* table) {
    if (!node) return;

    if (node->type == NODE_FUNCTION) {
    addSymbol(table, node->value, node->dataType, 1); 

    SymbolTable* localTable = createTable(table);
    ASTNode* p = node->params;
    while (p != NULL) {
        addSymbol(localTable, p->value, p->dataType, 0);
        p = p->next;
    }
    typeCheck(node->left, localTable);
    typeCheck(node->next, table);
    return; 
}
//type_error
  if (node->type == NODE_VAR_DECL) {
    char* exprType = checkExpression(node->right, table);
    char* finalType = node->dataType ? node->dataType : exprType;

    if (node->dataType) {
        int isCompatible = (strcmp(exprType, node->dataType) == 0) || 
                           (strcmp(node->dataType, "float") == 0 && strcmp(exprType, "int") == 0);
        if (!isCompatible) {
            printf("TYPE ERROR: Cannot assign %s to variable of type %s.\n", exprType, node->dataType);
            exit(1);
        }
    }
    
    addSymbol(table, node->value, finalType, 0);
}

    if (node->type == NODE_IF || node->type == NODE_WHILE) {
        char* condType = checkExpression(node->condition, table);
        if (strcmp(condType, "bool") != 0) {
            printf("TYPE ERROR: Condition must be bool, found %s.\n", condType);
            exit(1);
        }
    }
if (node->type != NODE_FUNCTION && node->type != NODE_VAR_DECL) {
    typeCheck(node->left, table);
    typeCheck(node->right, table);
    typeCheck(node->next, table);
}
}