#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Symbol {
    char* name;
    char* type; 
    int isGlobal;       // 1 if global, 0 if local
    struct Symbol* next; 
} Symbol;


typedef struct SymbolTable {
    Symbol* head;
    struct SymbolTable* parent; 
} SymbolTable;

SymbolTable* createTable(SymbolTable* parent);
void addSymbol(SymbolTable* table, char* name, char* type, int isGlobal);
Symbol* lookup(SymbolTable* table, char* name);

#endif