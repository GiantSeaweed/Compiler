//
// Created by 冯诗伟 on 2019-05-19.
//

#include "IRBuilderVisitor.h"
#include <iostream>
#include <string>
using namespace std;

string IRBuilderVisitor::findInIRSymTable(string key){
    return this->irSymbolTable->find(key)->second;
}

void IRBuilderVisitor::printIRList() {
    for(IRInstruction* irInstr : *irList){
        cout<<irInstr->toString()<<endl;
    }
}

bool IRBuilderVisitor::visit(Declarator &declarator) {

    return VisitorTrue::visit(declarator);
}

bool IRBuilderVisitor::visit(VarDec &varDec) {
    return VisitorTrue::visit(varDec);
}

bool IRBuilderVisitor::visit(NormalVarDec &normalVarDec) {
    return VisitorTrue::visit(normalVarDec);
}

bool IRBuilderVisitor::visit(ParamDec &paramDec) {
#ifdef BUILD_IR
    cout <<"Build IR ParamDec"<<endl;
#endif
    string myPlace = newPlace();
    IRInstruction *newInstr = new IRInstruction(IR_PARAM, myPlace);
    this->irList->push_back(newInstr);
    this->irSymbolTable->insert(pair<string,string>(paramDec.varDec->getID(), myPlace));
    return false;
}

bool IRBuilderVisitor::visit(FunDec &funDec) {
#ifdef BUILD_IR
    cout <<"Build IR FunDec"<<endl;
#endif
    IRInstruction* newInstr = new IRInstruction(IR_FUN, (funDec.id)->id);
    this->irList->push_back(newInstr);
    for(ParamDec *paramDec : *funDec.paramList){
        paramDec->accept(*this);
    }
//    cout<< irList->size() <<endl;
    return false;
}

bool IRBuilderVisitor::visit(ArrayDec &arrayDec) {
    return VisitorTrue::visit(arrayDec);
}

bool IRBuilderVisitor::visit(Definition &definition) {
    return VisitorTrue::visit(definition);
}

bool IRBuilderVisitor::visit(Dec &Dec) {
    return VisitorTrue::visit(Dec);
}

bool IRBuilderVisitor::visit(NormalDec &normalDec) {
    return VisitorTrue::visit(normalDec);
}

bool IRBuilderVisitor::visit(InitializedDec &initializedDec) {
    return VisitorTrue::visit(initializedDec);
}

bool IRBuilderVisitor::visit(Exp &exp) {
    return VisitorTrue::visit(exp);
}

bool IRBuilderVisitor::visit(InfixExp &infixExp) {
#ifdef BUILD_IR
    cout <<"Build IR InfixExp"<<endl;
#endif
    if(infixExp.leftSide->typeSystem->type == BASE_STRUCT){
        cout << "LHS is struct" <<endl;
    }else if(infixExp.leftSide->typeSystem->type == BASE_ARRAY){
        cout << "RHS is array" <<endl;
    }else {
        // ID
        if (infixExp.infixOp == INFIX_ASSIGN) {
            string var = irSymbolTable->find(((IDExp *) infixExp.leftSide)->id)->second;

            string placeTemp = this->place;
            string myPlace = this->place = newPlace();
            infixExp.rightSide->accept(*this);

            IRInstruction *irInstruction = new IRInstruction(IR_ASSIGN_SINGLE, myPlace, var);
            irList->push_back(irInstruction);
            irInstruction = new IRInstruction(IR_ASSIGN_SINGLE, var, placeTemp);
            irList->push_back(irInstruction);
        } else if (infixExp.infixOp == INFIX_PLUS
                   || infixExp.infixOp == INFIX_MINUS
                   || infixExp.infixOp == INFIX_STAR
                   || infixExp.infixOp == INFIX_DIV) {
            string placeTemp = this->place;
            string myPlace1 = this->place = newPlace();
            infixExp.leftSide->accept(*this);
            string myPlace2 = this->place = newPlace();
            infixExp.rightSide->accept(*this);

            IROperator op;
            switch (infixExp.infixOp) {
                case INFIX_PLUS:
                    op = IR_ASSIGN_PLUS;
                    break;
                case INFIX_MINUS:
                    op = IR_ASSIGN_MINUS;
                    break;
                case INFIX_STAR:
                    op = IR_ASSIGN_MUL;
                    break;
                case INFIX_DIV:
                    op = IR_ASSIGN_DIV;
                    break;
                default:
                    cerr << "Illegal operator at IRBuilderVisitor::InfixExp" << endl;
                    break;
            }
            IRInstruction *irInstr = new IRInstruction(op, myPlace1, myPlace2, placeTemp);

            this->irList->push_back(irInstr);
        } else if (infixExp.infixOp == INFIX_AND || infixExp.infixOp == INFIX_OR
                   || infixExp.infixOp == INFIX_GE || infixExp.infixOp == INFIX_GT
                   || infixExp.infixOp == INFIX_LE || infixExp.infixOp == INFIX_LT
                   || infixExp.infixOp == INFIX_NE || infixExp.infixOp == INFIX_EQ){
            //TODO
        }
    }
    return false;
}

