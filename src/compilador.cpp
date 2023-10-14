
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
Simbolo* last_simb;
// ParamFormal last_proc;

int nivel_lexico = 0;
int num_line = 1;
int num_amem = 0;
char num_params = 0;
bool pf_ref = false;
bool chamada_proc = false;
int idx_params = 0;
Simbolo* proc_atual = nullptr;

bool print = false;

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
    std::cerr << RED << "STACK_ROTULOS, nivel lexico : " << itoa(nivel_lexico)
              << std::endl;
    std::deque<char>::iterator it;
    for (it = this->stack_rotulos.begin(); it != this->stack_rotulos.end(); ++it) {
        std::cerr << ">>>>>>> R00" << (int)(*it) << std::endl;
    }
    std::cerr << "STACK_ROTULOS BACK\n" << NC << std::endl;
}

/* -------------------------------------------------------------------
 * funcoes do Compilador
 * ------------------------------------------------------------------- */

/* PROGRAM */

void beginCompilador() { MEPA.write_code("INPP"); }

// remove todos os simbolos na tabela de simbolos
void endCompilador() {
    print("endCompilador");
    MEPA.write_code("PARA");
    TS.clear();
    TS.show();
    stack_rotulos.show();
}

/* BLOCO */

// Escreve alocacao de memoria na mepa e salva na stack_mem
// ???? DSVS nn lembro pq isso ta aqui, procedures acho?
void varsDeclarado() {
    MEPA.Alloc(num_amem);
    stack_mem.push_back(num_amem);

    stack_rotulos.push();
    MEPA.write_code("DSVS " + stack_rotulos.transformTop());

    num_amem = 0;
    print = true;
}

// escreve o rotulo no topo da stack e o remove
void subrotDeclarado() {
    MEPA.write_rotulo(stack_rotulos.top(), "NADA");
    stack_rotulos.pop();
}

// remove simbolos fora do escopo, libera memoria destes simbolos
void endComandos() {
    removeForaEscopo();
    MEPA.Free(stack_mem.back());
    stack_mem.pop_back();
}

/* DECLARA_VARS & LISTA_ID_VAR */

// configura os N ultimos simbolos cujo tipo eh undefined com o simbolo corrente
void aplicarTipos() {
    VariableType tipo = simbolo == simb_integer   ? VariableType::INTEIRO
                        : simbolo == simb_boolean ? VariableType::BOOLEANO
                                                  : VariableType::UNDEFINED;
    if (tipo == UNDEFINED) {
        error("undefined type");
    }

    TS.setTipos(tipo);
}

// declaracao de um novo simbolo
void novoSimbolo() {
    flags("NOVO SIMB");
    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico}};
    var->setDeslocamento(TS.getNovoDeslocamento(nivel_lexico));
    var->setTipo(VariableType::UNDEFINED);
    var->setPassagem(PassageType::BY_VALUE);

    TS.InsereSimbolo(var);
    num_amem++;
}

/* DECLARA_PROCEDIMENTO */

// declaracao de um novo procedimento
void beginProcedure() {
    nivel_lexico++;

    Simbolo* proc{new Simbolo{meu_token, PROCEDURE, nivel_lexico}};
    proc->setNumParams(0);
    proc->setRotulo(stack_rotulos.push().top());
    TS.InsereSimbolo(proc);

    MEPA.write_rotulo(stack_rotulos.top(), "ENPR " + itoa(nivel_lexico));
    stack_rotulos.pop();
}

void endProcedure() {
    MEPA.write_code("RTPR " + itoa(nivel_lexico, num_params));
    nivel_lexico--;
    num_params = 0;
}

