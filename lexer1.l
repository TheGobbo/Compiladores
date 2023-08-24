/* https://learnmoderncpp.com/2020/12/16/generating-c-programs-with-flex-and-bison-1/ */
%{
#include <cstdlib>
#include <iomanip>
#include "Parser1.hpp"
#include "compilador.hpp"

using namespace calc;

// #define IMPRIME(STR) {};
#define PADD std::left << std::setw(TAM_TOKEN) << std::setfill(' ')
#define IMPRIME(STR) std::cerr << PADD << STR << "\t# " << PADD << token << nl << '\n';

int nl = 1;
%}

%option reentrant interactive noyywrap noyylineno nodefault outfile="Scanner1.cpp" header="Scanner1.hpp"

pulo_linha [\n]

descarte [\t ]+

ident [a-zA-Z][a-zA-Z0-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;}

{descarte}

{comentario}

program { simbolo  = simb_program;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("program ");
          return Parser::token::PROGRAM;
 }

label   { simbolo = simb_label;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("label ");
          return Parser::token::LABEL;
 }

type    { simbolo = simb_type;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("type ");
          return Parser::token::TYPE;
 }

array     { simbolo = simb_array;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("array ");
          return Parser::token::ARRAY;
 }

of    { simbolo = simb_of;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("of ");
          return Parser::token::OF;
 }

var      { simbolo = simb_var;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("var  ");
          return Parser::token::VAR;
 }

procedure     { simbolo = simb_procedure;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("procedure ");
          return Parser::token::PROCEDURE;
 }

function    { simbolo = simb_function;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("function ");
          return Parser::token::FUNCTION;
 }

goto    { simbolo = simb_goto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("goto ");
          return Parser::token::GOTO;
 }

if    { simbolo = simb_if;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("if ");
          return Parser::token::IF;
 }

then    { simbolo = simb_then;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("then ");
          return Parser::token::THEN;
 }

else    { simbolo = simb_else;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("else ");
          return Parser::token::ELSE;
 }

while     { simbolo = simb_while;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("while ");
          return Parser::token::WHILE;
 }

do    { simbolo = simb_do;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("do ");
          return Parser::token::DO;
 }

or    { simbolo = simb_or;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("or ");
          return Parser::token::OR;
 } 

div     { simbolo = simb_div;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("div ");
          return Parser::token::DIV;
 }

and     { simbolo = simb_and;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("and ");
          return Parser::token::AND;
 }

not     { simbolo = simb_not;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("not ");
          return Parser::token::NOT;
 }

begin { simbolo = simb_begin;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return Parser::token::T_BEGIN;
 }

end { simbolo = simb_end;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end  ");
          return Parser::token::T_END;
 }

:= { simbolo = simb_atribuicao;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" atribuicao  ");
          return Parser::token::ATRIBUICAO;
 }

\; { simbolo = simb_ponto_e_virgula;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return Parser::token::PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return Parser::token::DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return Parser::token::VIRGULA;
 }


\. { simbolo = simb_ponto;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return Parser::token::PONTO;
    }

\(  { simbolo = simb_abre_parenteses;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return Parser::token::ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return Parser::token::FECHA_PARENTESES;
 }

\+  { simbolo = simb_mais;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" mais ");
          return Parser::token::MAIS; 
}

\-  { simbolo = simb_menos;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menos ");
          return Parser::token::MENOS; 
}

(<>)  {   simbolo = simb_diferenca;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" diferenca ");
          return Parser::token::DIFERENCA; 
}

(<=)  {   simbolo = simb_menor_igual;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor_igual ");
          return Parser::token::MENOR_IGUAL; 
}

(>=)  {   simbolo = simb_maior_igual;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior_igual ");
          return Parser::token::MAIOR_IGUAL; 
}

\<  {   simbolo = simb_menor;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menor ");
          return Parser::token::MENOR; 
}

\>  {   simbolo = simb_maior;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" maior ");
          return Parser::token::MAIOR; 
}

{numero}  { simbolo = simb_numero;
            strncpy(token, yytext, TAM_TOKEN);
            IMPRIME(" numero");
            return Parser::token::NUMERO;
 }

{ident} { simbolo = simb_identificador;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return Parser::token::IDENT;
        }

.           /* no action on unmatched input */


 
%%
 
int main() {
    yyscan_t scanner;
    yylex_init(&scanner);
    calc::Parser parser{ scanner };
    std::cout.precision(10);
    parser.parse();
    yylex_destroy(scanner);
}