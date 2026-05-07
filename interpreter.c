#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Environment* createEnv(Environment* parent) {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    env->head = NULL;
    env->parent = parent;
    return env;
}

void setVal(Environment* env, char* name, Value val) {
    ValEntry* entry = (ValEntry*)malloc(sizeof(ValEntry));
    entry->name = strdup(name);
    entry->val = val;
    entry->next = env->head;
    env->head = entry;
}

Value getVal(Environment* env, char* name) {
    Environment* currEnv = env;
    while (currEnv) {
        ValEntry* curr = currEnv->head;
        while (curr) {
            if (strcmp(curr->name, name) == 0) return curr->val;
            curr = curr->next;
        }
        currEnv = currEnv->parent;
    }
    printf("RUNTIME ERROR: Variable %s not found\n", name);
    exit(1);
}

Value interpretExpression(ASTNode* node, Environment* env) {
    Value res;
    if (!node) return res;

    if (node->type == NODE_INT_LIT) {
        res.type = "int";
        res.data.i_val = atoi(node->value);
        return res;
    }
    if (node->type == NODE_FLOAT_LIT) {
        res.type = "float";
        res.data.f_val = atof(node->value);
        return res;
    }
    if (node->type == NODE_BOOL_LIT) {
        res.type = "bool";
        res.data.b_val = (strcmp(node->value, "true") == 0) ? 1 : 0;
        return res;
    }
    if (node->type == NODE_IDENT) {
        return getVal(env, node->value);
    }
   if (node->type == NODE_BINOP) {
    Value left = interpretExpression(node->left, env);
    Value right = interpretExpression(node->right, env);

    if (strcmp(left.type, "int") == 0 && strcmp(right.type, "int") == 0) {
        res.type = "int";
        if (strcmp(node->value, "+") == 0) res.data.i_val = left.data.i_val + right.data.i_val;
        if (strcmp(node->value, "-") == 0) res.data.i_val = left.data.i_val - right.data.i_val;
        if (strcmp(node->value, "*") == 0) res.data.i_val = left.data.i_val * right.data.i_val;
        if (strcmp(node->value, "/") == 0) {
            if (right.data.i_val == 0) { printf("RUNTIME ERROR: Division by zero\n"); exit(1); }
            res.data.i_val = left.data.i_val / right.data.i_val;
        }
    } 
    else if (strcmp(left.type, "float") == 0 || strcmp(right.type, "float") == 0) {
        res.type = "float";
        double l_val = (strcmp(left.type, "int") == 0) ? left.data.i_val : left.data.f_val;
        double r_val = (strcmp(right.type, "int") == 0) ? right.data.i_val : right.data.f_val;

        if (strcmp(node->value, "+") == 0) res.data.f_val = l_val + r_val;
        if (strcmp(node->value, "-") == 0) res.data.f_val = l_val - r_val;
        if (strcmp(node->value, "*") == 0) res.data.f_val = l_val * r_val;
        if (strcmp(node->value, "/") == 0) {
            if (r_val == 0.0) { printf("RUNTIME ERROR: Division by zero\n"); exit(1); }
            res.data.f_val = l_val / r_val;
        }
    }
    return res;
}
    return res;
}

void interpret(ASTNode* node, Environment* env) {
    if (!node) return;

   if (node->type == NODE_VAR_DECL) {
    Value val = interpretExpression(node->right, env);
    
    if (strcmp(node->dataType, "float") == 0 && strcmp(val.type, "int") == 0) {
        val.type = "float";
        val.data.f_val = (double)val.data.i_val;
    }
    
    setVal(env, node->value, val);
}
    else if (node->type == NODE_PRINT) {
        Value v = interpretExpression(node->right, env);
        if (strcmp(v.type, "int") == 0) printf("%d\n", v.data.i_val);
        else if (strcmp(v.type, "float") == 0) printf("%f\n", v.data.f_val);
        else printf("%s\n", v.data.b_val ? "true" : "false");
    }

    interpret(node->left, env);
    interpret(node->right, env);
    interpret(node->next, env);
}