// chama procedure com ultimo IDENT de procedure salvo
void callProcedure() {
    Simbolo* proc = TS.BuscarSimbolo(proc_tk); /* GAMBI */

    if (proc == nullptr || proc->getCategoria() != Category::PROCEDURE) {
        std::cerr << "nao achou procedure " << proc_tk << '\n';
        return;
    }

    char rotulo = proc->getRotulo();
    MEPA.write_code("CHPR " + stack_rotulos.transform(rotulo) + ", " +
                    itoa(nivel_lexico));

    // if (proc->getTamParams() != num_params) {
    //     flags(std::to_string(proc->getTamParams());
    //     flags(std::to_string(num_params);
    //     error("Wrong number of parameters for function <" + proc_tk + ">");
    // }

    chamada_proc = false;
}

/* DECLARA_FUNCAO */
void beginFunction() {
    nivel_lexico++;

    Simbolo* func{new Simbolo{meu_token, FUNCTION, nivel_lexico}};
    func->setNumParams(0);
    func->setRotulo(stack_rotulos.push().top());
    TS.InsereSimbolo(func);

    MEPA.write_rotulo(stack_rotulos.top(), "ENPR " + itoa(nivel_lexico));
    stack_rotulos.pop();
}

void endFunction() {
    MEPA.write_code("RTPR " + itoa(nivel_lexico, num_params));
    nivel_lexico--;
    num_params = 0;
}
void callFunction() {
    Simbolo* proc = TS.BuscarSimbolo(proc_tk); /* GAMBI */

    if (proc == nullptr || proc->getCategoria() != Category::FUNCTION) {
        // std::cerr << "nao achou procedure " << proc_tk << '\n';
        flags("&&&&&& Nao achou funcao <" + meu_token + ">");
        return;
    }

    // if (proc->getTamParams() != num_params) {
    //     flags(std::to_string(proc->getTamParams());
    //     flags(std::to_string(num_params);
    //     error("Wrong number of parameters for function <" + proc_tk + ">");
    // }

    char rotulo = proc->getRotulo();
    MEPA.write_code("CHPR " + stack_rotulos.transform(rotulo) + ", " +
                    itoa(nivel_lexico));

    chamada_proc = false;
}

/* PARAMETROS FORMAIS */
// define tipo de passagem dos params formais encontrados
void paramFormal() {
    Simbolo* var{new Simbolo{meu_token, PARAMETRO_FORMAL, nivel_lexico}};
    var->setTipo(VariableType::UNDEFINED);

    if (pf_ref) {
        var->setPassagem(PassageType::BY_REFERENCE);
    } else {
        var->setPassagem(PassageType::BY_VALUE);
    }

    TS.InsereSimbolo(var);
}

// configura infos dos params formais
void fimParamFormal() {
    num_params = TS.setParamFormal();
    TS.show();
}

/* ATRIBUICAO */
// carrega valor atribuido ao simbolo na mepa, valida tipos envolvidos
void aplicaAtribuicao() {
    operaTiposValidos();
    armazenaValor(last_simb);
    stack_tipos.clear();
}

// ao encontrar IDENT, define como tratar se for variavel ou procedure
void declaraIdentificador() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("symbol not found");

    if (simbolo->getCategoria() != Category::PROCEDURE) {
        stack_tipos.push_back(simbolo->getTipo());
        last_simb = simbolo;
    }

    if (simbolo->getCategoria() == Category::PROCEDURE ||
        simbolo->getCategoria() == FUNCTION) {
        proc_tk = meu_token; /* GAMBI */
        proc_atual = simbolo;
    }
}

/* READ & WRITE */
void Read() {
    MEPA.write_code("LEIT");
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (!simbolo) error("read symbol not found: " + meu_token);
    armazenaValor(simbolo);
}
void Write() { MEPA.write_code("IMPR"); }

/* IF_THEN_ELSE */
void endCondicional() {
    MEPA.write_rotulo(stack_rotulos.top(), "NADA");
    stack_rotulos.pop();
}
void beginCondicional() {
    MEPA.write_code("DSVF " + stack_rotulos.push().transformTop());
}

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
// interface para escrever operacoes mepa
void aplicarOperacao(const std::string& command, VariableType resultado) {
    MEPA.write_code(command);

    stack_tipos.pop_back();
    stack_tipos.pop_back();

    if (resultado != VariableType::UNDEFINED) {
        stack_tipos.push_back(resultado);
    }
}