bool IRBuilderVisitor::visit(PrefixExp &prefixExp) {
#ifdef BUILD_IR
    cout <<"Build IR PrefixExp"<<endl;
#endif
    if(prefixExp.prefixOp == PREFIX_MINUS){
        string placeTemp = this->place;
        string myPlace = this->place = newPlace();
        prefixExp.exp->accept(*this);

        IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_MINUS, "#0", myPlace, placeTemp);
        this->irList->push_back(irInstr);
    }else if(prefixExp.prefixOp == PREFIX_NOT){

    }else{
        cerr << "Illegal operator at IRBuilderVisitor::PrefixExp" << endl;
    }
    return false;
}

bool IRBuilderVisitor::visit(ParenthesizedExp &parenthesizedExp) {
    return VisitorTrue::visit(parenthesizedExp);
}

bool IRBuilderVisitor::visit(ArrayExp &arrayExp) {
    return VisitorTrue::visit(arrayExp);
}

bool IRBuilderVisitor::visit(IDExp &idExp) {
#ifdef BUILD_IR
    cout <<"Build IR IDExp : " << idExp.id <<endl;
#endif
    map<string, string>::iterator it = irSymbolTable->find(idExp.id);
    if (it == irSymbolTable->end()) {
        irSymbolTable->insert(pair<string, string>(idExp.id, newPlace()));
    } else {
        string var = it->second;
        IRInstruction *newInstr = new IRInstruction(IR_ASSIGN_SINGLE, var, this->place);
        this->irList->push_back(newInstr);//TODO
    }
    return false;
}

bool IRBuilderVisitor::visit(StructExp &structExp) {
    return VisitorTrue::visit(structExp);
}

bool IRBuilderVisitor::visit(IntExp &intExp) {
#ifdef BUILD_IR
    cout <<"Build IR IntExp : "<<intExp.value<<endl;
#endif
    int value = intExp.value;
    IRInstruction* newInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#"+to_string(value), this->place);
    irList->push_back(newInstr);
    return false;
}

bool IRBuilderVisitor::visit(FloatExp &floatExp) {
    return VisitorTrue::visit(floatExp);
}

bool IRBuilderVisitor::visit(FunExp &funExp) {
    return VisitorTrue::visit(funExp);
}

bool IRBuilderVisitor::visit(Program &program) {
    return VisitorTrue::visit(program);
}

bool IRBuilderVisitor::visit(ExtDef &extDef) {
    return VisitorTrue::visit(extDef);
}

bool IRBuilderVisitor::visit(DecDef &decDef) {
    return VisitorTrue::visit(decDef);
}

bool IRBuilderVisitor::visit(TypeDef &typeDef) {
    return VisitorTrue::visit(typeDef);
}

bool IRBuilderVisitor::visit(FunDef &funDef) {
    return VisitorTrue::visit(funDef);
}

bool IRBuilderVisitor::visit(Specifier &specifier) {
    return VisitorTrue::visit(specifier);
}

bool IRBuilderVisitor::visit(BasicSpecifier &basicSpecifier) {
    return VisitorTrue::visit(basicSpecifier);
}

bool IRBuilderVisitor::visit(StructSpecifier &structSpecifier) {
    return VisitorTrue::visit(structSpecifier);
}

bool IRBuilderVisitor::visit(NormalStructSpecifier &normalStructSpecifier) {
    return VisitorTrue::visit(normalStructSpecifier);
}

bool IRBuilderVisitor::visit(DefStructSpecifier &defStructSpecifier) {
    return VisitorTrue::visit(defStructSpecifier);
}

bool IRBuilderVisitor::visit(Stmt &stmt) {
    return VisitorTrue::visit(stmt);
}

bool IRBuilderVisitor::visit(CompSt &compSt) {
    return VisitorTrue::visit(compSt);
}

bool IRBuilderVisitor::visit(ExpStmt &expStmt) {
    return VisitorTrue::visit(expStmt);
}

bool IRBuilderVisitor::visit(ReturnStmt &returnStmt) {
#ifdef BUILD_IR
    cout <<"Build IR RetunStatment"<<endl;
#endif
    string placeTemp = this->place = newPlace();
    returnStmt.exp->accept(*this);

    IRInstruction* newInstr = new IRInstruction(IR_RETURN, placeTemp);
    this->irList->push_back(newInstr);
    return false;
}

bool IRBuilderVisitor::visit(IfElseStmt &ifElseStmt) {
    return VisitorTrue::visit(ifElseStmt);
}

bool IRBuilderVisitor::visit(WhileStmt &whileStmt) {
    return VisitorTrue::visit(whileStmt);
}
