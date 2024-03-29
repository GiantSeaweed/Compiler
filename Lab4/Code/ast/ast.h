//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_AST_H
#define AST_AST_H
#include "../tree.h"
// struct MultiNode;
#include "Declarator.h"
#include "Definition.h"
#include "Specifier.h"
#include "Expression.h"
#include "HighLevelDef.h"
#include "Statement.h"

#include "ASTNode.h"
#include "visitor/Visitor.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;


//  #define DEBUG
// #define SEMANTIC
//#define BUILD_IR
Program *transToAST(MultiNode *root);

#endif //AST_AST_H
