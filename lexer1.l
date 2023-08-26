/* https://learnmoderncpp.com/2020/12/16/generating-c-programs-with-flex-and-bison-1/ */
%{
#include <cstdlib>
#include <iomanip>
#include "Parser.hpp"
#include "compilador.hpp"

using namespace bison;

#define PADD std::left << std::setw(TAM_TOKEN) << std::setfill(' ')
#define IMPRIME(STR) if(print) { std::cerr << PADD << STR << "\t# " << PADD << meu_token << num_line << '\n' ;}
// #define IMPRIME(STR) {};

%}

%option reentrant interactive noyywrap noyylineno nodefault outfile="Scanner.cpp" header="Scanner.hpp"

pulo_linha [\n]

descarte [\t ]+

ident [a-zA-Z][a-zA-Z0-9]*

numero [0-9]+

comentario "(*".*"*)"


%%

{pulo_linha} {num_line++;}

{descarte}

{comentario}

program { simbolo  = simb_program;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("program ");
          return Parser::token::PROGRAM;
 }

label   { simbolo = simb_label;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("label ");
          return Parser::token::LABEL;
 }

type    { simbolo = simb_type;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("type ");
          return Parser::token::TYPE;
 }

array     { simbolo = simb_array;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("array ");
          return Parser::token::ARRAY;
 }

of    { simbolo = simb_of;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("of ");
          return Parser::token::OF;
 }

var      { simbolo = simb_var;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("var  ");
          return Parser::token::VAR;
}

integer {   simbolo = simb_integer;
            IMPRIME("integer ");
            return Parser::token::INTEGER;
}

boolean {   simbolo = simb_boolean;
            IMPRIME("boolean ");
            return Parser::token::BOOLEAN;
}

procedure     { simbolo = simb_procedure;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("procedure ");
          return Parser::token::PROCEDURE;
 }

function    { simbolo = simb_function;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("function ");
          return Parser::token::FUNCTION;
 }

goto    { simbolo = simb_goto;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("goto ");
          return Parser::token::GOTO;
 }

if    { simbolo = simb_if;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("if ");
          return Parser::token::IF;
 }

then    { simbolo = simb_then;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("then ");
          return Parser::token::THEN;
 }

else    { simbolo = simb_else;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("else ");
          return Parser::token::ELSE;
 }

while     { simbolo = simb_while;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("while ");
          return Parser::token::WHILE;
 }

do    { simbolo = simb_do;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("do ");
          return Parser::token::DO;
 }

or    { simbolo = simb_or;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("or ");
          return Parser::token::OR;
 } 

div     { simbolo = simb_div;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("div ");
          return Parser::token::DIV;
}

\*  {   simbolo = simb_mult;
        strncpy (meu_token, yytext, TAM_TOKEN);
        IMPRIME("mult ");
        return Parser::token::MULT;
}

and     { simbolo = simb_and;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("and ");
          return Parser::token::AND;
 }

not     { simbolo = simb_not;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("not ");
          return Parser::token::NOT;
 }

begin { simbolo = simb_begin;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return Parser::token::T_BEGIN;
 }

end { simbolo = simb_end;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME("end ");
          return Parser::token::T_END;
 }

:= { simbolo = simb_atribuicao;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" atribuicao  ");
          return Parser::token::ATRIBUICAO;
 }

\; { simbolo = simb_ponto_e_virgula;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return Parser::token::PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return Parser::token::DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return Parser::token::VIRGULA;
 }


\. { simbolo = simb_ponto;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return Parser::token::PONTO;
    }

\(  { simbolo = simb_abre_parenteses;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return Parser::token::ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return Parser::token::FECHA_PARENTESES;
 }

\+  { simbolo = simb_mais;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" mais ");
          return Parser::token::MAIS; 
}

\-  { simbolo = simb_menos;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" menos ");
          return Parser::token::MENOS; 
}

(<>)  {   simbolo = simb_diferenca;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" diferenca ");
          return Parser::token::DIFERENCA; 
}

(<=)  {   simbolo = simb_menor_igual;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" menor_igual ");
          return Parser::token::MENOR_IGUAL; 
}

(>=)  {   simbolo = simb_maior_igual;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" maior_igual ");
          return Parser::token::MAIOR_IGUAL; 
}

\<  {   simbolo = simb_menor;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" menor ");
          return Parser::token::MENOR; 
}

\>  {   simbolo = simb_maior;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" maior ");
          return Parser::token::MAIOR; 
}

{numero}  { simbolo = simb_numero;
            strncpy(meu_token, yytext, TAM_TOKEN);
            IMPRIME(" numero");
            return Parser::token::NUMERO;
 }

{ident} { simbolo = simb_identificador;
          strncpy (meu_token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return Parser::token::IDENT;
        }

.           /* no action on unmatched input */


 
%%
 
int main() {
    yyscan_t scanner;
    yylex_init(&scanner);
    bison::Parser parser{ scanner };
    std::cout.precision(10);
    parser.parse();
    yylex_destroy(scanner);
}