#ifndef SIMBOLO_HPP
#define SIMBOLO_HPP

#include <string>

#define TAM_TOKEN 16

// declarar dpois std::stack<Simbolo> tabelaDeSimbolos;
typedef enum { VARIAVEL_SIMPLES, PARAMETRO_FORMAL, PROCEDURE } Category;
typedef enum { BY_VALUE, BY_REFERENCE } PassageType;  // for formal parameters
typedef enum { INTEIRO, BOOLEANO, UNDEFINED } VariableType;

class Simbolo {
   public:
    Simbolo(char* identificador, Category categoria, int nivel_lexico, int deslocamento);

    void setTipo(VariableType tipo);

    // somente getters, proibe alteracao
    char* getIdent();
    VariableType getTipo();
    Category getCategoria();
    int getNivelLexico();
    int getDeslocamento();

    bool valido();
    bool operator==(Simbolo& other);

    void show();

   private:
    char identificador[TAM_TOKEN];  // constant from start
    VariableType tipo{UNDEFINED};   // constant later
    Category categoria;             // constant from start
    int nivel_lexico{0};            // constant from start
    int deslocamento{0};            // constant from start
    // todo: atributos variaveis [params, ...]
};

#endif