/* https://learnmoderncpp.com/2020/12/17/generating-c-programs-with-flex-and-bison-2/ */

%{
#include <iostream>
#include <string>
#include <cmath>
#include "Scanner.hpp"
#include "compilador.hpp"

#define flags(STR) std::cerr << "\033[1;31m" << STR << "\033[0m\n"
#define flag std::cerr << "\033[1;31mFLAG\033[0m\n"

%}
 
%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"
 
%define api.parser.class {Parser}
%define api.namespace {bison}
%define api.value.type variant
%param {yyscan_t scanner}
 
%code provides
{
    #define YY_DECL \
        int yylex(bison::Parser::semantic_type *yylval, yyscan_t yyscanner)
    YY_DECL;
}


%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO

%token LABEL TYPE ARRAY OF PROCEDURE FUNCTION 
%token GOTO IF THEN ELSE WHILE DO OR AND NOT NUMERO
%token MAIS MENOS ASTERISCO DIV MULT READ WRITE
%token DIFERENCA MENOR MENOR_IGUAL MAIOR_IGUAL MAIOR IGUALDADE
%token INTEGER BOOLEAN TRUE FALSE 

%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE
%%

/* 1. */
programa    : 
        { beginCompilador(); }
            PROGRAM IDENT
            ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
            bloco PONTO 
        { endCompilador(); }
;

/* 2. */
bloco       : 
            parte_declara_vars      { varsDeclarado(); }

            /* parte_declara_rotulos */
            parte_declara_subrot    { subrotDeclarado(); }
            comando_composto        { endComandos(); }
;


/* 3. */
/* parte_declara_rotulos   : parte_declara_rotulos VIRGULA NUMERO */
                        /* | LABEL NUMERO  */
                        /* | %empty */
/* ; */


/* 8. */
parte_declara_vars  : var
;

/* 9. */
var         : VAR declara_vars
            | %empty
;

/* 9. */
declara_vars: declara_vars declara_var
            | declara_var
;

/* 9. */
declara_var : lista_id_var DOIS_PONTOS tipo { aplicarTipos(); } PONTO_E_VIRGULA
;

/* 6. (?) */
tipo        : INTEGER | BOOLEAN
;

/* 9. (?) */
lista_id_var    : lista_id_var VIRGULA IDENT { novoSimbolo(); }  /* insere ultima vars na tabela de simbolos */ 
                | IDENT { novoSimbolo(); }                       /* insere vars na tabela de simbolos */
;

/* 10. */
lista_idents: lista_idents VIRGULA IDENT
            | IDENT
;

/* 11. */
parte_declara_subrot : parte_declara_subrot declara_procedimento PONTO_E_VIRGULA 
                     /* | parte_declara_subrot declara_funcao PONTO_E_VIRGULA */
                     | %empty
;

/* 12. */
declara_procedimento : PROCEDURE IDENT 
                    { beginProcedure(); } 
                        param_formais PONTO_E_VIRGULA bloco 
                    { endProcedure(); }
;
// 
// /* 13. */
// declara_funcao    : FUNCTION IDENT param_formais PONTO_E_VIRGULA IDENT PONTO_E_VIRGULA bloco
// ;
// 
/* 14. */
param_formais   : ABRE_PARENTESES nparam_formais FECHA_PARENTESES
                | %empty
;

/* 14. */
nparam_formais  : nparam_formais PONTO_E_VIRGULA sec_param_formais
                | sec_param_formais
;

/* 15. */
sec_param_formais : VAR lista_idents DOIS_PONTOS IDENT
                  | lista_idents DOIS_PONTOS IDENT
                  | PROCEDURE lista_idents 
                  /* /* | FUNCTION lista_idents DOIS_PONTOS IDENT */
;

/* 16. */
comando_composto: { print = true; } T_BEGIN comandos T_END
; 
/* 17. */
comandos    : comandos PONTO_E_VIRGULA comando
            | comando
;

/* 17. */
comando     : NUMERO DOIS_PONTOS comando_sem_rotulo
            | comando_sem_rotulo
            | %empty
