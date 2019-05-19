//
// Created by 冯诗伟 on 2019-05-19.
//

#include "IRBuilderVisitor.h"
using namespace std;

bool IRBuilderVisitor::visit(Declarator &declarator) {

    return VisitorFalse::visit(declarator);
}

bool IRBuilderVisitor::visit(VarDec &varDec) {
    return VisitorFalse::visit(varDec);
}

bool IRBuilderVisitor::visit(NormalVarDec &normalVarDec) {
    return VisitorFalse::visit(normalVarDec);
}

bool IRBuilderVisitor::visit(ParamDec &paramDec) {
    return VisitorFalse::visit(paramDec);
}

bool IRBuilderVisitor::visit(FunDec &funDec) {
    return VisitorFalse::visit(funDec);
}

bool IRBuilderVisitor::visit(ArrayDec &arrayDec) {
    return VisitorFalse::visit(arrayDec);
}

bool IRBuilderVisitor::visit(Definition &definition) {
    return VisitorFalse::visit(definition);
}

bool IRBuilderVisitor::visit(Dec &Dec) {
    return VisitorFalse::visit(Dec);
}

bool IRBuilderVisitor::visit(NormalDec &normalDec) {
    return VisitorFalse::visit(normalDec);
}

bool IRBuilderVisitor::visit(InitializedDec &initializedDec) {
    return VisitorFalse::visit(initializedDec);
}

bool IRBuilderVisitor::visit(Exp &exp) {
    return VisitorFalse::visit(exp);
}

bool IRBuilderVisitor::visit(InfixExp &infixExp) {
    return VisitorFalse::visit(infixExp);
}

bool IRBuilderVisitor::visit(PrefixExp &prefixExp) {
    return VisitorFalse::visit(prefixExp);
}

bool IRBuilderVisitor::visit(ParenthesizedExp &parenthesizedExp) {
    return VisitorFalse::visit(parenthesizedExp);
}

bool IRBuilderVisitor::visit(ArrayExp &arrayExp) {
    return VisitorFalse::visit(arrayExp);
}

bool IRBuilderVisitor::visit(IDExp &idExp) {
    return VisitorFalse::visit(idExp);
}

bool IRBuilderVisitor::visit(StructExp &structExp) {
    return VisitorFalse::visit(structExp);
}

bool IRBuilderVisitor::visit(IntExp &intExp) {
    return VisitorFalse::visit(intExp);
}

bool IRBuilderVisitor::visit(FloatExp &floatExp) {
    return VisitorFalse::visit(floatExp);
}

bool IRBuilderVisitor::visit(FunExp &funExp) {
    return VisitorFalse::visit(funExp);
}

bool IRBuilderVisitor::visit(Program &program) {
    return VisitorFalse::visit(program);
}

bool IRBuilderVisitor::visit(ExtDef &extDef) {
    return VisitorFalse::visit(extDef);
}

bool IRBuilderVisitor::visit(DecDef &decDef) {
    return VisitorFalse::visit(decDef);
}

bool IRBuilderVisitor::visit(TypeDef &typeDef) {
    return VisitorFalse::visit(typeDef);
}

bool IRBuilderVisitor::visit(FunDef &funDef) {
    return VisitorFalse::visit(funDef);
}

bool IRBuilderVisitor::visit(Specifier &specifier) {
    return VisitorFalse::visit(specifier);
}

bool IRBuilderVisitor::visit(BasicSpecifier &basicSpecifier) {
    return VisitorFalse::visit(basicSpecifier);
}

bool IRBuilderVisitor::visit(StructSpecifier &structSpecifier) {
    return VisitorFalse::visit(structSpecifier);
}

bool IRBuilderVisitor::visit(NormalStructSpecifier &normalStructSpecifier) {
    return VisitorFalse::visit(normalStructSpecifier);
}

bool IRBuilderVisitor::visit(DefStructSpecifier &defStructSpecifier) {
    return VisitorFalse::visit(defStructSpecifier);
}

bool IRBuilderVisitor::visit(Stmt &stmt) {
    return VisitorFalse::visit(stmt);
}

bool IRBuilderVisitor::visit(CompSt &compSt) {
    return VisitorFalse::visit(compSt);
}

bool IRBuilderVisitor::visit(ExpStmt &expStmt) {
    return VisitorFalse::visit(expStmt);
}

bool IRBuilderVisitor::visit(ReturnStmt &returnStmt) {
    return VisitorFalse::visit(returnStmt);
}

bool IRBuilderVisitor::visit(IfElseStmt &ifElseStmt) {
    return VisitorFalse::visit(ifElseStmt);
}

bool IRBuilderVisitor::visit(WhileStmt &whileStmt) {
    return VisitorFalse::visit(whileStmt);
}
