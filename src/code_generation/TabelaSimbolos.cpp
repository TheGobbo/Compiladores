#include "include/code_generation/TabelaSimbolos.hpp"

#include <cstring>
#include <iostream>

#include "include/code_generation/Simbolo.hpp"

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
    if (simbolo == nullptr || !simbolo->valido()) {
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

Simbolo* TabelaSimbolos::BuscarSimbolo(char (&identificador)[TAM_TOKEN]) {
    Simbolo* simb = nullptr;
    for (auto it = this->tabelaDeSimbolos.rbegin(); it != this->tabelaDeSimbolos.rend(); ++it) {
        simb = (Simbolo*)*it;
        if (std::strncmp(simb->getIdent(), identificador, TAM_TOKEN) == 0) {
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
/*
// testes da classe
int main() {
    // Create a TabelaSimbolos object
    TabelaSimbolos tabela;

    // Create Simbolo objects
    char id1[TAM_TOKEN] = "primeiro";
    char id2[TAM_TOKEN] = "segundo";
    char(&rid1)[TAM_TOKEN]{id1};
    char(&rid2)[TAM_TOKEN]{id2};

    Simbolo* s1{new Simbolo{rid1, VARIAVEL_SIMPLES, 0, 0}};
    Simbolo* s2{new Simbolo{rid2, PROCEDURE, 1, 1}};

    // Insert Simbolo objects into the TabelaSimbolos
    tabela.InsereSimbolo(s1);
    tabela.InsereSimbolo(s2);

    // Display the contents of the TabelaSimbolos
    tabela.show();

    // Search for a Simbolo by identificador
    Simbolo* foundSymbol = tabela.BuscarSimbolo(rid1);
    if (foundSymbol) {
        std::cout << "Found Simbolo: ";
        foundSymbol->show();
    } else {
        std::cout << "Simbolo not found.\n";
    }

    // Remove Simbolos
    tabela.RemoveSimbolos(1);

    // Display the updated contents of the TabelaSimbolos
    tabela.show();

    // Search for a Simbolo that has been removed
    foundSymbol = tabela.BuscarSimbolo(rid1);
    if (foundSymbol) {
        std::cout << "Found Simbolo: ";
        foundSymbol->show();
    } else {
        std::cout << "Simbolo not found (after removal)." << '\n';
    }

    tabela.RemoveSimbolos(-3);
    tabela.InsereSimbolo(nullptr);
    tabela.InsereSimbolo({new Simbolo{rid1, VARIAVEL_SIMPLES, -1, 0}});

    return 0;
}
*/
