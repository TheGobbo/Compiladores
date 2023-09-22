
#include "MepaInterface.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "compilador.hpp"

/* -------------------------------------------------------------------
 * MEPA STREAM
 * ------------------------------------------------------------------- */

MepaInterface::MepaInterface() : mepa_file{"MEPA"} { this->mepa_stream.open(this->mepa_file); }

MepaInterface::MepaInterface(std::string mepa_file) : mepa_file{mepa_file} {
    this->mepa_stream.open(this->mepa_file);
}

void MepaInterface::geraCodigo_() {
    this->mepa_stream << '\n';
    this->mepa_stream.flush();
}

/* geraCodigo(1, 2, 3, 4) prints "1, 2, 3, 4\n" */
template <typename T, typename... Types>
void MepaInterface::geraCodigo_(T var1, Types... var2) {
    if (!this->mepa_stream.is_open()) {
        this->mepa_stream.open(this->mepa_file);  // Open the file "MEPA" for writing
    }
    this->mepa_stream << var1;

    // std::cout << var1 << '\n';
    // if constexpr (std::is_same_v<T, const char*>) {
    //     if (var1 && *var1 == 'R') this->mepa_stream << ": ";
    // }

    this->mepa_stream.flush();

    geraCodigo_(var2...);
}

/* -------------------------------------------------------------------
 * MEPA FUNCTIONS
 * ------------------------------------------------------------------- */

void MepaInterface::Start() { geraCodigo_(this->START); }

void MepaInterface::Finish() { geraCodigo_(this->FINISH); }

void MepaInterface::Read() {
    geraCodigo_(this->READ);
    geraCodigo_(this->STORE, getAddrLex());
}

void MepaInterface::Write() { geraCodigo_(this->WRITE); }

void MepaInterface::Atribuicao() {
    operaTiposValidos();
    geraCodigo_(this->STORE + addr_variavel);
}

void MepaInterface::JumpTo(std::string rotulo) { geraCodigo_(this->JUMP, rotulo); }

void MepaInterface::IfJumpTo(std::string rotulo) { geraCodigo_(this->IF_JUMP, rotulo); }

/* rotAddrNome(ROTULO, NOME) -> "rotulo: nome", SE nome nao eh dado, "rotulo: NADA" */
void MepaInterface::rotAddrNome(std::string rotulo, std::string nome) {
    geraCodigo_(rotulo, ": ", nome);
}

/* rotAddrNome(ROTULO, NOME) -> "rotulo: nome", SE nome nao eh dado, "rotulo: NADA" */
void MepaInterface::rotAddrNome(std::string rotulo) { geraCodigo_(rotulo, ": NADA"); }

void MepaInterface::LoadFrom(std::string addr_lex) { geraCodigo_(this->LOADV, addr_lex); }

void MepaInterface::LoadValue(std::string value) { geraCodigo_(this->LOADC, value); }

void MepaInterface::Alloc(int num_alloc) {
    if (num_alloc <= 0) return;
    geraCodigo_(this->ALLOC, num_alloc);
}

void MepaInterface::Free(int num_free) {
    if (num_free <= 0) return;
    geraCodigo_(this->FREE, num_free);
}

void MepaInterface::ProcInit(std::string rotulo, int nivel_lexico) {
    geraCodigo_(rotulo, ": ENPR ", nivel_lexico);
}

void MepaInterface::CallProc(std::string rotulo, int nivel_lexico) {
    /* callProc rotulo, nivel_lexico  */
    geraCodigo_("\tCHPR ", rotulo);  // , ", ", nivel_lexico); GAMBI?
}

void MepaInterface::ProcEnd(int nivel_lexico, int num_params) {
    geraCodigo_("\tRTPR ", nivel_lexico, ", ", num_params);
}

/* Mepa.Operacao(Mepa.DIFF) e stack_tipos.push_back(resultado) */
void MepaInterface::Operacao(const char* operation, VariableType resultado) {
    operaTiposValidos();
    geraCodigo_(operation);
    stack_tipos.push_back(resultado);
}

/* Mepa.Operacao(Mepa.DIFF) Mepa.SOMA */
void MepaInterface::Operacao(const char* operation) {
    operaTiposValidos();
    geraCodigo_(operation);
}

/*
int main() { geraCodigo_(MEPA::FREE, 3); }
*/