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

#define TAM_TOKEN 16

#include "TabelaSimbolos.hpp"

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
extern TabelaSimbolos TS;
extern simbolos simbolo;

extern char meu_token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;

extern int num_line;
extern int num_amem;
extern int num_vars;
extern char num_rots;

extern std::string addr_variavel;

extern bool print; /*helper*/

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

void geraCodigo(const char* rot, std::string comando);

void geraCodigoWrite();
void geraCodigoRead();
void geraCodigoAtribuicao();
void geraCodigoWhile();
void geraCodigoDo();
void geraCodigoEndWhile();

void jumpTopoSeFalso();
void jumpTopoSempre();
void popRotulo();

std::string getAddrLex();
std::string novoRotulo();
std::string getRotulo();
void popPenultRotulo();

void salvarVariavel();
void salvarTipos(simbolos simbolo);
void operaTiposValidos(VariableType resultado);
void operaTiposValidos();

void print_tipos();
void error(const std::string& msg);