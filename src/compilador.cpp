
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
#include <map>  // operacao
#include <string>
/*GAMBI - mudar*/
#include <cstring>

#define flags(STR) std::cerr << "\033[1;31m" << STR << "\033[0m\n"
#define flag std::cerr << "\033[1;31mFLAG\033[0m\n"

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

MepaInterface MEPA("output/output.mepa");

std::deque<VariableType> stack_tipos;
std::deque<char> stack_rotulos;
std::deque<int> stack_mem;

TabelaSimbolos TS;
simbolos simbolo;

char meu_token[TAM_TOKEN];
char proc_tk[TAM_TOKEN]; /* GAMBI */
int nivel_lexico = 0;

int num_line = 1;
int num_amem = 0;
char num_rots = 0;

std::string addr_variavel;

bool print = false;

std::map<simbolos, std::pair<const char*, VariableType>> operations;

/* -------------------------------------------------------------------
 * funcoes do Compilador
 * ------------------------------------------------------------------- */
// auxiliar //
void saveCurrentSimbolo() {}

/* PROGRAM */
void beginCompilador() {
    MEPA.Start();

    operations[simb_or] = std::pair{MEPA.OR, BOOLEANO};
    operations[simb_and] = std::pair{MEPA.AND, VariableType::BOOLEANO};
    operations[simb_maior] = std::pair{MEPA.MAIOR, VariableType::BOOLEANO};
    operations[simb_menor] = std::pair{MEPA.MENOR, VariableType::BOOLEANO};
    operations[simb_diferenca] = std::pair{MEPA.DIFF, VariableType::BOOLEANO};
    operations[simb_igualdade] = std::pair{MEPA.IGUAL, VariableType::BOOLEANO};
    operations[simb_maior_igual] = std::pair{MEPA.MAIOR_IGUAL, VariableType::BOOLEANO};
    operations[simb_menor_igual] = std::pair{MEPA.MENOR_IGUAL, VariableType::BOOLEANO};

    operations[simb_div] = std::pair{MEPA.DIV, VariableType::INTEIRO};
    operations[simb_mult] = std::pair{MEPA.MULT, VariableType::INTEIRO};
    operations[simb_mais] = std::pair{MEPA.SOMA, VariableType::INTEIRO};
    operations[simb_menos] = std::pair{MEPA.SUBT, VariableType::INTEIRO};
}
void endCompilador() {
    flags("endCompilador");
    MEPA.Finish();
    TS.clear();
    TS.show();
}

/* BLOCO */
void varsDeclarado() {
    MEPA.Alloc(num_amem);
    stack_mem.push_back(num_amem);
    num_amem = 0;
    print = true;
}
void subrotDeclarado() {
    flags("NL : " + std::to_string(nivel_lexico));
    std::deque<char>::iterator it;
    for (it = stack_rotulos.begin(); it != stack_rotulos.end(); ++it) {
        flags("subrot: " + getRotulo((*it)));
    }
    TS.show();

    if (stack_rotulos.size() == 1) {
        MEPA.rotAddrNome(getRotulo(stack_rotulos.back()));
        stack_rotulos.pop_back();
    }
}
void endComandos() {
    removeForaEscopo();
    MEPA.Free(stack_mem.back());
    stack_mem.pop_back();
}

/* DECLARA_VARS & LISTA_ID_VAR */
void aplicarTipos() {
    VariableType tipo = simbolo == simb_integer   ? INTEIRO
                        : simbolo == simb_boolean ? BOOLEANO
                                                  : UNDEFINED;
    if (tipo == UNDEFINED) {
        error("undefined type");
    }

    TS.setTipos(tipo);
}
void novoSimbolo() {
    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico,
                             TS.getNovoDeslocamento(nivel_lexico)}};
    TS.InsereSimbolo(var);
    num_amem++;
}

