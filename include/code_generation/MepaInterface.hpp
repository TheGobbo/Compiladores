#ifndef MEPA_INTERFACE_HPP
#define MEPA_INTERFACE_HPP

#include <deque>
#include <fstream>
#include <string>

#include "code_generation/Simbolo.hpp"

/* OPERACOES MEPA */
/* https://homepages.dcc.ufmg.br/~bigonha/Cursos/comp-slides-p4.pdf */

class MepaInterface {
   public:
    MepaInterface(const std::string& outputFile);
    ~MepaInterface();

    void write_code(const std::string& command);
    void write_rotulo(char rotulo, const std::string& nome);

    void Alloc(int num_alloc);
    void Free(int num_free);

   private:
    std::ofstream mepa_stream;
};

#endif  // MEPA_INTERFACE_HPP
