#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
// #include "lex.yy.c"
// extern FILE* yyin;
extern int yylex();
extern void yyrestart();
extern void yyparse();
extern int yydebug;
// int main(int argc, char **argv){
//     if (argc > 1){
//         if (!(yyin = fopen(argv[1], "r"))){
//             perror(argv[1]);
//             return 1;
//         }
//     }
//     while (yylex() != 0);
//     return 0;
// }
int main(int argc, char **argv)
{
    if (argc < 1)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);

    // yydebug = 1;
    yyparse();
    return 0;
}
