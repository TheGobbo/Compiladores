/*
https://www.geeksforgeeks.org/stack-in-cpp-stl/
The functions associated with stack are:
empty() – Returns whether the stack is empty – Time Complexity : O(1)
size() – Returns the size of the stack – Time Complexity : O(1)
top() – Returns a reference to the top most element of the stack – Time
Complexity : O(1) push(g) – Adds the element ‘g’ at the top of the stack – Time
Complexity : O(1) pop() – Deletes the most recent entered element of the stack –
Time Complexity : O(1)
*/

#ifndef TABELA_SIMBOLOS_HPP
#define TABELA_SIMBOLOS_HPP

#include <deque>

#define MAX_STACK_SIZE 100

#include "code_generation/Simbolo.hpp"

class TabelaSimbolos {
   public:
    TabelaSimbolos();
    ~TabelaSimbolos();

    void InsereSimbolo(Simbolo* simbolo);
    void RemoveSimbolos(int quantidade_simbolos);
    // void RemoveProcedures(int nivel_lexico);
    Simbolo* BuscarSimbolo(const std::string& identificador);
    Simbolo* getTopo();

    bool empty();
    void clear();

    int getNovoDeslocamento(int nivel_lexico);
    int size() const;
    void setTipos(VariableType tipo);

    void show();

   private:
    int quantidade_simbolos{0};
    std::deque<Simbolo*> tabelaDeSimbolos;
};

#endif  // TABELA_SIMBOLOS_HPP
