#include "syntax.tab.h"
#include "ast/type.h"
#include "tree.h"
// #include "ast/Transform.h"


#include <iostream>
using namespace std;

extern "C"{
    void cppParser(MultiNode *root);
}

void cppParser(MultiNode *root)
{
    Program *p = transToAST(root);
    // printf("Hello\n");
}
