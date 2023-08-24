#ifndef SIMBOLO_HPP
#define SIMBOLO_HPP

#include <cstdint>
#include <string>

#define TAM_TOKEN 16

// declarar dpois std::stack<Simbolo> tabelaDeSimbolos;
typedef enum { VARIAVEL_SIMPLES, PARAMETRO_FORMAL, PROCEDURE } Category;
typedef enum { BY_VALUE, BY_REFERENCE } PassageType;  // for formal parameters
typedef enum { INTEGER, UNDEFINED } VariableType;

class Simbolo {
   public:
    Simbolo(char (&identificador)[TAM_TOKEN], Category categoria, int8_t nivel_lexico,
            int8_t deslocamento);

    void setTipo(VariableType tipo);

    // somente getters, proibe alteracao
    char (&getIdent())[TAM_TOKEN];
    VariableType getTipo();
    Category getCategoria();
    int8_t getNivelLexico();
    int8_t getDeslocamento();

    bool valido();
    bool operator==(Simbolo& other);

    void show();

   private:
    char (&identificador)[TAM_TOKEN];  // constant from start
    VariableType tipo{UNDEFINED};      // constant later
    Category categoria;                // constant from start
    int8_t nivel_lexico{0};            // constant from start
    int8_t deslocamento{0};            // constant from start
    // todo: atributos variaveis [params, ...]
};

#endif