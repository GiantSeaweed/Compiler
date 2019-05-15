//
// Created by 冯诗伟 on 2019-05-15.
//

#include "Visitor.h"
#include "../ast.h"
#include "../type.h"
#include <iostream>
#include "PrintVisitor.h"
#include <string>

#define TEST

using namespace std;

string getLineno(ASTNode &node) {
    return string("  ") + to_string(node.beginLine) + string(":::") + to_string(node.endLine) + string("  ");
}

bool PrintVisitor::visit(Exp &exp) {
#ifdef TEST
    cout << "Visit expression" << getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(InfixExp &infixExp) {
#ifdef TEST
    cout << "Visit InfixExp " << InfixOperator(infixExp.infixOp) << getLineno(infixExp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ParenthesizedExp &parenthesizedExp) {
#ifdef TEST
    cout << "Visit ParenthesizedExp" << getLineno(parenthesizedExp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(PrefixExp &prefixExp) {
#ifdef TEST
    cout << "Visit PrefixExp" << getLineno(prefixExp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(IDExp &exp) {
#ifdef TEST
    cout << "Visit IDExp:" <<exp.id<<", "<<getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(IntExp &exp) {
#ifdef TEST
    cout << "Visit IntExp" << getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(FloatExp &exp) {
#ifdef TEST
    cout << "Visit FloatExp" << getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(FunExp &exp) {
#ifdef TEST
    cout << "Visit FunExp" << getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ArrayExp &exp) {
#ifdef TEST
    cout << "Visit ArrayExp" << getLineno(exp) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(StructExp &exp) {
#ifdef TEST
    cout << "Visit StructExp" << getLineno(exp) << endl;
#endif
    return true;
}
bool PrintVisitor::visit(Specifier &specifier)
{
#ifdef TEST
    cout << "Visit Specifier" << getLineno(specifier) << endl;
#endif
    return true;
}
bool PrintVisitor::visit(BasicSpecifier &specifier) {
#ifdef TEST
    cout << "Visit BasicSpecifier" << getLineno(specifier) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(StructSpecifier &specifier) {
#ifdef TEST
    cout << "Visit StructSpecifier" << getLineno(specifier) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(DefStructSpecifier &specifier) {
#ifdef TEST
    cout << "Visit DefStructSpecifier" << getLineno(specifier) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(NormalStructSpecifier &specifier) {
#ifdef TEST
    cout << "Visit NormalStructSpecifier" << getLineno(specifier) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(Declarator &declarator) {
#ifdef TEST
    cout << "Visit declarator" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(VarDec &declarator) {
#ifdef TEST
    cout << "Visit VarDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(NormalVarDec &declarator) {
#ifdef TEST
    cout << "Visit NormalVarDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ArrayDec &declarator) {
#ifdef TEST
    cout << "Visit ArrayDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ParamDec &declarator) {
#ifdef TEST
    cout << "Visit ParamDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(FunDec &declarator) {
#ifdef TEST
    cout << "Visit FunDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(Dec &declarator) {
#ifdef TEST
    cout << "Visit Dec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(NormalDec &declarator) {
#ifdef TEST
    cout << "Visit NormalDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(InitializedDec &declarator) {
#ifdef TEST
    cout << "Visit InitializedDec" << getLineno(declarator) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(Definition &definition) {
#ifdef TEST
    cout << "Visit Definition" << getLineno(definition) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(Stmt &stmt) {
#ifdef TEST
    cout << "Visit Stmt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(CompSt &stmt) {
#ifdef TEST
    cout << "Visit CompSt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ExpStmt &stmt) {
#ifdef TEST
    cout << "Visit ExpStmt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ReturnStmt &stmt) {
#ifdef TEST
    cout << "Visit ReturnStmt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(IfElseStmt &stmt) {
#ifdef TEST
    cout << "Visit IfElseStmt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(WhileStmt &stmt) {
#ifdef TEST
    cout << "Visit WhileStmt" << getLineno(stmt) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(Program &hlDef) {
#ifdef TEST
    cout << "Visit Program" << getLineno(hlDef) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(ExtDef &hlDef) {
#ifdef TEST
    cout << "Visit ExtDef" << getLineno(hlDef) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(DecDef &hlDef) {
#ifdef TEST
    cout << "Visit DecDef" << getLineno(hlDef) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(TypeDef &hlDef) {
#ifdef TEST
    cout << "Visit TypeDef" << getLineno(hlDef) << endl;
#endif
    return true;
}

bool PrintVisitor::visit(FunDef &hlDef) {
#ifdef TEST
    cout << "Visit FunDef" << getLineno(hlDef) << endl;
#endif
    return true;
}

PrintVisitor::~PrintVisitor() = default;

