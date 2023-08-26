/* https://learnmoderncpp.com/2020/12/17/generating-c-programs-with-flex-and-bison-2/ */

%{
#include <iostream>
#include <string>
#include <cmath>
#include "Scanner.hpp"
#include "compilador.hpp"
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
%token MAIS MENOS ASTERISCO DIV MULT
%token DIFERENCA MENOR MENOR_IGUAL MAIOR_IGUAL MAIOR 
%token INTEGER BOOLEAN

%left MAIS MENOS
%left MULT DIV MODULO
%%

/* 1. */
programa    :{
             geraCodigo (NULL, "INPP");
             }
             PROGRAM IDENT
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
             geraCodigo (NULL, "PARA");
             TS.show();
             }
;

/* 2. */
bloco       : parte_declara_vars { /* AMEM */
                geraCodigo(std::to_string(num_amem).c_str(), "AMEM");
                num_amem = 0;
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
parte_declara_vars  : parte_declara_vars var
                    | var
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
              tipo { salvarTipos(simbolo); }
              PONTO_E_VIRGULA
;

/* 6. (?) */
tipo        : INTEGER | BOOLEAN
;

/* 9. (?) */
lista_id_var    : lista_id_var VIRGULA IDENT { 
                    /* insere ultima vars na tabela de simbolos */ 
                    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico, TS.getNovoDeslocamento(nivel_lexico)}};
                    TS.InsereSimbolo(var);
                    num_amem++;
                    num_vars++;
                }
                | IDENT { 
                    /* insere vars na tabela de simbolos */
                    Simbolo* var{new Simbolo{meu_token, VARIAVEL_SIMPLES, nivel_lexico, TS.getNovoDeslocamento(nivel_lexico)}};
                    TS.InsereSimbolo(var);
                    num_amem++;
                    num_vars++;
                }
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
comando_composto: { print = true; } T_BEGIN comandos T_END 
 
/* 17. */
comandos: comandos PONTO_E_VIRGULA comando
        | comando
;

/* 17. */
comando : NUMERO PONTO_E_VIRGULA comando_sem_rotulo
        | comando_sem_rotulo
        |
;

// /* 18. */
comando_sem_rotulo   : atribuicao
//                      | chamada_procedimento
//                      | desvio 
//                      | comando_composto
//                      | comando_condicional
//                      | comando_repetitivo
;
// 
// /* 19. */
atribuicao  : variavel ATRIBUICAO expr {operaTiposValidos();}
;

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
expr        : expr_simples relacao expr_simples { operaTiposValidos(BOOLEANO); }
            /* expr relacao expr_simples */
            | expr_simples 
;

/* 26. */
relacao  : DIFERENCA
         | MENOR_IGUAL
         | MAIOR_IGUAL
         | MENOR
         | MAIOR
;

/* 27. */
expr_simples: opt_sinal conteudo 
;
/* 27. */
opt_sinal   : MAIS | MENOS | /* vazio */;
conteudo    : conteudo oper | termo ;
oper        : MAIS termo  { operaTiposValidos(INTEIRO); }  
            | MENOS termo { operaTiposValidos(INTEIRO); }  
            | OR termo    { operaTiposValidos(BOOLEANO); }
;

/* 28. */
termo : termo MULT fator { operaTiposValidos(INTEIRO); }
      | termo DIV fator  { operaTiposValidos(INTEIRO); }
      | termo AND fator  { operaTiposValidos(BOOLEANO); }
      | fator
;

/* 29. */
fator   : variavel
        | NOT fator
        | NUMERO {stack_tipos.push_back(INTEIRO);}
        /* | chamada_funcao */
        | expr
;

/* 30. */   /* ERRO se != VS, PF ou Func */
variavel    : IDENT {
                Simbolo* simbolo = TS.BuscarSimbolo(meu_token);
                if(simbolo == nullptr) error("variable not found");
                stack_tipos.push_back(simbolo->getTipo());
            }
            /* | IDENT lista_expr */
;
// /* 31. */
// chamada_funcao : chamada_funcao lista_expr 
//                | IDENT
// ;

%%
 
void bison::Parser::error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line <<'\n';
    exit(0);
}