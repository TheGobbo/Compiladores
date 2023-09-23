#ifndef SIMBOLO_HPP
#define SIMBOLO_HPP

#include <string>
#include <vector>

typedef enum { VARIAVEL_SIMPLES, PARAMETRO_FORMAL, PROCEDURE } Category;
typedef enum { INTEIRO, BOOLEANO, UNDEFINED } VariableType;
typedef enum { BY_VALUE, BY_REFERENCE } PassageType;  // for formal parameters

typedef std::vector<std::pair<VariableType, PassageType>> ParamFormal;

class Simbolo {
   public:
    Simbolo(const std::string& identificador, Category categoria, int nivel_lexico);

    // Setters for additional attributes based on the category
    Simbolo& setTipo(VariableType tipo);
    Simbolo& setDeslocamento(int deslocamento);
    Simbolo& setRotulo(char rotulo);
    Simbolo& setNumParams(int num_params);
    Simbolo& addParam(VariableType tipo, PassageType passagem);
    Simbolo& setPassagem(PassageType passagem);

    // Getters proibem alteracao
    std::string getIdentificador() const;
    const ParamFormal& getParams() const;
    Category getCategoria() const;
    VariableType getTipo() const;
    int getDeslocamento() const;
    int getNivelLexico() const;
    int getTamParams() const;
    char getRotulo() const;

    // cout << Simbolo
    void show();

   private:
    std::string identificador;
    Category categoria;
    int nivel_lexico;

    // Other attributes depending on the category
    PassageType passagem;
    VariableType tipo;
    int deslocamento;

    char rotulo;
    int num_params;
    ParamFormal params;
};

#endif