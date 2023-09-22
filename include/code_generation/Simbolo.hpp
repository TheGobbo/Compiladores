#ifndef SIMBOLO_HPP
#define SIMBOLO_HPP

#include <forward_list>
#include <string>

#define TAM_TOKEN 16

// declarar dpois std::stack<Simbolo> tabelaDeSimbolos;
typedef enum { VARIAVEL_SIMPLES, PARAMETRO_FORMAL, PROCEDURE } Category;
typedef enum { BY_VALUE, BY_REFERENCE } PassageType;  // for formal parameters
typedef enum { INTEIRO, BOOLEANO, UNDEFINED } VariableType;

typedef std::forward_list<std::pair<VariableType, PassageType>> ParamFormal;

// struct detalhes_proc {
//     /* proc */
//     char rotulo;                // constant from start Rotulo: R000, R001, ... , R255
//     ParamFormal param_formais;  // (p1, var p2, p3, ...)
// };

// struct detalhes_vars {
//     VariableType tipo; /* var simples */
//     PassageType pass;  /* param formal */
// };

// union DetalhesSimb {
//     struct detalhes_proc proc;
//     struct detalhes_vars vars;
// };

class Simbolo {
   public:
    Simbolo(char* identificador, Category categoria, int nivel_lexico, int deslocamento);

    void setTipo(VariableType tipo);
    void setRotulo(char rotulo);

    // somente getters, proibe alteracao
    char* getIdent();
    VariableType getTipo();
    Category getCategoria();
    int getNivelLexico();
    int getDeslocamento();
    int getNumParamFormal();
    char getRotulo();

    bool valido();
    // bool operator==(Simbolo& other);

    void show();

   private:
    char identificador[TAM_TOKEN];  // constant from start
    Category categoria;             // constant from start
    int nivel_lexico;               // constant from start
    int numero;                     // constant from start (deslocamento ou num_param_formais)

    // DetalhesSimb detalhes;  // dependente da categoria
    VariableType tipo;          /* var simples */
    PassageType pass;           /* param formal */
    char rotulo;                // constant from start Rotulo: R000, R001, ... , R255
    ParamFormal param_formais;  // (p1, var p2, p3, ...)
};

#endif