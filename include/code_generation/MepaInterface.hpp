#ifndef MEPA_INTERFACE_HPP
#define MEPA_INTERFACE_HPP

#include <deque>
#include <fstream>
#include <string>

#include "code_generation/Simbolo.hpp"

class MepaInterface {
   public:
    MepaInterface();
    MepaInterface(std::string mepa_file);

    void Start();
    void Finish();

    void Read();
    void Write();
    void Atribuicao();
    void JumpTo(std::string rotulo);
    void IfJumpTo(std::string rotulo);
    void rotAddrNome(std::string rotulo);
    void rotAddrNome(std::string rotulo, std::string nome);
    void LoadFrom(std::string addr_lex);
    void LoadValue(std::string value);

    void ProcInit(std::string rotulo, int nivel_lexico);
    void CallProc(std::string rotulo, int nivel_lexico);
    void ProcEnd(int nivel_lexico, int num_params);

    void Alloc(int num_alloc);
    void Free(int num_free);

    void Operacao(const char* operation);
    void Operacao(const char* operation, VariableType resultado);

    /* OPERACOES MEPA */
    /* https://homepages.dcc.ufmg.br/~bigonha/Cursos/comp-slides-p4.pdf */
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
    const char* NOT = "\tNEGA";

    /* tem parametros */
    const char* ALLOC = "\tAMEM ";
    const char* FREE = "\tDMEM ";
    const char* STORE = "\tARMZ ";
    const char* LOADV = "\tCRVL ";
    const char* LOADC = "\tCRCT ";
    const char* JUMP = "\tDSVS ";
    const char* IF_JUMP = "\tDSVF ";

   private:
    std::ofstream mepa_stream;
    std::string mepa_file;

    template <typename T, typename... Types>
    void geraCodigo_(T var1, Types... var2);
    void geraCodigo_();
};

#endif  // MEPA_INTERFACE_HPP
