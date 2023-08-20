
// Testar se funciona corretamente o empilhamento de parametros
// passados por valor ou por referencia.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "stack.h"

int num_vars;
Stack* variaveis;

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO

%token LABEL TYPE ARRAY OF PROCEDURE FUNCTION 
%token GOTO IF THEN ELSE WHILE DO OR AND NOT NUMERO
%token MAIS MENOS ASTERISCO DIV MULT
%token DIFERENCA MENOR MENOR_IGUAL MAIOR_IGUAL MAIOR 

%%

/* 1. */
programa    :{
             geraCodigo (NULL, "INPP");
             }
             PROGRAM IDENT
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
             geraCodigo (NULL, "PARA");
             }
;

/* 2. */
bloco       : parte_declara_vars
              {
              }

              comando_composto
/*
            | parte_declara_rotulos
            | parte_declara_subrot 
*/
;


/* 3. */
/* parte_declara_rotulos   : parte_declara_rotulos VIRGULA NUMERO */
                        /* | LABEL NUMERO  */
                        /* | vazio */
/* ; */


/* 8. */
parte_declara_vars:  var
;

/* 9. */
var         : { } VAR declara_vars
            |
;

/* 9. */
declara_vars: declara_vars declara_var
            | declara_var
;

/* 9. */
declara_var : { }
              lista_id_var DOIS_PONTOS
              tipo
              { /* AMEM */
              }
              PONTO_E_VIRGULA
;

/* 6. (?) */
tipo        : IDENT
;

/* 9. (?) */
lista_id_var: lista_id_var VIRGULA IDENT
              { /* insere ultima vars na tabela de simbolos */ }
            | IDENT { /* insere vars na tabela de simbolos */}
;

/* 10. */
lista_idents: lista_idents VIRGULA IDENT
            | IDENT
;

/* 11. */
// parte_declara_subrot : parte_declara_subrot declara_procedimento PONTO_E_VIRGULA
//                      | parte_declara_subrot declara_funcao PONTO_E_VIRGULA
//                      /* | vazio */
// ;
// 
// /* 12. */
// declara_procedimento : PROCEDURE IDENT param_formais PONTO_E_VIRGULA bloco
// ;
// 
// /* 13. */
// declara_funcao    : FUNCTION IDENT param_formais PONTO_E_VIRGULA IDENT PONTO_E_VIRGULA bloco
// ;
// 
// /* 14. */
// param_formais  : param_formais PONTO_E_VIRGULA sec_param_formais
//                | sec_param_formais
// ;
// 
// /* 15. */
// sec_param_formais : VAR lista_idents DOIS_PONTOS IDENT
//                   | lista_idents DOIS_PONTOS IDENT
//                   /* | FUNCTION lista_idents DOIS_PONTOS IDENT
//                   | PROCEDURE lista_idents */
// ;
// 
/* 16. */
comando_composto: T_BEGIN comandos T_END
 
/* 17. */
comandos: 
         /* comandos PONTO_E_VIRGULA comando */
         /* | comando */
;

/* 17. */
// comando  : 
//          /* NUMERO PONTO_E_VIRGULA comando_sem_rotulo */
//          /* | comando_sem_rotulo */
// ;
// 
// /* 18. */
// comando_sem_rotulo   : atribuicao
//                      | chamada_procedimento
//                      | desvio 
//                      | comando_composto
//                      | comando_condicional
//                      | comando_repetitivo
// ;
// 
// /* 19. */
// atribuicao  : VAR ATRIBUICAO expr
// ;
// 
// /* 20. */
// chamada_procedimento : IDENT lista_expr
//                      | IDENT
// ;
// 
// /* 21. */
// desvio  : GOTO NUMERO
// ;
// 
// /* 22. */
// comando_condicional  : IF expr THEN comando_sem_rotulo
//                      | IF expr THEN comando_sem_rotulo ELSE comando_sem_rotulo
// ;
// 
// /* 23. */
// comando_repetitivo   : WHILE expr DO comando_sem_rotulo
// ;
// 
// /* 24. */
// lista_expr  : lista_expr VIRGULA expr 
//             | expr
// ;
// 
// /* 25. */
// expr        : expr_simples relacao expr_simples 
//             | expr_simples
// ;
// 
// /* 26. */
// relacao  : ATRIBUICAO 
//          | DIFERENCA
//          | MENOR_IGUAL
//          | MAIOR_IGUAL
//          | MENOR
//          | MAIOR
// ;
// 
// /* 27. */
// expr_simples: opt_sinal termo
//             | expr_simples MAIS termo
//             | expr_simples MENOR termo  
//             | expr_simples OR termo 
// ;
// 
// /* 27. */
// opt_sinal   : MAIS 
//             | MENOS 
//             | /* vazio */
// ;
// 
// /* 28. */
// termo : termo MULT fator 
//       | termo DIV fator 
//       | termo AND fator
//       | fator
// ;
// 
// /* 29. */
// fator : NOT fator
//       | NUMERO
//       | chamada_funcao
//       | expr
// ;
// 
// /* 31. */
// chamada_funcao : chamada_funcao lista_expr 
//                | IDENT
// ;

%%

int main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }


/* -------------------------------------------------------------------
 *  Inicia a Tabela de Simbolos
 * ------------------------------------------------------------------- */

   yyin=fp;
   yyparse();

   return 0;
}
