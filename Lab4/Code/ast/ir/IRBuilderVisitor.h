//
// Created by 冯诗伟 on 2019-05-19.
//

#ifndef AST_IRBUILDERVISITOR_H
#define AST_IRBUILDERVISITOR_H

#include "../visitor/Visitor.h"
#include "IRInstruction.h"
#include "../Symbol.h"
#include "../type.h"
#include "../ast.h"
#include "../TypeSystem.h"

#include <vector>
#include <map>
using namespace std;

struct IRBuilderVisitor : VisitorTrue {
    set<Symbol *, decltype(op)> *symbolTable;
    set<Symbol *, decltype(op)> *funTable;

    vector<IRInstruction*> *irList;
//    vector<string>*argList;
    map<string,string> *irSymbolTable;

    const string FUN_READ  = "read";
    const string FUN_WRITE = "write";
    // serve as parameters
    string place;
    string label_true;
    string label_false;

    int placeCounter = 0;
    int tempCounter = 0;

    IRBuilderVisitor() {
        place = "t0";
        irList = new vector<IRInstruction*>();
        irSymbolTable = new map<string,string>();

    }


    string newPlace() {
        return "t" + to_string(++tempCounter);
    }

    string newLabel() {
        return "label" + to_string(++placeCounter);
    }


    void printIRList();
    string findInIRSymTable(string key);

    void translateCond(Exp &exp, string label_true, string label_false);

    Symbol *findInSymTable(const string &id) {
        Symbol tmp(id);
        auto sym = this->symbolTable->find(&tmp);
        return sym != this->symbolTable->end() ? *sym : nullptr;
    }
    // Declarator.h 6
    virtual bool visit(Declarator &declarator) override ;

    virtual bool visit(VarDec &varDec) override ;

    virtual bool visit(NormalVarDec &normalVarDec) override ;

    virtual bool visit(ParamDec &paramDec) override ;

    virtual bool visit(FunDec &funDec) override ;

    virtual bool visit(ArrayDec &arrayDec) override ;

    //Definition.h 4
    virtual bool visit(Definition &definition) override ;

    virtual bool visit(Dec &Dec) override ;

    virtual bool visit(NormalDec &normalDec) override ;

    virtual bool visit(InitializedDec &initializedDec) override ;


    //Expression.h 10
    virtual bool visit(Exp &exp) override ;

    virtual bool visit(InfixExp &infixExp) override ;

    virtual bool visit(PrefixExp &prefixExp) override ;

    virtual bool visit(ParenthesizedExp &parenthesizedExp) override ;

    virtual bool visit(ArrayExp &arrayExp) override ;

    virtual bool visit(IDExp &idExp) override ;

    virtual bool visit(StructExp &structExp) override ;

    virtual bool visit(IntExp &intExp) override ;

    virtual bool visit(FloatExp &floatExp) override ;

    virtual bool visit(FunExp &funExp) override ;

    //HighLevelDef.h 5
    virtual bool visit(Program &program) override ;

    virtual bool visit(ExtDef &extDef) override ;

    virtual bool visit(DecDef &decDef) override ;

    virtual bool visit(TypeDef &typeDef) override ;

    virtual bool visit(FunDef &funDef) override ;

    //Specifier.h 5
    virtual bool visit(Specifier &specifier) override ;

    virtual bool visit(BasicSpecifier &basicSpecifier) override ;

    virtual bool visit(StructSpecifier &structSpecifier) override ;

    virtual bool visit(NormalStructSpecifier &normalStructSpecifier) override ;

    virtual bool visit(DefStructSpecifier &defStructSpecifier) override ;

    //Statement.h 6
    virtual bool visit(Stmt &stmt) override ;

    virtual bool visit(CompSt &compSt) override ;

    virtual bool visit(ExpStmt &expStmt) override ;

    virtual bool visit(ReturnStmt &returnStmt) override ;

    virtual bool visit(IfElseStmt &ifElseStmt) override ;

    virtual bool visit(WhileStmt &whileStmt) override ;
};

#endif //AST_IRBUILDERVISITOR_H
