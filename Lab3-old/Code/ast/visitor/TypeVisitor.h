//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_TYPEVISITOR_H
#define AST_TYPEVISITOR_H


#include "../type.h"
#include "../ast.h"


#include "Visitor.h"
#include "../TypeSystem.h"
#include "../Symbol.h"
#include <set>

using namespace std;

auto op = [](Symbol *s1, Symbol *s2) -> bool {
    return s1->id < s2->id;
};

struct TypeVisitor : VisitorFalse {
    set<Symbol *, decltype(op)> *symbolTable;
    set<Symbol *, decltype(op)> *funTable;

    TypeVisitor();

    //Specifier
    bool visit(DefStructSpecifier &specifier) override;
    bool visit(NormalStructSpecifier &specifier) override;
    bool visit(BasicSpecifier &specifier) override;

    //Declarator
    bool visit(NormalVarDec &declarator) override;
    bool visit(ArrayDec &declarator) override;
    bool visit(FunDec &declarator) override;
    bool visit(ParamDec &declarator) override;

    //Definition
    bool visit(NormalDec &declarator) override;
    bool visit(InitializedDec &declarator) override;
    bool visit(Definition &definition) override;

    //High Level Definition
    bool visit(DecDef &hlDef) override;
    bool visit(TypeDef &hlDef) override;
    bool visit(FunDef &hlDef) override;
    bool visit(Program &hlDef) override;

    //Statement
    bool visit(CompSt &stmt) override;
    bool visit(IfElseStmt &stmt) override;
    bool visit(WhileStmt &stmt) override;

private:
    bool inStruct = false;

    static string genAnonymousName(ASTNode &node);

    bool checkAndAddIntoSymbolTable(Symbol *symbol);

    Symbol *lookupSymbolTable(Symbol *symbol);

    bool checkAndAddIntoFunTable(Symbol *symbol);
};

#endif //AST_TYPEVISITOR_H
