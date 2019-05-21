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

void IRBuilderVisitor::translateCond(Exp &exp, string label_true, string label_false) {
#ifdef BUILD_IR
    cout <<"Build IR translateCond"<<endl;
#endif
    InfixOperator infixOp = ((InfixExp*)&exp)->infixOp;

    if( infixOp == INFIX_GT || infixOp == INFIX_GE
      ||infixOp == INFIX_LT || infixOp == INFIX_LE
      ||infixOp == INFIX_NE || infixOp == INFIX_EQ){
        string myPlace1 = this->place = newPlace();
        ((InfixExp*)&exp)->leftSide->accept(*this);//code1
        string myPlace2 = this->place = newPlace();
        ((InfixExp*)&exp)->rightSide->accept(*this);//code2
        IROperator op;
        switch (infixOp) {
            case INFIX_GT: op = IR_IF_GT; break;
            case INFIX_GE: op = IR_IF_GE; break;
            case INFIX_LT: op = IR_IF_LT; break;
            case INFIX_LE: op = IR_IF_LE; break;
            case INFIX_NE: op = IR_IF_NE; break;
            case INFIX_EQ: op = IR_IF_EQ; break;
//          default: cerr << "Illegal operator at IRBuilderVisitor::InfixExp" << endl; break;
        }
        IRInstruction* irInstr = new IRInstruction(op, myPlace1,myPlace2,label_true );
        this->irList->push_back(irInstr); //code3
        irInstr = new IRInstruction(IR_GOTO, label_false);
        this->irList->push_back(irInstr); //goto

    }else if( infixOp == INFIX_AND){
        string label1 = newLabel();
        translateCond( (Exp&)(*((InfixExp*)&exp)->leftSide), label1, label_false);

        IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);

        translateCond( (Exp&)(*((InfixExp*)&exp)->rightSide), label_true, label_false);

    }else if( infixOp == INFIX_OR){
        string label1 = newLabel();
        translateCond( (Exp&)(*((InfixExp*)&exp)->leftSide), label_true, label1);

        IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);

        translateCond( (Exp&)(*((InfixExp*)&exp)->rightSide), label_true, label_false );

    }else if( ((PrefixExp*)&exp)->prefixOp == PREFIX_NOT){
        translateCond((Exp&)(*((PrefixExp*)&exp)->exp), label_false, label_true );
    }else{
        string myPlace = this->place = newPlace();
        exp.accept(*this);//code1

        IRInstruction* irInstr = new IRInstruction(IR_IF_NE, myPlace, "#0", label_true);
        this->irList->push_back(irInstr); // code2
        irInstr = new IRInstruction(IR_GOTO, label_false);
        this->irList->push_back(irInstr); // goto
    }

}

//void IRBuilderVisitor::translateCond(InfixExp &infixExp, string label_true, string label_false) {
//    //TODO
//}
//
//void IRBuilderVisitor::translateCond(PrefixExp &prefixExp, string label_true, string label_false) {
//    //TODO
//}

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
            string label1 = newLabel();
            string label2 = newLabel();
            string placeTemp = this->place;


            IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#0", placeTemp);
            this->irList->push_back(irInstr);//code0

            translateCond(infixExp, label1, label2);//code1

            //code2
            irInstr = new IRInstruction(IR_LABEL, label1);
            this->irList->push_back(irInstr);
            irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#1", placeTemp );
            this->irList->push_back(irInstr);
            irInstr = new IRInstruction(IR_LABEL, label2);
            this->irList->push_back(irInstr);

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
        //TODO
        string label1 = newLabel();
        string label2 = newLabel();
        IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#0", this->place);
        this->irList->push_back(irInstr);//code0

        translateCond(prefixExp, label1, label2);//code1

        //code2
        irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#1", this->place);
        this->irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_LABEL, label2);
        this->irList->push_back(irInstr);
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
