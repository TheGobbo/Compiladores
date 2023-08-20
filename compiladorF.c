
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compilador.h"

/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

simbolos simbolo, relacao;
char token[TAM_TOKEN];

FILE* fp = NULL;
void geraCodigo(char* rot, char* comando) {
    if (fp == NULL) {
        fp = fopen("MEPA", "w");
    }

    if (rot == NULL) {
        fprintf(fp, "     %s\n", comando);
        fflush(fp);
    } else {
        fprintf(fp, "%s: %s \n", rot, comando);
        fflush(fp);
    }
}

int imprimeErro(char* erro) {
    fprintf(stderr, "Erro na linha %d - %s\n", nl, erro);
    exit(-1);
}

void type_checker(Stack* s) {
    char l, r;
    l = stack_pop(s);
    r = stack_pop(s);
    // printf("l(%c) r(%c)\n", l, r);

    if (l != r) {
        char erro[40];
        sprintf(erro, "Types must be equal! %c <> %c", l, r);
        imprimeErro(erro);
        // printf("Error in line : %d\n", nl);
        // yyerror("Types must be equal!\n");
    }  // else l == r :

    stack_push(s, r);
}
