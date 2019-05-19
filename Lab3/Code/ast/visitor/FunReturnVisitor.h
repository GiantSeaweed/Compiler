//
// Created by 冯诗伟 on 2019-05-01.
//

#ifndef AST_FUNRETURNVISITOR_H
#define AST_FUNRETURNVISITOR_H


#include "Visitor.h"
#include <iostream>
using namespace std;

struct TypeSystem;

struct FunReturnVisitor : VisitorTrue {
    bool visit(ReturnStmt &stmt) override;

    bool visit(FunDef &hlDef) override;

private:
    bool inFunction;
    TypeSystem *retType;
};
#endif //AST_FUNRETURNVISITOR_H
