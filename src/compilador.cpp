
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

#define flags(STR) std::cerr << RED << STR << NC << std::endl
#define print(STR) std::cerr << GREEN << STR << NC << std::endl

/* -------------------------------------------------------------------
 * Classe de rotulos
 * ------------------------------------------------------------------- */

Rotulos::Rotulos() : rotulo(0) {}

/* cria um novo rotulo e adiciona na stack */
Rotulos& Rotulos::push() {
    if (this->rotulo >= 255) {
        error("Stack rotulos overflow");
    }
    this->stack_rotulos.push_back(this->rotulo);
    print("adiciona rot " + std::to_string((int)this->rotulo));

    this->rotulo++;
    return *this;
}

/* adiciona o rotulo r na stack */
Rotulos& Rotulos::push(char r) {
    if (this->rotulo >= 255) {
        error("Stack rotulos overflow");
    }
    this->stack_rotulos.push_back(r);
    print("adiciona rot " + std::to_string((int)r));

    return *this;
}

/* remove o topo da stack */
Rotulos& Rotulos::pop() {
    if (this->stack_rotulos.empty()) {
        error("Stack rotulos is empty.");
    }
    flags("remove rot " + std::to_string((int)stack_rotulos.back()));
    this->stack_rotulos.pop_back();
    return *this;
}

/* retorna o topo da stack */
char Rotulos::top() const {
    if (this->stack_rotulos.empty()) {
        error("Stack rotulos is empty.");
    }
    return this->stack_rotulos.back();
}

// retorna o próximo rotulo, sem alterar a stack, mas aumentando a contagem
// char Rotulos::peek() { return this->rotulo++; }

/* transforma topo da stack em -> R003 */
std::string Rotulos::transformTop() const {
    if (this->stack_rotulos.empty()) {
        error("Stack rotulos is empty.");
    }
    char topChar = this->stack_rotulos.back();
    return this->transform(topChar);
}

/* transforma char rotulo em -> R003 */
std::string Rotulos::transform(char rotulo) const {
    std::ostringstream format;
    format << "R" << std::setw(2) << std::setfill('0') << (int)rotulo;
    return format.str();
}

/* retona o tamanho da stack */
int Rotulos::size() { return this->stack_rotulos.size(); }

/* imprime a stack na tela */
void Rotulos::show() {
    std::cerr << RED << "STACK_ROTULOS, nivel lexico : " << itoa(nivel_lexico) << std::endl;
    std::deque<char>::iterator it;
    for (it = this->stack_rotulos.begin(); it != this->stack_rotulos.end(); ++it) {
        std::cerr << ">>>>>>> R00" << (int)(*it) << std::endl;
    }
    std::cerr << "STACK_ROTULOS BACK\n" << NC << std::endl;
}

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

MepaInterface MEPA("output/output.mepa");

std::deque<VariableType> stack_tipos;
std::deque<int> stack_mem;
Rotulos stack_rotulos;

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
    stack_rotulos.show();
}

/* BLOCO */
void varsDeclarado() {
    MEPA.Alloc(num_amem);
    stack_mem.push_back(num_amem);

    stack_rotulos.push();
    MEPA.write_code("DSVS " + stack_rotulos.transformTop());

    num_amem = 0;
    print = true;
}
void subrotDeclarado() {
    // TS.show();
    MEPA.write_rotulo(stack_rotulos.top(), "NADA");
    stack_rotulos.pop();
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
    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico}};
    var->setDeslocamento(TS.getNovoDeslocamento(nivel_lexico));
    var->setTipo(UNDEFINED);

    TS.InsereSimbolo(var);
    num_amem++;
}

/* DECLARA_PROCEDIMENTO */
void beginProcedure() {
    int numero_params = 0;
    nivel_lexico++;

    Simbolo* proc{new Simbolo{meu_token, PROCEDURE, nivel_lexico}};
    proc->setNumParams(numero_params);
    proc->setRotulo(stack_rotulos.push().top());
    TS.InsereSimbolo(proc);

    MEPA.write_rotulo(stack_rotulos.top(), "ENPR " + itoa(nivel_lexico));
    stack_rotulos.pop();
}
void endProcedure() {
    MEPA.write_code("RTPR " + itoa(nivel_lexico, 0));
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
    MEPA.write_code("CHPR " + stack_rotulos.transform(rotulo) + ", " + itoa(nivel_lexico));
}

/* ATRIBUICAO */
void aplicaAtribuicao() {
    operaTiposValidos();
    MEPA.write_code("ARMZ " + addr_variavel);
}
void declaraIdentificador() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("symbol not found");

    if (simbolo->getCategoria() == Category::VARIAVEL_SIMPLES) {
        stack_tipos.push_back(simbolo->getTipo());
        addr_variavel = getAddrLex();
    }

    if (simbolo->getCategoria() == Category::PROCEDURE) {
        proc_tk = meu_token; /* GAMBI */
    }
}

/* READ & WRITE */
void Read() {
    MEPA.write_code("LEIT");
    MEPA.write_code("ARMZ " + getAddrLex());
}
void Write() { MEPA.write_code("IMPR"); }

/* IF_THEN_ELSE */
void endCondicional() {
    MEPA.write_rotulo(stack_rotulos.top(), "NADA");
    stack_rotulos.pop();
}
void beginCondicional() { MEPA.write_code("DSVF " + stack_rotulos.push().transformTop()); }

void elseCondicional() {
    MEPA.write_code("DSVS " + stack_rotulos.push().transformTop());
    std::size_t rot_size = stack_rotulos.size();

    char topo = stack_rotulos.top();
    stack_rotulos.pop();

    char subtop = stack_rotulos.top();
    stack_rotulos.pop();
    stack_rotulos.push(topo);

    MEPA.write_rotulo(subtop, "NADA");
}

/* WHILE */
void beginWhile() { MEPA.write_rotulo(stack_rotulos.push().top(), "NADA"); }

void endWhile() {
    char end = stack_rotulos.top();
    stack_rotulos.pop();

    std::string begin = stack_rotulos.transformTop();
    stack_rotulos.pop();

    MEPA.write_code("DSVS " + begin);
    MEPA.write_rotulo(end, "NADA");
}

/* OPERACAO (EXPRESSAO) */
// from CodeGenContext
void aplicarOperacao(const std::string& command, VariableType resultado) {
    //                    CodeGenerationContext& context) {
    MEPA.write_code(command);

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

    MEPA.write_code("CRVL " + getAddrLex());
}

// boolean true & false; senao eh integer
void loadConstante(std::string valor) {
    if (valor == "true" || valor == "false") {
        stack_tipos.push_back(VariableType::BOOLEANO);
        MEPA.write_code("CRCT " + itoa(valor == "true"));
    } else {
        MEPA.write_code("CRCT " + valor);
        stack_tipos.push_back(VariableType::INTEIRO);
    }
}

void removeForaEscopo() {
    int num_vars = stack_mem.back();
    TS.RemoveSimbolos(num_vars);

    while (!TS.empty() && TS.getTopo()->getNivelLexico() - 1 >= nivel_lexico) {
        TS.RemoveSimbolos(1);
    }
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

// espelho de bison::Parse::error
void error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line << '\n';
    exit(0);
}

void print_tipos() {
    const char* variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    print("stack_tipos : (front) ");
    for (auto it = stack_tipos.begin(); it != stack_tipos.end(); ++it) {
        if (it != stack_tipos.begin()) std::cout << ", ";
        std::cout << variableTypeNames[*it];
    }

    print(" (back)");
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
