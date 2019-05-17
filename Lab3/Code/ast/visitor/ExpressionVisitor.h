//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_VISITOREXPRESSION_H
#define AST_VISITOREXPRESSION_H
//#include "../Symbol.h"
//#include "Visitor.h"
#include "TypeVisitor.h"
#include <set>
#include <string>

using namespace std;

struct ExpressionVisitor : VisitorTrue {
//    ExpressionVisitor(set<Symbol *, decltype(op)> *symTable);

    set<Symbol *, decltype(op)> *symTable;
    set<Symbol *, decltype(op)> *funcTable;

    bool inSymTable(const string &id);

    Symbol *findInSymTable(const string &id);

    ExpressionVisitor() = default;

    //Expression 10
    bool visit(InfixExp &infixExpression) override;

    bool visit(PrefixExp &prefixExpression) override;

    bool visit(ParenthesizedExp &parenthesizedExpression) override;

    bool visit(ArrayExp &expression) override;

    bool visit(IDExp &expression) override;

    bool visit(StructExp &expression) override;

    bool visit(IntExp &expression) override;

    bool visit(FloatExp &expression) override;

    bool visit(FunExp &expression) override;



    bool visit(DefStructSpecifier &specifier) override;

    bool inFuncTable(const string &id);

    Symbol *findInFuncTable(const string &id);
};
#endif //AST_VISITOREXPRESSION_H
