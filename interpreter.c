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

void updateVal(Environment* env, char* name, Value val) {
    Environment* currEnv = env;
    while (currEnv) {
        ValEntry* curr = currEnv->head;
        while (curr) {
            if (strcmp(curr->name, name) == 0) {
                curr->val = val;
                return;
            }
            curr = curr->next;
        }
        currEnv = currEnv->parent;
    }
    printf("RUNTIME ERROR: Variable %s not defined\n", name);
    exit(1);
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

  if (node->type == NODE_CALL) {
    Value funcVal = getVal(env, node->value);
    ASTNode* funcNode = (ASTNode*)funcVal.data.node_ptr;
    Environment* localEnv = createEnv(env); 
    ASTNode* param = funcNode->params;
    ASTNode* arg = node->right; 
    
    while (param && arg) {
        Value val = interpretExpression(arg, env);
        if (strcmp(param->dataType, "float") == 0 && strcmp(val.type, "int") == 0) {
            val.type = "float";
            val.data.f_val = (double)val.data.i_val;
        }
        
        setVal(localEnv, param->value, val);
        param = param->next;
        arg = arg->next;
    }
    return interpret(funcNode->left, localEnv);
}
   if (node->type == NODE_BINOP|| node->type == NODE_COMPARE) {
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
        if (strcmp(node->value, "<") == 0) {
          res.type = "bool";
          res.data.b_val = left.data.i_val < right.data.i_val;
        }
        else if (strcmp(node->value, ">") == 0) {
          res.type = "bool";
          res.data.b_val = left.data.i_val > right.data.i_val;
        }
        else if (strcmp(node->value, "==") == 0) {
         res.type = "bool";
         res.data.b_val = left.data.i_val == right.data.i_val;
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
        else if (strcmp(node->value, "<") == 0) {
         res.type = "bool";
         res.data.b_val = l_val < r_val;
        }
        else if (strcmp(node->value, ">") == 0) {
         res.type = "bool";
         res.data.b_val = l_val > r_val;
        }
        else if (strcmp(node->value, "==") == 0) {
         res.type = "bool";
         res.data.b_val = l_val == r_val;
        }
    }
    return res;
}
    return res;
}

Value interpret(ASTNode* node, Environment* env) {
   Value voidVal = { .type = "void" }; 
    if (!node) return voidVal;

    if (node->type == NODE_RETURN) {
        return interpretExpression(node->right, env);
    }


    if (node->type == NODE_FUNCTION) {
    Value funcVal;
    funcVal.type = "function";
    funcVal.data.node_ptr = node;
    setVal(env, node->value, funcVal);  
    return interpret(node->next, env);
}

if (node->type == NODE_CALL) {
    Value funcVal = getVal(env, node->value);
    ASTNode* funcNode = (ASTNode*)funcVal.data.node_ptr;
    Environment* localEnv = createEnv(env);
    ASTNode* param = funcNode->params;
    ASTNode* arg = node->right;

    while (param && arg) {
        Value val = interpretExpression(arg, env);

        if (strcmp(param->dataType, "float") == 0 && strcmp(val.type, "int") == 0) {
            val.type = "float";
            val.data.f_val = (double)val.data.i_val;
        }   
        setVal(localEnv, param->value, val);
        param = param->next;
        arg = arg->next;
    }
   return interpret(funcNode->left, localEnv);
}

   if (node->type == NODE_VAR_DECL) {
    Value val = interpretExpression(node->right, env);
    
    if (strcmp(node->dataType, "float") == 0 && strcmp(val.type, "int") == 0) {
        val.type = "float";
        val.data.f_val = (double)val.data.i_val;
    }
    
    setVal(env, node->value, val);
    return interpret(node->next, env);
}
    else if (node->type == NODE_PRINT) {
        Value v = interpretExpression(node->right, env);
        if (strcmp(v.type, "int") == 0) printf("%d\n", v.data.i_val);
        else if (strcmp(v.type, "float") == 0) printf("%f\n", v.data.f_val);
        else printf("%s\n", v.data.b_val ? "true" : "false");
        return interpret(node->next, env);
    }
    else if (node->type == NODE_WHILE) {
    while (1) {
        Value condition = interpretExpression(node->condition, env);
        
        if (strcmp(condition.type, "bool") != 0) {
            printf("RUNTIME ERROR: While condition must be bool\n");
            exit(1);
        }

        if (condition.data.b_val) {
            interpret(node->left, env);
        } else {
            break;
        }
    }
     return interpret(node->next, env);
}
    else if (node->type == NODE_ASSIGN) {
     Value val = interpretExpression(node->right, env);
     updateVal(env, node->value, val);
     return interpret(node->next, env);
    }

   Value leftRes = interpret(node->left, env);
    if (strcmp(leftRes.type, "void") != 0) return leftRes;
   Value rightRes = interpret(node->right, env);
    if (strcmp(rightRes.type, "void") != 0) return rightRes;

    return voidVal;
}