/* https://learnmoderncpp.com/2020/12/16/generating-c-programs-with-flex-and-bison-1/ */
%{
#include <cstdlib>
#include <iomanip>
#include "Parser.hpp"
#include "compilador.hpp"

using namespace bison;

// #define PADD std::left << std::setw(16) << std::setfill(' ')
// #define IMPRIME(STR) if(print) { std::cerr << PADD << STR << "\t# " << PADD << meu_token << num_line << '\n' ;}
#define IMPRIME(STR) {};

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

program {   meu_token = yytext;
            IMPRIME("program ");
            return Parser::token::PROGRAM;
}

label   {   meu_token = yytext;
            IMPRIME("label ");
            return Parser::token::LABEL;
}

type    {   meu_token = yytext;
            IMPRIME("type ");
            return Parser::token::TYPE;
}

array   {   meu_token = yytext;
            IMPRIME("array ");
            return Parser::token::ARRAY;
}

of      {   meu_token = yytext;
            IMPRIME("of ");
            return Parser::token::OF;
}

var     {   meu_token = yytext;
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

procedure { meu_token = yytext;
            IMPRIME("procedure ");
            return Parser::token::PROCEDURE;
}

function {  meu_token = yytext;
            IMPRIME("function ");
            return Parser::token::FUNCTION;
}

goto    {   meu_token = yytext;
            IMPRIME("goto ");
            return Parser::token::GOTO;
}

if      {   meu_token = yytext;
            IMPRIME("if ");
            return Parser::token::IF;
}

then    {   meu_token = yytext;
            IMPRIME("then ");
            return Parser::token::THEN;
}

else    {   meu_token = yytext;
            IMPRIME("else ");
            return Parser::token::ELSE;
}

while   {   meu_token = yytext;
            IMPRIME("while ");
            return Parser::token::WHILE;
}

do      {   meu_token = yytext;
            IMPRIME("do ");
            return Parser::token::DO;
}

or      {   meu_token = yytext;
            IMPRIME("or ");
            return Parser::token::OR;
} 

div     {   meu_token = yytext;
            IMPRIME("div ");
            return Parser::token::DIV;
}

\*      {   meu_token = yytext;
            IMPRIME("mult ");
            return Parser::token::MULT;
}

and     {   meu_token = yytext;
            IMPRIME("and ");
            return Parser::token::AND;
}

not     {   meu_token = yytext;
            IMPRIME("not ");
            return Parser::token::NOT;
}

true    {   meu_token = yytext;
            IMPRIME("true ");
            return Parser::token::TRUE;
}

false   {   meu_token = yytext;
            IMPRIME("false ");
            return Parser::token::FALSE;
}

begin   {   meu_token = yytext;
            IMPRIME("begin ");
            return Parser::token::T_BEGIN;
}

end     {   meu_token = yytext;
            IMPRIME("end ");
            return Parser::token::T_END;
}

read    {   meu_token = yytext;
            IMPRIME("read ");
            return Parser::token::READ;
}

write   {   meu_token = yytext;
            IMPRIME("write ");
            return Parser::token::WRITE;
}

:=      {   meu_token = yytext;
            IMPRIME(" atribuicao  ");
            return Parser::token::ATRIBUICAO;
}

\;      {   meu_token = yytext;
            IMPRIME(" pt_virgula  ");
            return Parser::token::PONTO_E_VIRGULA;
}

\:      {   meu_token = yytext;
            IMPRIME(" dois pontos ");
            return Parser::token::DOIS_PONTOS;
}

\,      {   meu_token = yytext;
            IMPRIME(" virgula  ");
            return Parser::token::VIRGULA;
}


\.      {   meu_token = yytext;
            IMPRIME(" ponto  ");
            return Parser::token::PONTO;
}

\(      {   meu_token = yytext;
            IMPRIME(" abre_parenteses  ");
            return Parser::token::ABRE_PARENTESES;
}

\)      {   meu_token = yytext;
            IMPRIME(" fecha_parenteses ");
            return Parser::token::FECHA_PARENTESES;
}

\+      {   meu_token = yytext;
            IMPRIME(" mais ");
            return Parser::token::MAIS; 
}

\-      {   meu_token = yytext;
            IMPRIME(" menos ");
            return Parser::token::MENOS; 
}

\=      {   meu_token = yytext;
            IMPRIME(" igualdade ");
            return Parser::token::IGUALDADE;
}

(<>)    {   meu_token = yytext;
            IMPRIME(" diferenca ");
            return Parser::token::DIFERENCA; 
}

(<=)    {   meu_token = yytext;
            IMPRIME(" menor_igual ");
            return Parser::token::MENOR_IGUAL; 
}

(>=)    {   meu_token = yytext;
            IMPRIME(" maior_igual ");
            return Parser::token::MAIOR_IGUAL; 
}

\<      {   meu_token = yytext;
            IMPRIME(" menor ");
            return Parser::token::MENOR; 
}

\>      {   meu_token = yytext;
            IMPRIME(" maior ");
            return Parser::token::MAIOR; 
}

{numero} {  meu_token = yytext;
            IMPRIME(" numero");
            return Parser::token::NUMERO;
}

{ident} {   meu_token = yytext;
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