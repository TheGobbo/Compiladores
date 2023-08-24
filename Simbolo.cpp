#include "Simbolo.hpp"

#include <cstring>
#include <iostream>
#include <stack>

// construtor
Simbolo::Simbolo(char (&identificador)[TAM_TOKEN], Category categoria, int8_t nivel_lexico,
                 int8_t deslocamento)
    : identificador{identificador},
      categoria{categoria},
      nivel_lexico{nivel_lexico},
      deslocamento{deslocamento} {}

// setters
void Simbolo::setTipo(VariableType tipo) { this->tipo = tipo; }

// getters
char (&Simbolo::getIdent())[TAM_TOKEN] { return this->identificador; }
VariableType Simbolo::getTipo() { return this->tipo; }
Category Simbolo::getCategoria() { return this->categoria; }
int8_t Simbolo::getNivelLexico() { return this->nivel_lexico; }
int8_t Simbolo::getDeslocamento() { return this->deslocamento; }

// Simbolo == Simbolo
bool Simbolo::operator==(Simbolo& other) {
    // return std::strncmp(this->identificador, other.identificador, TAM_TOKEN) == 0;
    return this->tipo == other.tipo;
}

bool Simbolo::valido() {
    // Check if 'tipo' is a valid VariableType
    if (this->tipo != INTEGER && this->tipo != UNDEFINED) {
        std::cerr << "Valor 'tipo' invalido." << std::endl;
        return false;
    }

    // Check if 'categoria' is a valid Category
    if (this->categoria != VARIAVEL_SIMPLES && this->categoria != PARAMETRO_FORMAL &&
        this->categoria != PROCEDURE) {
        std::cerr << "Valor 'categoria' Invalido." << std::endl;
        return false;
    }

    // Check if 'nivel_lexico' and 'deslocamento' are non-negative
    if (this->nivel_lexico < 0 || this->deslocamento < 0) {
        std::cerr << "'nivel_lexico' e 'deslocamento' devem ser nao-negativos." << std::endl;
        return false;
    }

    // All checks passed, the Simbolo is valid
    return true;
}

void Simbolo::show() {
    // Map enums to strings
    const char* categoryNames[] = {"VARIAVEL_SIMPLES", "PARAMETRO_FORMAL", "PROCEDURE"};
    const char* variableTypeNames[] = {"INTEGER", "UNDEFINED"};

    std::cout << "identificador[" << this->identificador << "] ";
    std::cout << "tipo[" << variableTypeNames[this->tipo] << "] ";
    std::cout << "categoria[" << categoryNames[this->categoria] << "] ";
    std::cout << "nivel_lexico[" << static_cast<int>(this->nivel_lexico) << "] ";
    std::cout << "deslocamento[" << static_cast<int>(this->deslocamento) << "]\n";
}

/*
// testes da classe
int main() {
    char id[TAM_TOKEN] = "example";
    char(&rid)[TAM_TOKEN]{id};
    Simbolo* symbol{new Simbolo{rid, VARIAVEL_SIMPLES, 0, 0}};

    symbol->show();

    symbol->setTipo(INTEGER);

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
