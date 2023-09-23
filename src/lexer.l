/* https://learnmoderncpp.com/2020/12/16/generating-c-programs-with-flex-and-bison-1/ */
%{
#include <cstdlib>
#include <iomanip>
#include "Parser.hpp"
#include "compilador.hpp"

using namespace bison;

#define PADD std::left << std::setw(16) << std::setfill(' ')
#define IMPRIME(STR) if(print) { std::cerr << PADD << STR << "\t# " << PADD << meu_token << num_line << '\n' ;}
// #define IMPRIME(STR) {};

%}

%option reentrant interactive noyywrap noyylineno nodefault outfile="Scanner.cpp" header="Scanner.hpp"

pulo_linha [\n]

descarte [\t ]+

ident [a-zA-Z][a-zA-Z0-9]*

numero [0-9]+

comentario "(*".*"*)"

comentario2 "//".*

%%

{pulo_linha} {num_line++;}

{descarte}

{comentario}

{comentario2}

program {   simbolo  = simb_program;
            meu_token = yytext;
            IMPRIME("program ");
            return Parser::token::PROGRAM;
}

label   {   simbolo = simb_label;
            meu_token = yytext;
            IMPRIME("label ");
            return Parser::token::LABEL;
}

type    {   simbolo = simb_type;
            meu_token = yytext;
            IMPRIME("type ");
            return Parser::token::TYPE;
}

array   {   simbolo = simb_array;
            meu_token = yytext;
            IMPRIME("array ");
            return Parser::token::ARRAY;
}

of      {   simbolo = simb_of;
            meu_token = yytext;
            IMPRIME("of ");
            return Parser::token::OF;
}

var     {   simbolo = simb_var;
            meu_token = yytext;
            IMPRIME("var  ");
            return Parser::token::VAR;
}

integer {   simbolo = simb_integer;
            meu_token = yytext;
            IMPRIME("integer ");
            return Parser::token::INTEGER;
}

boolean {   simbolo = simb_boolean;
            meu_token = yytext;
            IMPRIME("boolean ");
            return Parser::token::BOOLEAN;
}

procedure { simbolo = simb_procedure;
            meu_token = yytext;
            IMPRIME("procedure ");
            return Parser::token::PROCEDURE;
}

function {  simbolo = simb_function;
            meu_token = yytext;
            IMPRIME("function ");
            return Parser::token::FUNCTION;
}

goto    {   simbolo = simb_goto;
            meu_token = yytext;
            IMPRIME("goto ");
            return Parser::token::GOTO;
}

if      {   simbolo = simb_if;
            meu_token = yytext;
            IMPRIME("if ");
            return Parser::token::IF;
}

then    {   simbolo = simb_then;
            meu_token = yytext;
            IMPRIME("then ");
            return Parser::token::THEN;
}

else    {   simbolo = simb_else;
            meu_token = yytext;
            IMPRIME("else ");
            return Parser::token::ELSE;
}

while   {   simbolo = simb_while;
            meu_token = yytext;
            IMPRIME("while ");
            return Parser::token::WHILE;
}

do      {   simbolo = simb_do;
            meu_token = yytext;
            IMPRIME("do ");
            return Parser::token::DO;
}

or      {   simbolo = simb_or;
            meu_token = yytext;
            IMPRIME("or ");
            return Parser::token::OR;
} 

div     {   simbolo = simb_div;
            meu_token = yytext;
            IMPRIME("div ");
            return Parser::token::DIV;
}

\*      {   simbolo = simb_mult;
            meu_token = yytext;
            IMPRIME("mult ");
            return Parser::token::MULT;
}

and     {   simbolo = simb_and;
            meu_token = yytext;
            IMPRIME("and ");
            return Parser::token::AND;
}

not     {   simbolo = simb_not;
            meu_token = yytext;
            IMPRIME("not ");
            return Parser::token::NOT;
}

true    {   simbolo = simb_true;
            meu_token = yytext;
            IMPRIME("true ");
            return Parser::token::TRUE;
}

false   {   simbolo = simb_false;
            meu_token = yytext;
            IMPRIME("false ");
            return Parser::token::FALSE;
}

begin   {   simbolo = simb_begin;
            meu_token = yytext;
            IMPRIME("begin ");
            return Parser::token::T_BEGIN;
}

end     {   simbolo = simb_end;
            meu_token = yytext;
            IMPRIME("end ");
            return Parser::token::T_END;
}

read    {   simbolo = simb_read;
            meu_token = yytext;
            IMPRIME("read ");
            return Parser::token::READ;
}

write   {   simbolo = simb_write;
            meu_token = yytext;
            IMPRIME("write ");
            return Parser::token::WRITE;
}

:=      {   simbolo = simb_atribuicao;
            meu_token = yytext;
            IMPRIME(" atribuicao  ");
            return Parser::token::ATRIBUICAO;
}

\;      {   simbolo = simb_ponto_e_virgula;
            meu_token = yytext;
            IMPRIME(" pt_virgula  ");
            return Parser::token::PONTO_E_VIRGULA;
}

\:      {   simbolo = simb_dois_pontos;
            meu_token = yytext;
            IMPRIME(" dois pontos ");
            return Parser::token::DOIS_PONTOS;
}

\,      {   simbolo = simb_virgula;
            meu_token = yytext;
            IMPRIME(" virgula  ");
            return Parser::token::VIRGULA;
}


\.      {   simbolo = simb_ponto;
            meu_token = yytext;
            IMPRIME(" ponto  ");
            return Parser::token::PONTO;
}

\(      {   simbolo = simb_abre_parenteses;
            meu_token = yytext;
            IMPRIME(" abre_parenteses  ");
            return Parser::token::ABRE_PARENTESES;
}

\)      {   simbolo = simb_fecha_parenteses;
            meu_token = yytext;
            IMPRIME(" fecha_parenteses ");
            return Parser::token::FECHA_PARENTESES;
}

\+      {   simbolo = simb_mais;
            meu_token = yytext;
            IMPRIME(" mais ");
            return Parser::token::MAIS; 
}

\-      {   simbolo = simb_menos;
            meu_token = yytext;
            IMPRIME(" menos ");
            return Parser::token::MENOS; 
}

\=      {   simbolo = simb_igualdade;
            meu_token = yytext;
            IMPRIME(" igualdade ");
            return Parser::token::IGUALDADE;
}

(<>)    {   simbolo = simb_diferenca;
            meu_token = yytext;
            IMPRIME(" diferenca ");
            return Parser::token::DIFERENCA; 
}

(<=)    {   simbolo = simb_menor_igual;
            meu_token = yytext;
            IMPRIME(" menor_igual ");
            return Parser::token::MENOR_IGUAL; 
}

(>=)    {   simbolo = simb_maior_igual;
            meu_token = yytext;
            IMPRIME(" maior_igual ");
            return Parser::token::MAIOR_IGUAL; 
}

\<      {   simbolo = simb_menor;
            meu_token = yytext;
            IMPRIME(" menor ");
            return Parser::token::MENOR; 
}

\>      {   simbolo = simb_maior;
            meu_token = yytext;
            IMPRIME(" maior ");
            return Parser::token::MAIOR; 
}

{numero} {  simbolo = simb_numero;
            meu_token = yytext;
            IMPRIME(" numero");
            return Parser::token::NUMERO;
}

{ident} {   simbolo = simb_identificador;
            meu_token = yytext;
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