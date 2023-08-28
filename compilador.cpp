
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [09/08/2020, 19h:01m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include "compilador.hpp"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

std::deque<VariableType> stack_tipos;
std::deque<char> stack_rotulos;
TabelaSimbolos TS;
simbolos simbolo;

char meu_token[TAM_TOKEN];
int nivel_lexico = 0;

int num_line = 1;
int num_amem = 0;
int num_vars = 0;
char num_rots = 0;

std::string addr_variavel;

bool print = false;

/* -------------------------------------------------------------------
 * funcoes do Compilador
 * ------------------------------------------------------------------- */

void salvarTipos(simbolos simbolo) {
    VariableType tipo = simbolo == simb_integer   ? INTEIRO
                        : simbolo == simb_boolean ? BOOLEANO
                                                  : UNDEFINED;

    if (tipo == UNDEFINED) {
        error("undefined type");
    }

    TS.setTipos(tipo);
}

void operaTiposValidos() {
    VariableType l, r;
    print_tipos();
    l = stack_tipos.back();
    stack_tipos.pop_back();

    r = stack_tipos.back();
    stack_tipos.pop_back();

    if (l != r) {
        error("type mismatch");
    }
}

void operaTiposValidos(VariableType resultado) {
    operaTiposValidos();
    stack_tipos.push_back(resultado);
    print_tipos();
}

std::string getAddrLex() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr || !simbolo->valido()) {
        error("undefined symbol");
    }

    std::string nl = std::to_string(simbolo->getNivelLexico());
    std::string ds = std::to_string(simbolo->getDeslocamento());

    return nl + ", " + ds;
}

/* -------------------------------------------------------------------
 * funcoes auxiliares
 * ------------------------------------------------------------------- */

// espelho de bison::Parse::error
void error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line << '\n';
    exit(0);
}

void print_tipos() {
    const char* variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    std::cout << "stack_tipos : (front) ";
    for (auto it = stack_tipos.begin(); it != stack_tipos.end(); ++it) {
        if (it != stack_tipos.begin()) std::cout << ", ";
        std::cout << variableTypeNames[*it];
    }

    std::cout << " (back)" << std::endl;
}
