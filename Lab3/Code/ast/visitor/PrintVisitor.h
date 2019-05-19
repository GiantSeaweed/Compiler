//
// Created by 冯诗伟 on 2019-05-15.
//

#ifndef AST_PRINTVISITOR_H
#define AST_PRINTVISITOR_H
#include "Visitor.h"

struct PrintVisitor : Visitor {
    bool visit(Exp &exp) override;

    bool visit(InfixExp &infixExp) override;

    bool visit(ParenthesizedExp &parenthesizedExp) override;

    bool visit(PrefixExp &prefixExp) override;

    bool visit(IDExp &exp) override;

    bool visit(IntExp &exp) override;

    bool visit(FloatExp &exp) override;

    bool visit(FunExp &exp) override;

    bool visit(ArrayExp &exp) override;

    bool visit(StructExp &exp) override;

    bool visit(Specifier &specifier) override;

    bool visit(BasicSpecifier &specifier) override;

    bool visit(StructSpecifier &specifier) override;

    bool visit(DefStructSpecifier &specifier) override;

    bool visit(NormalStructSpecifier &specifier) override;

    bool visit(Declarator &declarator) override;

    bool visit(VarDec &declarator) override;

    bool visit(NormalVarDec &declarator) override;

    bool visit(ArrayDec &declarator) override;

    bool visit(ParamDec &declarator) override;

    bool visit(FunDec &declarator) override;

    bool visit(Dec &declarator) override;

    bool visit(NormalDec &declarator) override;

    bool visit(InitializedDec &declarator) override;

    bool visit(Definition &definition) override;

    bool visit(Stmt &stmt) override;

    bool visit(CompSt &stmt) override;

    bool visit(ExpStmt &stmt) override;

    bool visit(ReturnStmt &stmt) override;

    bool visit(IfElseStmt &stmt) override;

    bool visit(WhileStmt &stmt) override;

    bool visit(Program &hlDef) override;

    bool visit(ExtDef &hlDef) override;

    bool visit(DecDef &hlDef) override;

    bool visit(TypeDef &hlDef) override;

    bool visit(FunDef &hlDef) override;

    ~PrintVisitor() override;
};

#endif //AST_PRINTVISITOR_H
