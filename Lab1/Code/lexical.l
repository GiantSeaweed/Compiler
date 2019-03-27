%{
#include "syntax.tab.h"

int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
digit [0-9]
letter [_a-zA-Z]
relop ">"|"<"|">="|"<="|"=="|"!="
int 0|[1-9]{digit}*
id {letter}({digit}|{letter}){0,31}
float_norm {digit}+\.{digit}+
float_exp ({digit}*\.{digit}+|{digit}+\.)[Ee][+-]?{digit}+
float {float_norm}|{float_exp}
%option yylineno
%%
{int} { yylval.sval = strdup(yytext); return INT; }
{float} { yylval.sval = strdup(yytext); return FLOAT; }
";" { return SEMI; }
"," { return COMMA; }
"=" { return ASSIGNOP; }
">"|"<"|">="|"<="|"=="|"!=" { return RELOP; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"/" { return DIV; }
"&&" { return AND; }
"||" { return OR; }
"." { return DOT; }
"!" { return NOT; }
int|float {  yylval.sval = strdup(yytext); return TYPE; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"{" { return LC; }
"}" { return RC; }
"struct" { return STRUCT; }
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
{id} { yylval.sval = strdup(yytext); return ID; }
\n|\r { yycolumn = 1; }
" "|\t { }
. { printf("Error type A at Line %d: Mysterious characters \'%s\'.\n",
            yylineno, yytext);
    }
%%
