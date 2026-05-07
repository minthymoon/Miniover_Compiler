#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "symbol_table.h"

typedef struct {
    char* type;
    union {
        int i_val;
        double f_val;
        int b_val;
        void* node_ptr;
    } data;
} Value;

typedef struct ValEntry {
    char* name;
    Value val;
    struct ValEntry* next;
} ValEntry;

typedef struct Environment {
    ValEntry* head;
    struct Environment* parent;
} Environment;

Value interpretExpression(ASTNode* node, Environment* env);
Value interpret(ASTNode* node, Environment* env);
Value getVal(Environment* env, char* name);
void setVal(Environment* env, char* name, Value val);
Environment* createEnv(Environment* parent);

#endif