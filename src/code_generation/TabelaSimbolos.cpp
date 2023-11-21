#include "code_generation/TabelaSimbolos.hpp"

#include <cstring>
#include <iostream>

#include "code_generation/Simbolo.hpp"
#include "compilador.hpp"

TabelaSimbolos::TabelaSimbolos() {}

TabelaSimbolos::~TabelaSimbolos() {}

int TabelaSimbolos::getNovoDeslocamento(int nivel_lexico) {
    if (this->tabelaDeSimbolos.empty()) {
        return 0;
    }

    Simbolo* topo = (Simbolo*)this->tabelaDeSimbolos.back();
    // inicia com zero se for novo nivel lexico ou se o topo eh procedure
    return (topo->getNivelLexico() == nivel_lexico && topo->getDeslocamento() >= 0 &&
            topo->getCategoria() != Category::PROCEDURE)
               ? topo->getDeslocamento() + 1
               : 0;
}

void TabelaSimbolos::setTipos(VariableType tipo) {
    if (this->tabelaDeSimbolos.empty()) {
        return;
    }
    Simbolo* simb = nullptr;

    for (auto it = this->tabelaDeSimbolos.rbegin();
         it != this->tabelaDeSimbolos.rend(); ++it) {
        simb = (Simbolo*)*it;
        if (simb->getTipo() != UNDEFINED) {
            return;
        }
        simb->setTipo(tipo);
    }
}

int TabelaSimbolos::setParamFormal() {
    int deslocamento = -4;

    ParamFormal infoProc;

    std::deque<Simbolo*>::reverse_iterator simb = this->tabelaDeSimbolos.rbegin();
    for (; (*simb)->getCategoria() == Category::PARAMETRO_FORMAL; ++simb) {
        (*simb)->setDeslocamento(deslocamento--);
        infoProc.push_front(
            std::make_pair((*simb)->getTipo(), (*simb)->getPassage()));
    }

    if ((*simb)->getCategoria() == Category::FUNCTION) {
        (*simb)->setDeslocamento(deslocamento);
    }

    int num_params = std::abs(deslocamento + 4);
    (*simb)->setNumParams(num_params);
    (*simb)->setParams(infoProc);

    return num_params;
}

void TabelaSimbolos::InsereSimbolo(Simbolo* simbolo) {
    if (simbolo == nullptr) {
        std::cerr << "'Insere simbolo' abortado, 'simbolo' invalido\n";
        return;
    }

    if (this->quantidade_simbolos >= MAX_STACK_SIZE) {
        std::cerr << "'Insere simbolo' abortado, 'TabelaSimbolos' cheia\n";
        return;
    }

    std::cout << "Inserindo : ";
    simbolo->show();
    this->tabelaDeSimbolos.push_back(simbolo);
    this->quantidade_simbolos++;
}

void TabelaSimbolos::RemoveSimbolos(int quantidade_simbolos) {
    while (quantidade_simbolos > 0 && !this->tabelaDeSimbolos.empty()) {
        std::cerr << "Removendo : ";
        ((Simbolo*)this->tabelaDeSimbolos.back())->show();
        this->tabelaDeSimbolos.pop_back();
        this->quantidade_simbolos--;
        quantidade_simbolos--;
    }
    return;
}

Simbolo* TabelaSimbolos::BuscarSimbolo(const std::string& identificador) {
    Simbolo* simb = nullptr;
    for (auto it = this->tabelaDeSimbolos.rbegin();
         it != this->tabelaDeSimbolos.rend(); ++it) {
        simb = (Simbolo*)*it;

        /*
            procedure f(x) -> nl 1
                procedure g(x) -> nl 2

            g(1) -> nl 1

        */

        if (simb->getNivelLexico() > nivel_lexico &&
            (simb->getCategoria() != Category::FUNCTION &&
             simb->getCategoria() != Category::PROCEDURE)) {
            // !func !proc -> vs pf
            // nl desse simb > nivel lexico atual
            // ignore
            std::cout << "SKIPPED : " << simb->getIdentificador() << " "
                      << simb->getNivelLexico() << " > " << nivel_lexico << " && "
                      << Simbolo::showCategory(simb->getCategoria()) << '\n';
            continue;
        }

        if (simb->getIdentificador() == identificador) {
            return simb;
        }
        // clang-format on
    }
    return nullptr;
}

Simbolo* TabelaSimbolos::getTopo() { return this->tabelaDeSimbolos.back(); }

void TabelaSimbolos::show() {
    std::cout << "NIVEL LEXICO : " << nivel_lexico << '\n';
    std::cout << "+ STACK TOP  +\n";
    for (auto it = this->tabelaDeSimbolos.rbegin();
         it != this->tabelaDeSimbolos.rend(); ++it) {
        std::cout << "| ";
        ((Simbolo*)*it)->show();
    }
    std::cout << "+ STACK DOWN +\n";
}

bool TabelaSimbolos::empty() { return this->tabelaDeSimbolos.empty(); }

void TabelaSimbolos::clear() { this->RemoveSimbolos(this->quantidade_simbolos); }

int TabelaSimbolos::size() const { return this->tabelaDeSimbolos.size(); }