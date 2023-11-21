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
    simb_integer,
    simb_boolean,
} simbolos;

class Rotulos {
   public:
    Rotulos();

    Rotulos& push();
    Rotulos& push(char r);
    Rotulos& pop();
    char top() const;
    std::string transformTop() const;
    std::string transform(char rotulo) const;
    void show();
    int size();

   private:
    std::deque<char> stack_rotulos;
    char rotulo;
};

/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern std::deque<VariableType> stack_tipos;
extern std::deque<int> stack_mem;
extern Rotulos stack_rotulos;

extern MepaInterface MEPA;
extern TabelaSimbolos TS;
extern simbolos simbolo;

extern std::string meu_token;
extern int nivel_lexico;

extern int num_line;
extern int num_amem;
extern char num_params;
extern bool pf_ref;
extern int chamada_proc;
extern int idx_params;
extern std::deque<int> idxs_params;
extern std::deque<Simbolo*> stack_subrots;
extern std::deque<std::pair<Category, VariableType>> stack_param;

// extern std::string addr_variavel;

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

void paramFormal();
void fimParamFormal();
int getLoadType(Simbolo* simbolo, Simbolo* procedure);
void carregaValor(Simbolo* simbolo);
void armazenaValor(Simbolo* simbolo);

void beginProcedure();
void endProcedure();
void callProcedure();

void beginFunction();
void endFunction();
void callFunction();

void inicioParams();
void fimParams();

void validateSignature(Simbolo* subrot);

void declaraIdentificador();  // salvarVarSimples();

void Read();
void Write();

void aplicaAtribuicao();

void endCondicional();
void beginCondicional();
void elseCondicional();
void validateIf();

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

/* utils */

void print_tipos();
void error(const std::string& msg);
const std::string& itoa(int arg1);
const std::string& itoa(int arg1, int arg2);

#endif  // COMPILADOR_HPP
