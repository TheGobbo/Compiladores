
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
#include <iostream>
#include <string>

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

std::deque<VariableType> stack_tipos;
TabelaSimbolos TS;
simbolos simbolo;

char meu_token[TAM_TOKEN];
int nivel_lexico = 0;

int num_line = 1;
int num_amem = 0;
int num_vars = 0;

bool print = false;

static std::ofstream fp;  // Use an ofstream instead of FILE* for C++ file handling
void geraCodigo(const char* rot, const char* comando) {
    if (!fp.is_open()) {
        fp.open("MEPA");  // Open the file "MEPA" for writing
    }

    if (rot == nullptr) {
        fp << "     " << comando << '\n';
        fp.flush();  // Flush the output to the file
    } else {
        fp << rot << ": " << comando << '\n';
        fp.flush();  // Flush the output to the file
    }
}

// espelho de bison::Parse::error
void error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line << '\n';
    exit(0);
}

void salvarTipos(simbolos simbolo) {
    VariableType tipo = simbolo == simb_integer   ? INTEIRO
                        : simbolo == simb_boolean ? BOOLEANO
                                                  : UNDEFINED;

    if (tipo == UNDEFINED) {
        error("undefined type at line ");
    }

    TS.setTipos(tipo);
}

void operaTiposValidos(VariableType resultado) {
    VariableType l, r;
    print_tipos();
    l = stack_tipos.back();
    stack_tipos.pop_back();

    r = stack_tipos.back();
    stack_tipos.pop_back();

    if (l != r) {
        error("type mismatch at line ");
    }
    stack_tipos.push_back(resultado);
    print_tipos();
}

void operaTiposValidos() {
    VariableType l, r;
    print_tipos();
    l = stack_tipos.back();
    stack_tipos.pop_back();

    r = stack_tipos.back();
    stack_tipos.pop_back();

    if (l != r) {
        error("type mismatch at line ");
    }

    print_tipos();
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