;

/* 18. */
comando_sem_rotulo  : atribuicao_e_procedimento
                    /* | chamada_procedimento */
                    | comando_composto
                    | comando_repetitivo
                    | comando_condicional   { endCondicional(); }
                    | READ  ABRE_PARENTESES lista_read FECHA_PARENTESES
                    | WRITE ABRE_PARENTESES lista_write FECHA_PARENTESES
//                      | desvio 
;

/* 18. + aula 10*/
atribuicao_e_procedimento   : IDENT { declaraIdentificador(); } ident_continua
;

/* 18. + aula 10*/
ident_continua  : atribuicao            { aplicaAtribuicao(); }
                | chamada_procedimento  { callProcedure(); }
;

lista_read  : lista_read VIRGULA IDENT  { Read(); }
            | IDENT                     { Read(); }
;

lista_write : lista_write VIRGULA fator { Write(); }
            | fator                     { Write(); }
;

// 
// /* 19. */
atribuicao  : ATRIBUICAO expr 
;

/* 20. */
chamada_procedimento    : ABRE_PARENTESES lista_expr FECHA_PARENTESES
                        | %empty
;

// /* 21. */
// desvio  : GOTO NUMERO
// ;
// 
// /* 22. */
comando_condicional  : if_then  %prec LOWER_THEN_ELSE
                     | if_then ELSE { elseCondicional(); } comando_sem_rotulo
;
if_then             : IF expr { beginCondicional(); } THEN comando_sem_rotulo
// 
// /* 23. */
comando_repetitivo  : WHILE 
                { beginWhile(); } 
                    expr { beginCondicional(); } DO comando_sem_rotulo 
                { endWhile(); }
;

/* 24. */
lista_expr  : lista_expr VIRGULA { } expr 
            | expr
;

/* 25. & 26. */
expr        : expr_simples 
            | expr_simples MENOR       expr_simples { aplicarOperacao(simb_menor); }
            | expr_simples MAIOR       expr_simples { aplicarOperacao(simb_maior); }
            | expr_simples IGUALDADE   expr_simples { aplicarOperacao(simb_igualdade); }
            | expr_simples DIFERENCA   expr_simples { aplicarOperacao(simb_diferenca); }
            | expr_simples MAIOR_IGUAL expr_simples { aplicarOperacao(simb_maior_igual); }
            | expr_simples MENOR_IGUAL expr_simples { aplicarOperacao(simb_menor_igual); }
;

/* 27. */
// expr_simples: opt_sinal conteudo 
// ;
// /* 27. */
// opt_sinal   : MAIS | MENOS | /* vazio */;
// conteudo    : conteudo oper | termo ;
expr_simples: expr_simples MAIS  termo { aplicarOperacao(simb_mais); }   
            | expr_simples MENOS termo { aplicarOperacao(simb_menos); }   
            | expr_simples OR    termo { aplicarOperacao(simb_or); } 
            | termo
;

/* 28. */
termo       : termo MULT fator { aplicarOperacao(simb_mult); }
            | termo DIV  fator { aplicarOperacao(simb_div); } 
            | termo AND  fator { aplicarOperacao(simb_and); }
            | fator
;

/* 29. */
fator   : variavel  
        | TRUE      { loadConstante("true"); }
        | FALSE     { loadConstante("false"); }
        | NUMERO    { loadConstante(std::string(meu_token)); }
        | ABRE_PARENTESES expr FECHA_PARENTESES
        /* | NOT fator */
        /* | chamada_funcao */
;

/* 30. */   /* ERRO se != VS, PF ou Func */
variavel    : IDENT { saveVariavel(); }
            /* | IDENT lista_expr */
;
// /* 31. */
// chamada_funcao : chamada_funcao ABRE_PARENTESES lista_expr FECHA_PARENTESES 
//                | IDENT
// ;

%%
 
void bison::Parser::error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line <<'\n';
    std::cerr << "with token " << meu_token << '\n';
    exit(0);
}