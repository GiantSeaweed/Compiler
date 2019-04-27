//
// Created by fsw on 2019-04-27.
//

#ifndef CPP_SRC_VISITOR_HH
#define CPP_SRC_VISITOR_HH

#include "Declarator.h"
#include "Definition.h"
#include "Expression.h"
#include "HighLevelDef.h"
#include "Specifier.h"
#include "Statement.h"

struct Visitor {
    //Declarator.h 6
    virtual bool visit(Declarator &declarator) = 0;

    virtual bool visit(VarDec &varDec) = 0;

    virtual bool visit(NormalVarDec &normalVarDec) = 0;

    virtual bool visit(ParamDec &paramDec) = 0;

    virtual bool visit(FunDec &funDec) = 0;

    virtual bool visit(ArrayDec &arrayDec) = 0;


    //Definition.h 4
    virtual bool visit(Definition &definition) = 0;

    virtual bool visit(Dec &Dec) = 0;

    virtual bool visit(NormalDec &normalDec) = 0;

    virtual bool visit(InitializedDec &initializedDec) = 0;


    //Expression.h 10
    virtual bool visit(Exp &exp) = 0;

    virtual bool visit(InfixExp &infixExp) = 0;

    virtual bool visit(PrefixExp &prefixExp) = 0;

    virtual bool visit(ParenthesizedExp &parenthesizedExp) = 0;

    virtual bool visit(ArrayExp &arrayExp) = 0;

    virtual bool visit(IDExp &idExp) = 0;

    virtual bool visit(StructExp &structExp) = 0;

    virtual bool visit(IntExp &intExp) = 0;

    virtual bool visit(FloatExp &floatExp) = 0;

    virtual bool visit(FunExp &funExp) = 0;

    //HighLevelDef.h 5
    virtual bool visit(Program &program) = 0;

    virtual bool visit(ExtDef &extDef) = 0;

    virtual bool visit(DecDef &decDef) = 0;

    virtual bool visit(TypeDef &typeDef) = 0;

    virtual bool visit(FunDef &funDef) = 0;

    //Specifier.h 5
    virtual bool visit(Specifier &specifier) = 0;

    virtual bool visit(BasicSpecifier &basicSpecifier) = 0;

    virtual bool visit(StructSpecifier &structSpecifier) = 0;

    virtual bool visit(NormalStructSpecifier &normalStructSpecifier) = 0;

    virtual bool visit(DefStructSpecifier &defStructSpecifier) = 0;

    //Statement.h 6
    virtual bool visit(Stmt &stmt) = 0;

    virtual bool visit(CompSt &compSt) = 0;

    virtual bool visit(ExpStmt &expStmt) = 0;

    virtual bool visit(ReturnStmt &returnStmt) = 0;

    virtual bool visit(IfElseStmt &ifElseStmt) = 0;

    virtual bool visit(WhileStmt &whileStmt) = 0;

};

struct VisitorTrue : Visitor{
    //Declarator.h 6
    virtual bool visit(Declarator &declarator) override { return true; }

    virtual bool visit(VarDec &varDec) override { return true; }

    virtual bool visit(NormalVarDec &normalVarDec) override { return true; }

    virtual bool visit(ParamDec &paramDec) override { return true; }

    virtual bool visit(FunDec &funDec) override { return true; }

    virtual bool visit(ArrayDec &arrayDec) override { return true; }


    //Definition.h 4
    virtual bool visit(Definition &definition) override { return true; }

    virtual bool visit(Dec &Dec) override { return true; }

    virtual bool visit(NormalDec &normalDec) override { return true; }

    virtual bool visit(InitializedDec &initializedDec) override { return true; }


    //Expression.h 10
    virtual bool visit(Exp &exp) override { return true; }

    virtual bool visit(InfixExp &infixExp) override { return true; }

    virtual bool visit(PrefixExp &prefixExp) override { return true; }

    virtual bool visit(ParenthesizedExp &parenthesizedExp) override { return true; }

    virtual bool visit(ArrayExp &arrayExp) override { return true; }

    virtual bool visit(IDExp &idExp) override { return true; }

    virtual bool visit(StructExp &structExp) override { return true; }

