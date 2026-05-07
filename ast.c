#include "ast.h"

ASTNode* createNode(NodeType type, char* value, char* dataType) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->dataType = dataType ? strdup(dataType) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->else_body = NULL;
    node->next = NULL;
    return node;
}

void printAST(ASTNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) printf("  ");

    printf("- Type: %d", node->type);
    if (node->value) printf(" | Val: %s", node->value);
    if (node->dataType) printf(" | Type: %s", node->dataType);
    printf("\n");

    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
    printAST(node->condition, level + 1);
    printAST(node->next, level);
}