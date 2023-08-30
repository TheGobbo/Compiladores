
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
#include <iomanip>
#include <iostream>
#include <string>

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

MepaInterface MEPA;

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

void popPenultRotulo() {
    std::size_t rot_size = stack_rotulos.size();
    if (rot_size < 2) {
        error("stack underflow on rotulos");
    }

    std::ostringstream rotulo;
    rotulo << "R" << std::setfill('0') << std::setw(3) << (int)stack_rotulos.at(rot_size - 2);
    // geraCodigo_(MEPA::ROTULO(rotulo.str()));
    MEPA.rotAddrNome(rotulo.str(), "NADA");

    stack_rotulos.erase(stack_rotulos.end() - 2);
}

std::string novoRotulo() {
    if (num_rots >= 255) {
        error("stack overflow: stack_rotulo");
    }
    stack_rotulos.push_back(num_rots);
    num_rots = num_rots + 1;
    return getRotulo();
}

std::string getRotulo() {
    std::ostringstream rotulo;
    rotulo << "R" << std::setfill('0') << std::setw(3) << (int)stack_rotulos.back();
    return rotulo.str();
}

void popRotulo() {
    std::size_t rot_size = stack_rotulos.size();
    if (rot_size < 1) {
        error("stack underflow on rotulos");
    }
    std::string topo = getRotulo();
    stack_rotulos.pop_back();

    MEPA.rotAddrNome(topo, "NADA");
}

void geraCodigoEndWhile() {
    std::string begin, end;
    end = getRotulo();
    stack_rotulos.pop_back();

    begin = getRotulo();
    stack_rotulos.pop_back();

    // geraCodigo_(MEPA::JUMP, begin);
    // geraCodigo_(MEPA::ROTULO(end));
    MEPA.JumpTo(begin);
    MEPA.rotAddrNome(end, "NADA");
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
