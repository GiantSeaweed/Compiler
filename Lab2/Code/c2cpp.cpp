#include "syntax.tab.h"
#include "ast/ast.h"
#include "tree.h"

// extern void YYSTYPEtoString(YYSTYPE *src, int level);

#include <iostream>
using namespace std;

extern "C"{
    void cppParser(MultiNode *root);
}

void cppParser(MultiNode *root)
{
    Program *p = transToAST(root);
}
