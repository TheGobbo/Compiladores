#ifndef SIMBOLO_HPP
#define SIMBOLO_HPP

#include <deque>
#include <string>

typedef enum { INTEIRO, BOOLEANO, UNDEFINED } VariableType;
typedef enum {
    VARIAVEL_SIMPLES,
    PARAMETRO_FORMAL,
    PROCEDURE,
    FUNCTION,
    CTE
} Category;
typedef enum {
    BY_VALUE,
    BY_REFERENCE,
    BY_UNDEFINED
} PassageType;  // for formal parameters

typedef std::deque<std::pair<VariableType, PassageType>> ParamFormal;

class Simbolo {
   public:
    Simbolo(const std::string& identificador, Category categoria, int nivel_lexico);

    // Setters for additional attributes based on the category
    Simbolo& setTipo(VariableType tipo);
    Simbolo& setDeslocamento(int deslocamento);
    Simbolo& setRotulo(char rotulo);
    Simbolo& setNumParams(int num_params);
    Simbolo& addParam(VariableType tipo, PassageType passagem);
    Simbolo& setParams(ParamFormal params);
    Simbolo& setPassagem(PassageType passagem);

    // Getters proibem alteracao
    std::string getIdentificador() const;
    const ParamFormal& getParams() const;
    std::string getAddr() const;
    Category getCategoria() const;
    VariableType getTipo() const;
    PassageType getPassage() const;
    int getDeslocamento() const;
    int getNivelLexico() const;
    int getTamParams() const;
    char getRotulo() const;

    // cout << Simbolo
    void show();
    static std::string showVariable(VariableType tipo);
    static std::string showPassage(PassageType tipo);
    static std::string showCategory(Category tipo);

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