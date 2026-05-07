#include "symbol_table.h"

SymbolTable* createTable(SymbolTable* parent) {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    table->head = NULL;
    table->parent = parent;
    return table;
}

void addSymbol(SymbolTable* table, char* name, char* type, int isGlobal) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->isGlobal = isGlobal;
    newSymbol->next = table->head;
    table->head = newSymbol;
}

Symbol* lookup(SymbolTable* table, char* name) {
    if (!table) return NULL;
    
    Symbol* current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    
    return lookup(table->parent, name);
}