#include "code_generation/MepaInterface.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "compilador.hpp"

/* -------------------------------------------------------------------
 * MEPA STREAM
 * ------------------------------------------------------------------- */

MepaInterface::MepaInterface(const std::string& outputFile) {
    // std::ofstream output(outputFile, std::ofstream::out);
    this->mepa_stream.open(outputFile, std::ofstream::out);
}

MepaInterface::~MepaInterface() {
    // std::cerr << "destruindo mepa\n";
    this->mepa_stream.close();
}

void MepaInterface::write_code(const std::string& command) {
    this->mepa_stream << '\t' << command << "\n";
    this->mepa_stream.flush();
}

void MepaInterface::write_rotulo(char rotulo, const std::string& nome) {
    std::ostringstream format;
    format << "R" << std::setfill('0') << std::setw(2) << (int)rotulo;
    this->mepa_stream << format.str() + ": " + nome << '\n';
    this->mepa_stream.flush();
}

/* -------------------------------------------------------------------
 * MEPA FUNCTIONS
 * ------------------------------------------------------------------- */

void MepaInterface::Alloc(int num_alloc) {
    if (num_alloc <= 0) return;
    write_code("AMEM " + std::to_string(num_alloc));
}

void MepaInterface::Free(int num_free) {
    if (num_free <= 0) return;
    write_code("DMEM " + std::to_string(num_free));
}
