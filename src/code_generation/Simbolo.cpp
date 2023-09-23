#include "code_generation/Simbolo.hpp"

#include <iostream>
#include <stack>

/* BUILDER SETTERS */

// Constructor for Builder
Simbolo::Simbolo(const std::string& identificador, Category categoria, int nivel_lexico)
    : identificador{identificador}, categoria{categoria}, nivel_lexico{nivel_lexico} {}

// Setters for additional attributes based on the category
Simbolo& Simbolo::setTipo(VariableType tipo) {
    this->tipo = tipo;
    return *this;
}

Simbolo& Simbolo::setDeslocamento(int deslocamento) {
    this->deslocamento = deslocamento;
    return *this;
}

Simbolo& Simbolo::setRotulo(char rotulo) {
    this->rotulo = rotulo;
    return *this;
}

Simbolo& Simbolo::setNumParams(int num_params) {
    this->num_params = num_params;
    return *this;
}

Simbolo& Simbolo::addParam(VariableType tipo, PassageType passagem) {
    this->params.push_back(std::make_pair(tipo, passagem));
    return *this;
}

Simbolo& Simbolo::setPassagem(PassageType passagem) {
    this->passagem = passagem;
    return *this;
}

/* GETTERS */

std::string Simbolo::getIdentificador() const { return this->identificador; }

const ParamFormal& Simbolo::getParams() const { return this->params; }

Category Simbolo::getCategoria() const { return this->categoria; }

VariableType Simbolo::getTipo() const { return this->tipo; }

int Simbolo::getDeslocamento() const { return this->deslocamento; }

int Simbolo::getNivelLexico() const { return this->nivel_lexico; }

int Simbolo::getTamParams() const { return this->params.size(); }

char Simbolo::getRotulo() const { return this->rotulo; }

void Simbolo::show() {
    // Map enums to strings
    const char* categoryNames[] = {"VARIAVEL_SIMPLES", "PARAMETRO_FORMAL", "PROCEDURE"};
    const char* variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    const char* passageTypeNames[] = {"BY_VALUE", "BY_REFERENCE"};

    std::cout << "identificador[" << this->identificador << "] ";
    std::cout << "categoria[" << categoryNames[this->categoria] << "] ";
    std::cout << "nivel_lexico[" << this->nivel_lexico << "] ";
    if (this->categoria == VARIAVEL_SIMPLES || this->categoria == PARAMETRO_FORMAL) {
        std::cout << "desl[" << this->deslocamento << "] ";
        std::cout << "tipo[" << variableTypeNames[this->tipo] << "]\n";
    } else {
        std::cout << "Rotulo[" << (int)this->rotulo << "] ";
        std::cout << "N_Params[" << this->num_params << "] ";
        std::cout << "PARAMS: [";
        ParamFormal::iterator it;
        for (it = this->params.begin(); it != this->params.end(); ++it) {
            std::cout << " (" << passageTypeNames[(*it).first] << ", ";
        }
        std::cout << "]\n";
    }
}
