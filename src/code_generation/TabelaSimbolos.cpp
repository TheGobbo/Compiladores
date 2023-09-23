#include "code_generation/TabelaSimbolos.hpp"

#include <cstring>
#include <iostream>

#include "code_generation/Simbolo.hpp"

TabelaSimbolos::TabelaSimbolos() {}

TabelaSimbolos::~TabelaSimbolos() {}

int TabelaSimbolos::getNovoDeslocamento(int nivel_lexico) {
    if (this->tabelaDeSimbolos.empty()) {
        return 0;
    }

    Simbolo* topo = (Simbolo*)this->tabelaDeSimbolos.back();
    // inicia com zero se for novo nivel lexico ou se o topo eh procedure
    return (topo->getNivelLexico() == nivel_lexico && topo->getCategoria() != Category::PROCEDURE)
               ? topo->getDeslocamento() + 1
               : 0;
}

void TabelaSimbolos::setTipos(VariableType tipo) {
    if (this->tabelaDeSimbolos.empty()) {
        return;
    }
    Simbolo* simb = nullptr;

    for (auto it = this->tabelaDeSimbolos.rbegin(); it != this->tabelaDeSimbolos.rend(); ++it) {
        simb = (Simbolo*)*it;
        if (simb->getTipo() != UNDEFINED) {
            return;
        }
        simb->setTipo(tipo);
    }
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

// void TabelaSimbolos::RemoveProcedures(int nivel_lexico) {
//     int proc_nl;
//     int nl_proc;

//     std::list<Simbolo*>::iterator it;
//     for (it = this->tabelaDeSimbolos.begin(); it != this->tabelaDeSimbolos.end();) {
//         if ((*it)->getCategoria() != Category::PROCEDURE) {
//             ++it;
//             continue;
//         }

//         proc_nl = (*it)->getNivelLexico();

//         if (nivel_lexico <= proc_nl - 2) {
//             this->tabelaDeSimbolos.erase(it++);
//         } else {
//             ++it;
//         }
//     }
// }

Simbolo* TabelaSimbolos::BuscarSimbolo(const std::string& identificador) {
    Simbolo* simb = nullptr;
    for (auto it = this->tabelaDeSimbolos.rbegin(); it != this->tabelaDeSimbolos.rend(); ++it) {
        simb = (Simbolo*)*it;
        if (simb->getIdentificador() == identificador) {
            return simb;
        }
    }
    return nullptr;
}

Simbolo* TabelaSimbolos::getTopo() { return this->tabelaDeSimbolos.back(); }

void TabelaSimbolos::show() {
    std::cout << "+ STACK TOP  +\n";
    for (auto it = this->tabelaDeSimbolos.rbegin(); it != this->tabelaDeSimbolos.rend(); ++it) {
        std::cout << "| ";
        ((Simbolo*)*it)->show();
    }
    std::cout << "+ STACK DOWN +\n";
}

bool TabelaSimbolos::empty() { return this->tabelaDeSimbolos.empty(); }

void TabelaSimbolos::clear() { this->RemoveSimbolos(this->quantidade_simbolos); }
