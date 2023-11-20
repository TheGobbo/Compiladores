#include "code_generation/Simbolo.hpp"

#include <iostream>
#include <stack>

/* BUILDER SETTERS */

// Constructor for Builder
Simbolo::Simbolo(const std::string& identificador, Category categoria,
                 int nivel_lexico)
    : identificador{identificador},
      categoria{categoria},
      nivel_lexico{nivel_lexico} {
    // Other attributes depending on the category
    this->passagem = PassageType::BY_UNDEFINED;
    this->tipo = VariableType::UNDEFINED;
    this->deslocamento = -999;

    this->rotulo = 'U';
    this->num_params = -999;
}

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

Simbolo& Simbolo::setParams(ParamFormal params) {
    this->params = params;
    return *this;
}

Simbolo& Simbolo::setPassagem(PassageType passagem) {
    this->passagem = passagem;
    return *this;
}

/* GETTERS */

std::string Simbolo::getIdentificador() const { return this->identificador; }

const ParamFormal& Simbolo::getParams() const { return this->params; }

std::string Simbolo::getAddr() const {
    if (this->deslocamento == -999) {
        std::cerr << "Invalide getAddr in Simbolo\n";
        std::exit(-1);
    }

    return std::to_string(this->nivel_lexico) + ", " +
           std::to_string(this->deslocamento);
}

Category Simbolo::getCategoria() const { return this->categoria; }

VariableType Simbolo::getTipo() const { return this->tipo; }

PassageType Simbolo::getPassage() const { return this->passagem; };

int Simbolo::getDeslocamento() const { return this->deslocamento; }

int Simbolo::getNivelLexico() const { return this->nivel_lexico; }

int Simbolo::getTamParams() const { return this->params.size(); }

char Simbolo::getRotulo() const { return this->rotulo; }

std::string Simbolo::showVariable(VariableType tipo) {
    const std::string variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    return variableTypeNames[tipo];
}

std::string Simbolo::showPassage(PassageType tipo) {
    const std::string passageTypeNames[] = {"BY_VALUE", "BY_REFERENCE",
                                            "BY_UNDEFINED"};
    return passageTypeNames[tipo];
}

std::string Simbolo::showCategory(Category tipo) {
    const std::string categoryNames[] = {"VARIAVEL_SIMPLES", "PARAMETRO_FORMAL",
                                         "PROCEDURE", "FUNCTION", "CTE"};

    return categoryNames[tipo];
}

void Simbolo::show() {
    // Map enums to strings
    const char* categoryNames[] = {"VARIAVEL_SIMPLES", "PARAMETRO_FORMAL",
                                   "PROCEDURE", "FUNCTION"};

    std::cout << "identificador[" << this->identificador << "] ";
    std::cout << "categoria[" << categoryNames[this->categoria] << "] ";
    std::cout << "nivel_lexico[" << this->nivel_lexico << "] ";
    if (this->categoria == VARIAVEL_SIMPLES || this->categoria == PARAMETRO_FORMAL) {
        std::cout << "desl[" << this->deslocamento << "] ";
        std::cout << "tipo[" << showVariable(this->tipo) << "] ";
        std::cout << "pass[" << showPassage(this->passagem) << "]\n";
    } else {
        std::cout << "Rotulo[" << (int)this->rotulo << "] ";
        if (this->categoria == FUNCTION) {
            std::cout << "desl[" << this->deslocamento << "] ";
            std::cout << "tipo[" << showVariable(this->tipo) << "] ";
        }
        std::cout << "N_Params[" << this->num_params << "] ";
        std::cout << "PARAMS: [";
        ParamFormal::iterator it;
        for (it = this->params.begin(); it != this->params.end(); ++it) {
            std::cout << " (" << showVariable((*it).first) << ", ";
            std::cout << showPassage((*it).second) << ")";
        }
        std::cout << "]\n";
    }
}
