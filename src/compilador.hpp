/* -------------------------------------------------------------------
 *            Arquivo: compilador.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [09/08/2020, 19h:01m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e variáveis globais do compilador (via extern)
 *
 * ------------------------------------------------------------------- */

#ifndef COMPILADOR_HPP
#define COMPILADOR_HPP

#include <map>

#include "code_generation/MepaInterface.hpp"
#include "code_generation/TabelaSimbolos.hpp"

typedef enum simbolos {
    simb_program,
    simb_label,
    simb_type,
    simb_array,
    simb_of,
    simb_var,
    simb_integer,
    simb_boolean,
    simb_procedure,
    simb_function,
    simb_goto,
    simb_if,
    simb_then,
    simb_else,
    simb_while,
    simb_do,
    simb_or,
    simb_div,
    simb_mult,
    simb_and,
    simb_not,
    simb_true,
    simb_false,
    simb_begin,
    simb_end,
    simb_read,
    simb_write,
    simb_atribuicao,
    simb_ponto_e_virgula,
    simb_dois_pontos,
    simb_virgula,
    simb_ponto,
    simb_abre_parenteses,
    simb_fecha_parenteses,
    simb_mais,
    simb_menos,
    simb_diferenca,
    simb_igualdade,
    simb_menor_igual,
    simb_maior_igual,
    simb_maior,
    simb_menor,

    simb_numero,
    simb_identificador,
} simbolos;

/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern std::deque<VariableType> stack_tipos;
extern std::deque<char> stack_rotulos;
extern std::deque<int> stack_mem;

extern MepaInterface MEPA;
extern TabelaSimbolos TS;
extern simbolos simbolo;

extern std::string meu_token;
extern int nivel_lexico;

extern int num_line;
extern int num_amem;
extern char num_rots;

extern std::string addr_variavel;

extern bool print; /*helper*/

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

void beginCompilador();
void endCompilador();

void saveSimbolo();

void varsDeclarado();
void subrotDeclarado();
void endComandos();

void aplicarTipos();
void novoSimbolo();  // declaraVar();

void beginProcedure();
void endProcedure();
void callProcedure();

void declaraIdentificador();  // salvarVarSimples();

void Read();
void Write();

void aplicaAtribuicao();

void endCondicional();
void beginCondicional();
void elseCondicional();

void beginWhile();
void endWhile();

// void aplicarOperacao(simbolos simbolo);  // token MAIS/DIV/OR/...)
void aplicarOperacao(const std::string& command, VariableType resultado);
void saveVariavel();
void loadConstante(std::string valor);

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */
void removeForaEscopo();

void operaTiposValidos(VariableType resultado);
void operaTiposValidos();

std::string getAddrLex();

void popRotulo();
void popPenultRotulo();
std::string novoRotulo();
std::string getRotulo();
std::string getRotulo(char rotulo);
char novoRotuloc();

void geraCodigoEndWhile();

/* utils */
void print_tipos();
void error(const std::string& msg);
const std::string& itoa(int arg1);
const std::string& itoa(int arg1, int arg2);

#endif  // COMPILADOR_HPP
