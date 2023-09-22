#include "code_generation/Simbolo.hpp"

#include <cstring>
#include <iostream>
#include <stack>

#include "compilador.hpp"

// construtor
Simbolo::Simbolo(char* identificador, Category categoria, int nivel_lexico, int numero)
    : categoria{categoria}, nivel_lexico{nivel_lexico}, numero{numero} {
    strncpy(this->identificador, identificador, TAM_TOKEN);
}

// setters
void Simbolo::setTipo(VariableType tipo) { this->tipo = tipo; }

void Simbolo::setRotulo(char rotulo) { this->rotulo = rotulo; }

// getters
char* Simbolo::getIdent() { return this->identificador; }

VariableType Simbolo::getTipo() { return this->tipo; }

Category Simbolo::getCategoria() { return this->categoria; }

int Simbolo::getNivelLexico() { return this->nivel_lexico; }

int Simbolo::getDeslocamento() { return this->numero; }

int Simbolo::getNumParamFormal() { return this->numero; }

char Simbolo::getRotulo() { return this->rotulo; }

// Simbolo == Simbolo
// bool Simbolo::operator==(Simbolo& other) {
//     // return std::strncmp(this->identificador, other.identificador, TAM_TOKEN) == 0;
//     return this->tipo == other.tipo;
// }

bool Simbolo::valido() {
    // Check if 'tipo' is a valid VariableType
    if (this->tipo != INTEIRO && this->tipo != UNDEFINED && this->tipo != BOOLEANO) {
        std::cerr << "Valor 'tipo' invalido." << std::endl;
        return false;
    }

    // Check if 'categoria' is a valid Category
    if (this->categoria != VARIAVEL_SIMPLES && this->categoria != PARAMETRO_FORMAL &&
        this->categoria != PROCEDURE) {
        std::cerr << "Valor 'categoria' Invalido." << std::endl;
        return false;
    }

    // Check if 'nivel_lexico' and 'numero' are non-negative
    if (this->nivel_lexico < 0 || this->numero < 0) {
        std::cerr << "'nivel_lexico' e 'numero' devem ser nao-negativos." << std::endl;
        return false;
    }

    // All checks passed, the Simbolo is valid
    return true;
}

void Simbolo::show() {
    // Map enums to strings
    const char* categoryNames[] = {"VARIAVEL_SIMPLES", "PARAMETRO_FORMAL", "PROCEDURE"};
    const char* variableTypeNames[] = {"INTEIRO", "BOOLEANO", "UNDEFINED"};
    const char* passageTypeNames[] = {"BY_VALUE", "BY_REFERENCE"};

    std::cout << "identificador[" << this->identificador << "] ";
    std::cout << "categoria[" << categoryNames[this->categoria] << "] ";
    std::cout << "nivel_lexico[" << static_cast<int>(this->nivel_lexico) << "] ";
    if (this->categoria == VARIAVEL_SIMPLES || this->categoria == PARAMETRO_FORMAL) {
        std::cout << "numero[" << static_cast<int>(this->numero) << "]\n";
    } else {
        std::cout << "Rotulo[" << (int)this->rotulo << "] ";
        std::cout << "N_Params[" << this->numero << "] ";
        std::cout << "PARAMS: [";
        ParamFormal::iterator it;
        for (it = this->param_formais.begin(); it != this->param_formais.end(); ++it) {
            std::cout << " (" << passageTypeNames[(*it).first] << ", ";
        }
        std::cout << "]\n";
    }
}

/*
// testes da classe
int main() {
    char id[TAM_TOKEN] = "example";
    char(&rid)[TAM_TOKEN]{id};
    Simbolo* symbol{new Simbolo{rid, VARIAVEL_SIMPLES, 0, 0}};

    symbol->show();

    symbol->setTipo(INTEIRO);

    symbol->show();

    delete symbol;

    char id1[TAM_TOKEN] = "a123";
    char id2[TAM_TOKEN] = "a123";
    char id3[TAM_TOKEN] = "bc";

    char(&rid1)[TAM_TOKEN]{id1};
    char(&rid2)[TAM_TOKEN]{id2};
    char(&rid3)[TAM_TOKEN]{id3};

    Simbolo s1{rid1, VARIAVEL_SIMPLES, 0, 1};
    Simbolo s2{rid2, VARIAVEL_SIMPLES, 2, 3};
    Simbolo s3{rid3, PARAMETRO_FORMAL, 4, 5};

    if (s1 == s2) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }

    if (s1 == s3) {
        std::cout << "FAIL" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }

    if (s2 == s3) {
        std::cout << "FAIL" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }

    if (s3 == s2) {
        std::cout << "FAIL" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }
}
*/
