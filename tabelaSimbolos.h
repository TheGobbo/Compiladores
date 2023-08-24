#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "compilador.h"
#include "stack.h"

typedef enum { SIMPLE_VARIABLE, FORMAL_PARAMETER, PROCEDURE } Category;
typedef enum { BY_VALUE, BY_REFERENCE } PassageType;  // for formal parameters
typedef enum { INTEGER } VariableType;

typedef struct {
    char identifier[TAM_TOKEN];
    int lexical_level;
    int displacement;
    Category category;
    VariableType type;
    PassageType passage;
} SymbolEntry;

SymbolEntry TS[MAX_STACK_SIZE];  // tabela de simbolos

Stack* newSymbolTable(void** ts);

SymbolEntry* newSymbolEntry(char* identifier, int lexical_level,
                            Category category, VariableType type,
                            PassageType passage, int displacement);

void deleteSymbolEntry(SymbolEntry* symbolEntry);

void insertSymbol(Stack* table, SymbolEntry* entry);
void removeSymbol(Stack* table, int n);
SymbolEntry* searchSymbol(Stack* table, char* identifier);
void

#endif  // SYMBOL_TABLE_H
