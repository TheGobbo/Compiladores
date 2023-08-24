
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [09/08/2020, 19h:01m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include "compilador.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

TabelaSimbolos TS;
simbolos simbolo, relacao;
char token[TAM_TOKEN];

static std::ofstream fp;  // Use an ofstream instead of FILE* for C++ file handling
void geraCodigo(const char* rot, const char* comando) {
    if (!fp.is_open()) {
        fp.open("MEPA");  // Open the file "MEPA" for writing
    }

    if (rot == nullptr) {
        fp << "     " << comando << '\n';
        fp.flush();  // Flush the output to the file
    } else {
        fp << rot << ": " << comando << '\n';
        fp.flush();  // Flush the output to the file
    }
}

int imprimeErro(const char* erro) {
    std::cerr << "Erro na linha " << nl << " - " << erro << '\n';
    std::exit(-1);  // Use std::exit to terminate the program
}

void type_checker() {
    TS.show();
    // char l, r;
    // l = TS.(s);
    // r = stack_pop(s);
    // // printf("l(%c) r(%c)\n", l, r);

    // if (l != r) {
    //     char erro[40];
    //     sprintf(erro, "Types must be equal! %c <> %c", l, r);
    //     imprimeErro(erro);
    //     // printf("Error in line : %d\n", nl);
    //     // yyerror("Types must be equal!\n");
    // }  // else l == r :

    // stack_push(s, r);
}
