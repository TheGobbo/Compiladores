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
    Simbolo* BuscarSimbolo(const std::string& identificador);
    Simbolo* getTopo();

    bool empty();
    void clear();

    int getNovoDeslocamento(int nivel_lexico);
    int size() const;
    void setTipos(VariableType tipo);
    int setParamFormal();

    void show();

   private:
    int quantidade_simbolos{0};
    std::deque<Simbolo*> tabelaDeSimbolos;
};

#endif  // TABELA_SIMBOLOS_HPP
