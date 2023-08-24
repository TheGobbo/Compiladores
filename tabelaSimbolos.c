#include "tabelaSimbolos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stack* newSymbolTable(void** ts) {
    Stack* s = createStack((void**)ts);
    return s;
}

SymbolEntry* newSymbolEntry(char* identifier, int lexical_level,
                            Category category, VariableType type,
                            PassageType passage, int displacement) {
    SymbolEntry* newEntry = (SymbolEntry*)malloc(sizeof(SymbolEntry));

    int sz = strlen(identifier);
    sz = sz > TAM_TOKEN ? TAM_TOKEN : sz;
    strncpy(newEntry->identifier, identifier, sz);
    newEntry->lexical_level = lexical_level;
    newEntry->displacement = displacement;
    newEntry->category = category;
    newEntry->passage = passage;
    newEntry->type = type;

    return newEntry;
}

void deleteSymbolEntry(SymbolEntry* symbolEntry) { free(symbolEntry); }

void insertSymbol(Stack* table, SymbolEntry* entry) {
    if (isFull(table)) {
        printf("Symbol Table is full. Cannot insert.\n");
        return;
    }
    table->top += 1;
    table->data[table->top] = entry;
}

void removeSymbol(Stack* table, int n) {
    while (!isEmpty(table) && n > 0) {
        deleteSymbolEntry(table->data[table->top]);
        table->top--;
        n--;
    }

    if (isEmpty(table) && n > 0) {
        printf("Symbol Table is empty. Cannot remove %d.\n", n);
    }
}

SymbolEntry* searchSymbol(Stack* table, char* identifier) {
    int sz = strlen(identifier);
    sz = sz > TAM_TOKEN ? TAM_TOKEN : sz;
    for (int i = table->top; i >= 0; i--) {
        if (strncmp(((SymbolEntry*)table->data[i])->identifier, identifier,
                    sz) == 0) {
            return table->data[i];
        }
    }
    return NULL;
}

void showTopTS(Stack* symbolTable) {
    printf("topo tabela_de_simbolos[%d]:\n", symbolTable->top);
    printf("char identifier[TAM_TOKEN] = %s \n",
           ((SymbolEntry*)peek(symbolTable))->identifier);
    printf("int lexical_level = %d \n",
           ((SymbolEntry*)peek(symbolTable))->lexical_level);
    printf("int displacement = %d \n",
           ((SymbolEntry*)peek(symbolTable))->displacement);

    // Category cat = ((SymbolEntry*)peek(symbolTable))->category;
    // printf("Category category = %s \n",
    //        cat == SIMPLE_VARIABLE    ? "SIMPLE_VARIABLE"
    //        : cat == FORMAL_PARAMETER ? "FORMAL_PARAMETER"
    //        : cat == PROCEDURE        ? "PROCEDURE"
    //                                  : "nao identificado?");

    // VariableType var = ((SymbolEntry*)peek(symbolTable))->type;
    // printf("VariableType type = %s \n",
    //        var == INTEGER ? "INTEGER" : "nao identificado?");

    // PassageType pas = ((SymbolEntry*)peek(symbolTable))->passage;
    // printf("PassageType passage = %s \n", pas == BY_VALUE ? "BY_VALUE"
    //                                       : pas == BY_REFERENCE
    //                                           ? "BY_REFERENCE"
    //                                           : "nao identificado?");
}

int main() {
    // Example SymbolEntry
    Stack* stackSymbolTable = newSymbolTable((void**)&TS);
    SymbolEntry* entry1 =
        newSymbolEntry("var1", 1, SIMPLE_VARIABLE, INTEGER, BY_VALUE, 0);

    printf("criado symbol : %s\n", entry1->identifier);

    // Test insertSymbol
    insertSymbol(stackSymbolTable, entry1);
    showTopTS(stackSymbolTable);

    char* searchIdentifier = "variable1";
    SymbolEntry* foundEntry = searchSymbol(stackSymbolTable, searchIdentifier);

    if (foundEntry != NULL)
        printf("Found Symbol: %s\n", foundEntry->identifier);
    else
        printf("Symbol not found.\n");

    removeSymbol(stackSymbolTable, 1);
    showTopTS(stackSymbolTable);

    return 0;
}