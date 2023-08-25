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

#include "Simbolo.hpp"

class TabelaSimbolos {
   public:
    TabelaSimbolos();
    ~TabelaSimbolos();

    void InsereSimbolo(Simbolo* simbolo);
    void RemoveSimbolos(int8_t num_simb);
    Simbolo* BuscaSimbolo(char (&identificador)[TAM_TOKEN]);

    int8_t getNovoDeslocamento(int8_t nivel_lexico);
    void show();

   private:
    int8_t num_simb{0};
    std::deque<Simbolo*> tabelaDeSimbolos;
};

#endif  // TABELA_SIMBOLOS_HPP
