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

/*
namespace MEPA {
const char* READ = "\tLEIT ";
const char* WRITE = "\tIMPR ";
const char* STORE = "\tARMZ ";
const char* LOADV = "\tCRVL ";
const char* LOADC = "\tCRCT ";
const char* JUMP = "\tDSVS ";
const char* JUMP_SE = "\tDSVF ";
const char* MAIOR_IGUAL = "\tCMEG ";
const char* MENOR_IGUAL = "\tCMAG ";
const char* MENOR = "\tCMME ";
const char* MAIOR = "\tCMMA ";
const char* IGUAL = "\tCMIG ";
const char* DIFF = "\tCMDF ";
const char* SOMA = "\tSOMA ";
const char* SUBT = "\tSUBT ";
const char* DIV = "\tDIVI ";
const char* MULT = "\tMULT ";
const char* OR = "\tDISJ ";
const char* AND = "\tCONJ ";
}  // namespace MEPA
*/

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

/*
template <typename T, typename... Types>
void geraCodigo(T var1, Types... var2);

void geraCodigo() {
    fp << '\n';
    fp.flush();
}

/* geraCodigo(1, 2, 3, 4) prints "1, 2, 3, 4\n"
template <typename T, typename... Types>
void geraCodigo(T var1, Types... var2) {
    if (!fp.is_open()) {
        fp.open("MEPA");  // Open the file "MEPA" for writing
    }
    fp << var1;

    if constexpr (std::is_same_v<T, const char*>) {
        if(var1 && *var1 == 'R')
        fp << ": ";
    }

    fp.flush();

    geraCodigo(var2...);
}
*/
void geraCodigo(const char* rot, std::string comando);

void geraCodigoWrite();
void geraCodigoRead();
void geraCodigoAtribuicao();
void geraCodigoWhile();
void geraCodigoDo();
void geraCodigoEndWhile();

std::string getAddrLex();
std::string novoRotulo();
std::string getRotulo();

void salvarVariavel();
void salvarTipos(simbolos simbolo);
void operaTiposValidos(VariableType resultado);
void operaTiposValidos();

void print_tipos();
void error(const std::string& msg);