/* FATOR */
// Encontra simbolo do token corrente e carrega seu valor na mepa
void saveVariavel() {
    Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
    if (simbolo == nullptr) error("variable not found (" + std::string(meu_token) + ")");
    if (simbolo->getCategoria() != Category::PROCEDURE) {
        stack_tipos.push_back(simbolo->getTipo());
    }

    carregaValor(simbolo);
}

// trata carregamento de constante boolean e integer
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

// remove da tabela de simbolos os simbolos fora do escopo atual em t_compilacao
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

// escolhe operacao de carrecar simbolo na mepa
void carregaValor(Simbolo* simbolo) {
    if (simbolo->getCategoria() == FUNCTION) {
        MEPA.write_code("AMEM 1");
        return;
    }
    int loadType = getLoadType(simbolo, proc_atual);
    switch (loadType) {
        case 1:
            flags("CRVL : " + meu_token);
            MEPA.write_code("CRVL " + simbolo->getAddr());
            break;
        case 2:
            flags("CRVI : " + meu_token);
            MEPA.write_code("CRVI " + simbolo->getAddr());
            break;
        case 3:
            flags("CREN : " + meu_token);
            MEPA.write_code("CREN " + simbolo->getAddr());
            break;
    }
}

// define logica de como carregar simbolo, subrotina = procedure;function;
// 1 = CRVL ; 2 = CRVI ; 3 = CREN
int getLoadType(Simbolo* simbolo, Simbolo* subrotina) {
    PassageType ps_simbolo, ps_formal, by_ref, by_val;

    by_ref = PassageType::BY_REFERENCE;
    by_val = PassageType::BY_VALUE;
    ps_simbolo = simbolo->getPassage();

    if (!chamada_proc || !subrotina || subrotina->getParams().size() == 0) {
        return ps_simbolo == by_ref ? 2 : 1;
    }

    ps_formal = proc_atual->getParams().at(idx_params).second;

    // addr & addr ; val & val ;
    if (ps_formal == ps_simbolo) {
        return 1;  // CRVL
    }

    // ps_simb = ref
    if (ps_formal == PassageType::BY_VALUE) {
        return 2;  // CRVI
    }

    // ps_formal = ref
    if (ps_simbolo == PassageType::BY_VALUE) {
        return 3;  // CREN
    }

    error("Nenhum tipo de load especificado!");
    return -1;
}

// decide entre ARMZ e ARMI para armazenar simbolo
void armazenaValor(Simbolo* simbolo) {
    if (simbolo->getPassage() == PassageType::BY_VALUE ||
        simbolo->getCategoria() == Category::FUNCTION) {
        flags("ARMZ!!!!!!");
        MEPA.write_code("ARMZ " + simbolo->getAddr());
    } else {
        flags("ARMI!!!!!!");
        MEPA.write_code("ARMI " + simbolo->getAddr());
    }
}

// valida operacao de tipos, nao adiciona nada na stack de tipos
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

// valida operacao de tipos e adicionar resultado na stack de tipos
void operaTiposValidos(VariableType resultado) {
    operaTiposValidos();
    stack_tipos.push_back(resultado);
    print_tipos();
}

// espelho de bison::Parse::error
void error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line << '\n';
    exit(0);
}

// util para visualizar stack_tipos durante compilacao
void print_tipos() {
    const char* variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    print("stack_tipos : (front) ");
    for (auto it = stack_tipos.begin(); it != stack_tipos.end(); ++it) {
        if (it != stack_tipos.begin()) std::cout << ", ";
        std::cout << variableTypeNames[*it];
    }

    print(" (back)");
}

// util que transforma int em addr string pra mepa
const std::string& itoa(int arg1) {
    static std::string result;
    result = std::to_string(arg1);
    return result;
}

// util que transforma dois ints em addr string pra mepa
const std::string& itoa(int arg1, int arg2) {
    static std::string result;
    result = std::to_string(arg1) + ", " + std::to_string(arg2);
    return result;
}
