#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "tree.h"
#include "syntax.tab.h"


extern int yylex();
// extern void yyrestart();
extern int yyparse();
extern int yydebug;
extern int HAS_ERROR;
extern struct MultiNode* root;


extern void *yyin;


extern void cppParser(YYSTYPE *root);
extern int hasError;
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
    if(!HAS_ERROR){
        cppParser(root);
    }
    return 0;
}

