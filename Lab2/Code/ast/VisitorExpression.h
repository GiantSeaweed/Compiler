//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_VISITOREXPRESSION_H
#define AST_VISITOREXPRESSION_H
#include "Symbol.hh"
#include "Visitor.h"
//#include "AllTrueVisitor.hh"
#include <set>
#include <string>

using namespace std;

struct ExpressionVisitor : AllTrueVisitor {
//    ExpressionVisitor(set<Symbol *, decltype(op)> *symTable);

    set<Symbol *, decltype(op)> *symTable;
    set<Symbol *, decltype(op)> *funcTable;

    bool inSymTable(const string &id);

    Symbol *findInSymTable(const string &id);

    ExpressionVisitor() = default;

    bool visit(InfixExpression &infixExpression) override;

    bool visit(ParenthesizedExpression &parenthesizedExpression) override;

    bool visit(PrefixExpression &prefixExpression) override;

    bool visit(IDExpression &expression) override;

    bool visit(IntLiteral &expression) override;

    bool visit(FloatLiteral &expression) override;

    bool visit(MethodInvocation &expression) override;

    bool visit(ArrayAccess &expression) override;

    bool visit(FieldAccess &expression) override;

    bool visit(DefinitionStructSpecifier &specifier) override;

    bool inFuncTable(const string &id);

    Symbol *findInFuncTable(const string &id);
};
#endif //AST_VISITOREXPRESSION_H
