#ifndef MEPA_HPP
#define MEPA_HPP

#include "compilador.hpp"

/* https://homepages.dcc.ufmg.br/~bigonha/Cursos/comp-slides-p4.pdf */
namespace MEPA {
extern const char* START;
extern const char* FINISH;

extern const char* READ;
extern const char* WRITE;

extern const char* MENOR;
extern const char* MAIOR;
extern const char* IGUAL;
extern const char* MAIOR_IGUAL;
extern const char* MENOR_IGUAL;
extern const char* DIFF;

extern const char* SOMA;
extern const char* SUBT;
extern const char* DIV;
extern const char* MULT;
extern const char* OR;
extern const char* AND;

/* tem parametros */
extern const char* ALLOC;
extern const char* FREE;
extern const char* STORE;
extern const char* LOADV;
extern const char* LOADC;
extern const char* JUMP;
extern const char* JUMP_SE;

std::string ROTULO(std::string rotulo);
}  // namespace MEPA

template <typename T, typename... Types>
void geraCodigo_(T var1, Types... var2);

void geraCodigoRead();
void geraCodigoAtribuicao();

void geraCodigoEndWhile();

void jumpTopoSeFalso();
void jumpTopoSempre();
void popRotulo();

std::string getAddrLex();
std::string novoRotulo();
std::string getRotulo();
void popPenultRotulo();

#endif  // MEPA_HPP
