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
    simb_begin,
    simb_end,
    simb_atribuicao,
    simb_ponto_e_virgula,
    simb_dois_pontos,
    simb_virgula,
    simb_ponto,
    simb_abre_parenteses,
    simb_fecha_parenteses,

    simb_mais,
    simb_menos,
    // simb_asterisco,
    // simb_barra,

    simb_diferenca,
    simb_menor,
    simb_menor_igual,
    simb_maior_igual,
    simb_maior,

    simb_numero,
    simb_identificador,
} simbolos;

/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern TabelaSimbolos TS;
extern simbolos simbolo, relacao;
extern char meu_token[TAM_TOKEN];
extern int8_t nivel_lexico;
extern int8_t desloc;
extern int num_line;
extern int8_t num_amem;
// extern char* yytext;

extern bool print; /*helper*/

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */

void geraCodigo(const char* rot, const char* comando);
int yylex();
// void yyerror(const char* s);

void type_checker();