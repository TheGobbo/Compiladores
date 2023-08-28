
#include "Mepa.hpp"

#include <fstream>
#include <iomanip>

#include "compilador.hpp"

static std::ofstream mepa_file;  // Para geracao de codigo MEPA
// std::deque<char> stack_rotulos;

namespace MEPA {
/* nao tem params */
const char* START = "\tINPP";
const char* FINISH = "\tPARA";

const char* READ = "\tLEIT";
const char* WRITE = "\tIMPR";

const char* MENOR = "\tCMME";
const char* MAIOR = "\tCMMA";
const char* IGUAL = "\tCMIG";
const char* MAIOR_IGUAL = "\tCMEG";
const char* MENOR_IGUAL = "\tCMAG";
const char* DIFF = "\tCMDG";

const char* SOMA = "\tSOMA";
const char* SUBT = "\tSUBT";
const char* DIV = "\tDIVI";
const char* MULT = "\tMULT";
const char* OR = "\tDISJ";
const char* AND = "\tCONJ";

/* tem parametros */
const char* ALLOC = "\tAMEM ";
const char* FREE = "\tDMEM ";
const char* STORE = "\tARMZ ";
const char* LOADV = "\tCRVL ";
const char* LOADC = "\tCRCT ";
const char* JUMP = "\tDSVS ";
const char* JUMP_SE = "\tDSVF ";

std::string ROTULO(std::string rotulo) { return rotulo + ": NADA"; }

}  // namespace MEPA
/* -------------------------------------------------------------------
 * MEPA STREAM
 * ------------------------------------------------------------------- */

void geraCodigo_() {
    mepa_file << '\n';
    mepa_file.flush();
}

/* geraCodigo(1, 2, 3, 4) prints "1, 2, 3, 4\n" */
template <typename T, typename... Types>
void geraCodigo_(T var1, Types... var2) {
    if (!mepa_file.is_open()) {
        mepa_file.open("MEPA");  // Open the file "MEPA" for writing
    }
    mepa_file << var1;

    // if constexpr (std::is_same_v<T, const char*>) {
    //     if (var1 && *var1 == 'R') mepa_file << ": ";
    // }

    mepa_file.flush();

    geraCodigo_(var2...);
}

void geraCodigoRead() {
    geraCodigo_(MEPA::READ);
    geraCodigo_(MEPA::STORE, getAddrLex());
}

void geraCodigoAtribuicao() {
    operaTiposValidos();
    geraCodigo_(MEPA::STORE + addr_variavel);
}

void geraCodigoEndWhile() {
    std::string begin, end;
    end = getRotulo();
    stack_rotulos.pop_back();

    begin = getRotulo();
    stack_rotulos.pop_back();

    geraCodigo_(MEPA::JUMP, begin);
    geraCodigo_(MEPA::ROTULO(end));
}

void jumpTopoSeFalso() { geraCodigo_(MEPA::JUMP_SE, novoRotulo()); }
void jumpTopoSempre() { geraCodigo_(MEPA::JUMP, novoRotulo()); }
void popRotulo() {
    std::size_t rot_size = stack_rotulos.size();
    if (rot_size < 1) {
        error("stack underflow on rotulos");
    }
    std::string topo = getRotulo();
    stack_rotulos.pop_back();

    geraCodigo_(MEPA::ROTULO(topo));
}

void popPenultRotulo() {
    std::size_t rot_size = stack_rotulos.size();
    if (rot_size < 2) {
        error("stack underflow on rotulos");
    }

    std::ostringstream rotulo;
    rotulo << "R" << std::setfill('0') << std::setw(3) << (int)stack_rotulos.at(rot_size - 2);
    geraCodigo_(MEPA::ROTULO(rotulo.str()));

    stack_rotulos.erase(stack_rotulos.end() - 2);
}

std::string novoRotulo() {
    if (num_rots >= 255) {
        error("stack overflow: stack_rotulo");
    }
    num_rots = num_rots + 1;
    stack_rotulos.push_back(num_rots);
    return getRotulo();
}

std::string getRotulo() {
    std::ostringstream rotulo;
    rotulo << "R" << std::setfill('0') << std::setw(3) << (int)stack_rotulos.back();
    return rotulo.str();
}

/*
int main() { geraCodigo_(MEPA::FREE, 3); }
*/