
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Eduardo Gobbo Willi Vasconcellos Goncalves
 *               Data: 08/2023
 *      Atualizado em: [22/09/2020]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include "compilador.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define NC "\033[0m"  // No color

#define flags(STR) std::cerr << RED << STR << NC
#define print(STR) std::cerr << GREEN << STR << NC
/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

MepaInterface MEPA("output/output.mepa");

std::deque<VariableType> stack_tipos;
std::deque<char> stack_rotulos;
std::deque<int> stack_mem;

TabelaSimbolos TS;
simbolos simbolo;

std::string meu_token;
std::string proc_tk; /* GAMBI */
int nivel_lexico = 0;

int num_line = 1;
int num_amem = 0;
char num_rots = 0;

std::string addr_variavel;

bool print = false;

/* -------------------------------------------------------------------
 * funcoes do Compilador
 * ------------------------------------------------------------------- */

// auxiliar //
void saveCurrentSimbolo() {}

/* PROGRAM */
void beginCompilador() { MEPA.write_code("INPP"); }
void endCompilador() {
    print("endCompilador");
    MEPA.write_code("PARA");
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
    flags("NL : " + itoa(nivel_lexico));
    std::deque<char>::iterator it;
    for (it = stack_rotulos.begin(); it != stack_rotulos.end(); ++it) {
        flags("subrot: " + getRotulo((*it)));
    }
    TS.show();

    if (stack_rotulos.size() == 1) {
        MEPA.write_rotulo(stack_rotulos.back(), "NADA");
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
    // Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico,
    //                          TS.getNovoDeslocamento(nivel_lexico)}};

    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico}};
    var->setDeslocamento(TS.getNovoDeslocamento(nivel_lexico));
    var->setTipo(UNDEFINED);

    TS.InsereSimbolo(var);
    num_amem++;
}

/* DECLARA_PROCEDIMENTO */
void beginProcedure() {
    flags("beginProcedure");
    int numero_params = 0;
    nivel_lexico++;

    MEPA.write_code("DSVS " + novoRotulo());

    Simbolo* proc{new Simbolo{meu_token, PROCEDURE, nivel_lexico}};
    proc->setNumParams(numero_params);
    proc->setRotulo(novoRotuloc());

    TS.InsereSimbolo(proc);

    MEPA.write_rotulo(stack_rotulos.back(), "ENPR " + itoa(nivel_lexico));
}
void endProcedure() {
    if (stack_rotulos.empty()) {
        flags("GOTTEM");
    }
    char rotulo = stack_rotulos.back();

    // MEPA.ProcEnd(nivel_lexico, 0);
    MEPA.write_code("RTPR " + itoa(nivel_lexico, 0));
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
    // MEPA.CallProc(getRotulo(rotulo), nivel_lexico);
    MEPA.write_code("CHPR " + getRotulo(rotulo));
}

/* ATRIBUICAO */
void aplicaAtribuicao() {
    operaTiposValidos();
    // MEPA.Atribuicao();
    MEPA.write_code("ARMZ " + addr_variavel);
}
void declaraIdentificador() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("symbol not found");

    if (simbolo->getCategoria() == Category::VARIAVEL_SIMPLES) {
        flags(simbolo->getTipo());
        stack_tipos.push_back(simbolo->getTipo());
        addr_variavel = getAddrLex();
    }

    if (simbolo->getCategoria() == Category::PROCEDURE) {
        proc_tk = meu_token; /* GAMBI */
    }
}  // salvarVarSimples

/* READ & WRITE */
void Read() {
    MEPA.write_code("LEIT");
    MEPA.write_code("ARMZ " + getAddrLex());
}
void Write() { MEPA.write_code("IMPR"); }

/* IF_THEN_ELSE */
void endCondicional() { popRotulo(); }
void beginCondicional() {  // mesmo que bodyWhile analisaExpression
    // MEPA.IfJumpTo(novoRotulo());
    MEPA.write_code("DSVF " + novoRotulo());
}
void elseCondicional() {
    // MEPA.JumpTo(novoRotulo());
    MEPA.write_code("DSVS " + novoRotulo());
    popPenultRotulo();
}

/* WHILE */
void beginWhile() {
    // MEPA.rotAddrNome(novoRotulo());
    MEPA.write_rotulo(novoRotuloc(), "NADA");
}
void endWhile() {
    char end = stack_rotulos.back();
    stack_rotulos.pop_back();

    std::string begin = getRotulo();
    stack_rotulos.pop_back();

    MEPA.write_code("DSVS " + begin);
    MEPA.write_rotulo(end, "NADA");
}

/* OPERACAO (EXPRESSAO) */
// from CodeGenContext
void aplicarOperacao(const std::string& command, VariableType resultado) {
    //                    CodeGenerationContext& context) {
    MEPA.write_code(command);
    flags(command);

    stack_tipos.pop_back();
    stack_tipos.pop_back();

    if (resultado != VariableType::UNDEFINED) {
        stack_tipos.push_back(resultado);
    }
}

/* FATOR */
void saveVariavel() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("variable not found (" + std::string(meu_token) + ")");
    if (simbolo->getCategoria() == Category::VARIAVEL_SIMPLES) {
        stack_tipos.push_back(simbolo->getTipo());
    }
    // MEPA.LoadFrom(getAddrLex());
    MEPA.write_code("CRVL " + getAddrLex());
}
void loadConstante(std::string valor) {
    if (valor == "true" || valor == "false") {
        stack_tipos.push_back(VariableType::BOOLEANO);
        MEPA.write_code("CRCT " + itoa(valor == "true"));
    } else {
        MEPA.write_code("CRCT " + valor);
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

    flags(itoa(nivel_lexico));
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
    if (simbolo == nullptr || simbolo->getCategoria() == Category::PROCEDURE) {
        error("undefined symbol (" + std::string(meu_token) + ")");
    }

    return itoa(simbolo->getNivelLexico(), simbolo->getDeslocamento());
}

void popPenultRotulo() {
    std::size_t rot_size = stack_rotulos.size();
    if (rot_size < 2) {
        error("stack underflow on rotulos");
    }

    char topo = stack_rotulos.back();
    stack_rotulos.pop_back();

    char subtop = stack_rotulos.back();
    stack_rotulos.pop_back();
    stack_rotulos.push_back(topo);

    MEPA.write_rotulo(subtop, "NADA");
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

    MEPA.write_rotulo(stack_rotulos.back(), "NADA");
    stack_rotulos.pop_back();
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

const std::string& itoa(int arg1) {
    static std::string result;
    result = std::to_string(arg1);
    return result;
}

const std::string& itoa(int arg1, int arg2) {
    static std::string result;
    result = std::to_string(arg1) + ", " + std::to_string(arg2);
    return result;
}