    virtual bool visit(IntExp &intExp) override { return true; }

    virtual bool visit(FloatExp &floatExp) override { return true; }

    virtual bool visit(FunExp &funExp) override { return true; }

    //HighLevelDef.h 5
    virtual bool visit(Program &program) override { return true; }

    virtual bool visit(ExtDef &extDef) override { return true; }

    virtual bool visit(DecDef &decDef) override { return true; }

    virtual bool visit(TypeDef &typeDef) override { return true; }

    virtual bool visit(FunDef &funDef) override { return true; }

    //Specifier.h 5
    virtual bool visit(Specifier &specifier) override { return true; }

    virtual bool visit(BasicSpecifier &basicSpecifier) override { return true; }

    virtual bool visit(StructSpecifier &structSpecifier) override { return true; }

    virtual bool visit(NormalStructSpecifier &normalStructSpecifier) override { return true; }

    virtual bool visit(DefStructSpecifier &defStructSpecifier) override { return true; }

    //Statement.h 6
    virtual bool visit(Stmt &stmt) override { return true; }

    virtual bool visit(CompSt &compSt) override { return true; }

    virtual bool visit(ExpStmt &expStmt) override { return true; }

    virtual bool visit(ReturnStmt &returnStmt) override { return true; }

    virtual bool visit(IfElseStmt &ifElseStmt) override { return true; }

    virtual bool visit(WhileStmt &whileStmt) override { return true; }

};

struct VisitorFalse : Visitor{
    //Declarator.h 6
    virtual bool visit(Declarator &declarator) override { return false; }

    virtual bool visit(VarDec &varDec) override { return false; }

    virtual bool visit(NormalVarDec &normalVarDec) override { return false; }

    virtual bool visit(ParamDec &paramDec) override { return false; }

    virtual bool visit(FunDec &funDec) override { return false; }

    virtual bool visit(ArrayDec &arrayDec) override { return false; }


    //Definition.h 4
    virtual bool visit(Definition &definition) override { return false; }

    virtual bool visit(Dec &Dec) override { return false; }

    virtual bool visit(NormalDec &normalDec) override { return false; }

    virtual bool visit(InitializedDec &initializedDec) override { return false; }


    //Expression.h 10
    virtual bool visit(Exp &exp) override { return false; }

    virtual bool visit(InfixExp &infixExp) override { return false; }

    virtual bool visit(PrefixExp &prefixExp) override { return false; }

    virtual bool visit(ParenthesizedExp &parenthesizedExp) override { return false; }

    virtual bool visit(ArrayExp &arrayExp) override { return false; }

    virtual bool visit(IDExp &idExp) override { return false; }

    virtual bool visit(StructExp &structExp) override { return false; }

    virtual bool visit(IntExp &intExp) override { return false; }

    virtual bool visit(FloatExp &floatExp) override { return false; }

    virtual bool visit(FunExp &funExp) override { return false; }

    //HighLevelDef.h 5
    virtual bool visit(Program &program) override { return false; }

    virtual bool visit(ExtDef &extDef) override { return false; }

    virtual bool visit(DecDef &decDef) override { return false; }

    virtual bool visit(TypeDef &typeDef) override { return false; }

    virtual bool visit(FunDef &funDef) override { return false; }

    //Specifier.h 5
    virtual bool visit(Specifier &specifier) override { return false; }

    virtual bool visit(BasicSpecifier &basicSpecifier) override { return false; }

    virtual bool visit(StructSpecifier &structSpecifier) override { return false; }

    virtual bool visit(NormalStructSpecifier &normalStructSpecifier) override { return false; }

    virtual bool visit(DefStructSpecifier &defStructSpecifier) override { return false; }

    //Statement.h 6
    virtual bool visit(Stmt &stmt) override { return false; }

    virtual bool visit(CompSt &compSt) override { return false; }

    virtual bool visit(ExpStmt &expStmt) override { return false; }

    virtual bool visit(ReturnStmt &returnStmt) override { return false; }

    virtual bool visit(IfElseStmt &ifElseStmt) override { return false; }

    virtual bool visit(WhileStmt &whileStmt) override { return false; }

};

#endif //CPP_SRC_VISITOR_HH