/* DECLARA_PROCEDIMENTO */
void beginProcedure() {
    flags("beginProcedure");
    int numero_params = 0;
    nivel_lexico++;

    // char novorot = novoRotuloc();
    // MEPA.JumpTo(getRotulo(novorot));
    MEPA.JumpTo(novoRotulo());

    Simbolo* proc{new Simbolo{meu_token, PROCEDURE, nivel_lexico, numero_params}};
    // proc->setRotulo(novorot);
    proc->setRotulo(novoRotuloc());
    TS.InsereSimbolo(proc);

    MEPA.ProcInit(getRotulo(), nivel_lexico);
}
void endProcedure() {
    if (stack_rotulos.empty()) {
        flags("GOTTEM");
    }
    char rotulo = stack_rotulos.back();

    MEPA.ProcEnd(nivel_lexico, 0);
    // removeForaEscopo();//  REMOVE NO FINAL DO COMANDO

    stack_rotulos.pop_back();
    nivel_lexico--;
}
void callProcedure() {
    // TODO empilha parametros
    Simbolo* proc = TS.BuscarSimbolo(proc_tk); /* GAMBI */

    if (proc == nullptr || proc->getCategoria() != Category::PROCEDURE) {
        std::cerr << "nao achou " << proc_tk << '\n';
        return;
    }

    char rotulo = proc->getRotulo();
    MEPA.CallProc(getRotulo(rotulo), nivel_lexico);
}

/* ATRIBUICAO */
void aplicaAtribuicao() { MEPA.Atribuicao(); }
void declaraIdentificador() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("symbol not found");

    if (simbolo->getCategoria() == Category::VARIAVEL_SIMPLES) {
        stack_tipos.push_back(simbolo->getTipo());
        addr_variavel = getAddrLex();
    }

    if (simbolo->getCategoria() == Category::PROCEDURE) {
        strncpy(proc_tk, meu_token, TAM_TOKEN); /* GAMBI */
    }
}  // salvarVarSimples

/* READ & WRITE */
void Read() { MEPA.Read(); }
void Write() { MEPA.Write(); }

/* IF_THEN_ELSE */
void endCondicional() { popRotulo(); }
void beginCondicional() {  // mesmo que bodyWhile analisaExpression
    MEPA.IfJumpTo(novoRotulo());
}
void elseCondicional() {
    MEPA.JumpTo(novoRotulo());
    popPenultRotulo();
}

/* WHILE */
void beginWhile() { MEPA.rotAddrNome(novoRotulo()); }
void endWhile() {
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

/* OPERACAO (EXPRESSAO) */
void aplicarOperacao(simbolos simbolo) {
    MEPA.Operacao(operations[simbolo].first, operations[simbolo].second);
}  // token MAIS/DIV/OR/...)

/* FATOR */
void saveVariavel() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("variable not found (" + std::string(meu_token) + ")");
    if (simbolo->getCategoria() == Category::VARIAVEL_SIMPLES) {
        stack_tipos.push_back(simbolo->getTipo());
    }
    MEPA.LoadFrom(getAddrLex());
}
void loadConstante(std::string valor) {
    if (valor == "true" || valor == "false") {
        stack_tipos.push_back(VariableType::BOOLEANO);
        MEPA.LoadValue(std::to_string(valor == "true"));
    } else {
        MEPA.LoadValue(valor);
        stack_tipos.push_back(VariableType::INTEIRO);
    }

}  // boolean true & false; else integer

void removeForaEscopo() {
    int num_vars = stack_mem.back();
    TS.RemoveSimbolos(num_vars);

    while (!TS.empty() && TS.getTopo()->getNivelLexico() - 1 >= nivel_lexico) {
        std::cerr << "TO REMOVE ";
        TS.getTopo()->show();
        // std::cerr << " WITH ROTULO " << getRotulo() << '\n';
        std::cerr << "ROTULO SIZE: " << stack_rotulos.size() << "\n";
        TS.show();
        TS.RemoveSimbolos(1);
        stack_rotulos.pop_back();
    }

    flags(std::to_string(nivel_lexico));
    // TS.RemoveProcedures(nivel_lexico);

    /* mudar pra remover procedure sse sai do escopo */
}

/* -------------------------------------------------------------------
 * funcoes auxiliares
 * ------------------------------------------------------------------- */

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
        error("undefined symbol (" + std::string(meu_token) + ")");
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
    rotulo << "R" << std::setfill('0') << std::setw(2) << (int)stack_rotulos.at(rot_size - 2);
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

char novoRotuloc() {
    if (num_rots >= 255) {
        error("stack overflow: stack_rotulo");
    }
    stack_rotulos.push_back(num_rots);
    num_rots = num_rots + 1;
    return num_rots - 1;
}

std::string getRotulo() { return getRotulo(stack_rotulos.back()); }

std::string getRotulo(char rotulo) {
    std::ostringstream format;
    format << "R" << std::setfill('0') << std::setw(2) << (int)rotulo;
    return format.str();
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
