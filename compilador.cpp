
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

static std::ofstream fp;  // Para geracao de codigo MEPA

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

    return " " + nl + " " + ds;
}

std::string novoRotulo() {
    if (num_rots >= 255) {
        error("stack overflow: stack_rotulo");
    }
    num_rots = num_rots + 1;
    stack_rotulos.push_back(num_rots);
    return getRotulo();
}

std::string getRotulo() {
    std::ostringstream rotulo;
    rotulo << "R" << std::setfill('0') << std::setw(3) << (int)stack_rotulos.back();
    return rotulo.str();
}

/* -------------------------------------------------------------------
 * funcoes de codigo MEPA
 * ------------------------------------------------------------------- */

void geraCodigo(const char* rot, std::string comando) {
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

void geraCodigoRead() {
    geraCodigo(NULL, "LEIT");
    geraCodigo(NULL, "ARMZ" + getAddrLex());
}
void geraCodigoWrite() {
    geraCodigo(NULL, "CRVL" + getAddrLex());
    geraCodigo(NULL, "IMPR");
}

void geraCodigoAtribuicao() {
    operaTiposValidos();
    geraCodigo(NULL, "ARMZ" + addr_variavel);
}

void geraCodigoWhile() { geraCodigo(novoRotulo().c_str(), "NADA"); }
void geraCodigoDo() { geraCodigo(NULL, "DSVF " + novoRotulo()); }

void geraCodigoEndWhile() {
    std::string begin, end;
    end = getRotulo();
    stack_rotulos.pop_back();

    begin = getRotulo();
    stack_rotulos.pop_back();

    geraCodigo(NULL, "DSVS " + begin);
    geraCodigo(end.c_str(), "NADA");
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
