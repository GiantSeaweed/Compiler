//
// Created by 冯诗伟 on 2019-06-13.
//

#ifndef AST_MIPSBUILDER_H
#define AST_MIPSBUILDER_H

#include "../ast/ir/IRBuilderVisitor.h"
#include "../ast/ir/IRInstruction.h"

void mipsBuilder(vector<IRInstruction*> *irList);

void printMipsList();
#endif