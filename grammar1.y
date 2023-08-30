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
programa    : { MEPA.Start(); }
            PROGRAM IDENT
            ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
            bloco PONTO {
                MEPA.Finish();
                TS.show();
            }
;

/* 2. */
bloco       : parte_declara_vars { /* AMEM */
                MEPA.Alloc(num_amem);
            }
            
            comando_composto 

            /* | parte_declara_rotulos */
            /* | parte_declara_subrot  */

            {
                MEPA.Free(num_amem);
                num_amem = 0;
            }
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
//                |
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
; 
/* 17. */
comandos: comandos PONTO_E_VIRGULA comando
        | comando
;

/* 17. */
comando : NUMERO DOIS_PONTOS comando_sem_rotulo
        | comando_sem_rotulo
        |
;

// /* 18. */
comando_sem_rotulo  : atribuicao 
                    | comando_composto
                    | comando_repetitivo
                    | comando_condicional
                    | READ  ABRE_PARENTESES lista_read FECHA_PARENTESES
                    | WRITE ABRE_PARENTESES lista_write FECHA_PARENTESES
//                      | chamada_procedimento
//                      | desvio 
;

lista_read  : lista_read VIRGULA IDENT  { MEPA.Read(); }
            | IDENT                     { MEPA.Read(); }
;

lista_write : lista_write VIRGULA fator { MEPA.Write(); }   
            | fator                     { MEPA.Write(); }   
;

// 
// /* 19. */
atribuicao  : variavel { addr_variavel = getAddrLex(); } ATRIBUICAO expr { MEPA.Atribuicao(); }
;

// /* 20. */
// chamada_procedimento    : IDENT ABRE_PARENTESES lista_expr FECHA_PARENTESES
//                         | IDENT ABRE_PARENTESES FECHA_PARENTESES
// ;
// 
// /* 21. */
// desvio  : GOTO NUMERO
// ;
// 
// /* 22. */
comando_condicional  : if_then  %prec LOWER_THEN_ELSE { popRotulo(); } 
                     | if_then ELSE { MEPA.JumpTo(novoRotulo()); popPenultRotulo(); } comando_sem_rotulo { popRotulo(); }
;
if_then             : IF expr { MEPA.IfJumpTo(novoRotulo()); } THEN comando_sem_rotulo
// 
// /* 23. */
comando_repetitivo  : 
            WHILE { MEPA.rotAddrNome(novoRotulo()); } expr DO 
                { MEPA.IfJumpTo(novoRotulo()); } comando_sem_rotulo { geraCodigoEndWhile(); }
;
// 
// /* 24. */
// lista_expr  : lista_expr VIRGULA expr 
//             | expr
// ;
// 
// /* 25. & 26. */
expr        : expr_simples 
            | expr_simples DIFERENCA   expr_simples { MEPA.Operacao(MEPA.DIFF, VariableType::BOOLEANO);  }
            | expr_simples IGUALDADE   expr_simples { MEPA.Operacao(MEPA.IGUAL, VariableType::BOOLEANO);  }
            | expr_simples MENOR_IGUAL expr_simples { MEPA.Operacao(MEPA.MENOR_IGUAL, VariableType::BOOLEANO);  }
            | expr_simples MAIOR_IGUAL expr_simples { MEPA.Operacao(MEPA.MAIOR_IGUAL, VariableType::BOOLEANO);  }
            | expr_simples MENOR       expr_simples { MEPA.Operacao(MEPA.MENOR, VariableType::BOOLEANO);  }
            | expr_simples MAIOR       expr_simples { MEPA.Operacao(MEPA.MAIOR, VariableType::BOOLEANO);  }
;

/* 27. */
// expr_simples: opt_sinal conteudo 
// ;
// /* 27. */
// opt_sinal   : MAIS | MENOS | /* vazio */;
// conteudo    : conteudo oper | termo ;
expr_simples: expr_simples MAIS  termo { MEPA.Operacao(MEPA.SOMA, VariableType::INTEIRO); }   
            | expr_simples MENOS termo { MEPA.Operacao(MEPA.SUBT, VariableType::INTEIRO); }   
            | expr_simples OR    termo { MEPA.Operacao(MEPA.OR, VariableType::BOOLEANO); } 
            | termo
;

/* 28. */
termo       : termo MULT fator { MEPA.Operacao(MEPA.MULT, VariableType::INTEIRO); } 
            | termo DIV  fator { MEPA.Operacao(MEPA.DIV, VariableType::INTEIRO); } 
            | termo AND  fator { MEPA.Operacao(MEPA.AND, VariableType::BOOLEANO); } 
            | fator
;

/* 29. */
fator   : variavel  { MEPA.LoadFrom(getAddrLex()); }
        | NUMERO    { MEPA.LoadValue(std::string(meu_token)); stack_tipos.push_back(VariableType::INTEIRO); }
        | TRUE      { MEPA.LoadValue(std::to_string(1)); stack_tipos.push_back(VariableType::BOOLEANO); }  
        | FALSE     { MEPA.LoadValue(std::to_string(0)); stack_tipos.push_back(VariableType::BOOLEANO); }     
        | ABRE_PARENTESES expr FECHA_PARENTESES
        /* | NOT fator */
        /* | chamada_funcao */
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
// chamada_funcao : chamada_funcao ABRE_PARENTESES lista_expr FECHA_PARENTESES 
//                | IDENT
// ;

%%
 
void bison::Parser::error(const std::string& msg) {
    std::cerr << msg << " at line " << num_line <<'\n';
    exit(0);
}