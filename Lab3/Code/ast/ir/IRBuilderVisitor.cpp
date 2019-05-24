//
// Created by 冯诗伟 on 2019-05-19.
//

#include "IRBuilderVisitor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include<typeinfo>
using namespace std;

string IRBuilderVisitor::findInIRSymTable(string key){
    return this->irSymbolTable->find(key)->second;
}

void IRBuilderVisitor::printIRList() {
    if(outFilename[0] != 0){
        ofstream out(outFilename);
        if (!out.is_open()){
            cerr << outFilename << " open failed!" << endl;
        }
        for (IRInstruction *irInstr : *irList){
            out << irInstr->toString()+"\n";
        }
        out.close();
    }
    for (IRInstruction *irInstr : *irList){
        cout << irInstr->toString() << endl;
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
#ifdef BUILD_IR
        cout <<"\ttranslateCond-Infix-RELOP"<<endl;
#endif
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
#ifdef BUILD_IR
        cout <<"\ttranslateCond-Infix-AND"<<endl;
#endif
        string label1 = newLabel();
        translateCond( (Exp&)(*((InfixExp*)&exp)->leftSide), label1, label_false);

        IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);

        translateCond( (Exp&)(*((InfixExp*)&exp)->rightSide), label_true, label_false);

    }else if( infixOp == INFIX_OR){
#ifdef BUILD_IR
        cout <<"\ttranslateCond-Infix-OR"<<endl;
#endif
        string label1 = newLabel();
        translateCond( (Exp&)(*((InfixExp*)&exp)->leftSide), label_true, label1);

        IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);

        translateCond( (Exp&)(*((InfixExp*)&exp)->rightSide), label_true, label_false );

    }
//    else if( ((PrefixExp*)&exp)->prefixOp == PREFIX_NOT){
//#ifdef BUILD_IR
//        cout <<"\ttranslateCond-Prefix-NOT"<<endl;
//#endif
//        cout << (((IDExp*)&exp)->id) << endl;
//        translateCond((Exp&)(*((PrefixExp*)&exp)->exp), label_false, label_true );
//    }
    else{
#ifdef BUILD_IR
        cout <<"\ttranslateCond-else"<<endl;
#endif
        string myPlace = this->place = newPlace();
        (&exp)->accept(*this);//code1
        IRInstruction* irInstr = new IRInstruction(IR_IF_NE, myPlace, "#0", label_true);
        this->irList->push_back(irInstr); // code2
        irInstr = new IRInstruction(IR_GOTO, label_false);
        this->irList->push_back(irInstr); // goto
    }

}

bool IRBuilderVisitor::visit(Declarator &declarator) {
#ifdef BUILD_IR
    cout <<"Build IR Declarator"<<endl;
#endif
    return VisitorTrue::visit(declarator);
}

bool IRBuilderVisitor::visit(VarDec &varDec) {
#ifdef BUILD_IR
    cout <<"Build IR VarDec"<<endl;
#endif
    return VisitorTrue::visit(varDec);
}

bool IRBuilderVisitor::visit(NormalVarDec &normalVarDec) {
#ifdef BUILD_IR
    cout <<"Build IR NormalVarDec"<<endl;
#endif
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
        if(typeid(*paramDec->varDec) == typeid(ArrayDec)){
            cerr << "Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type." <<endl;
            exit(0);
        }
        paramDec->accept(*this);
    }

//    cout<< irList->size() <<endl;
    return false;
}

bool IRBuilderVisitor::visit(ArrayDec &arrayDec) {
#ifdef BUILD_IR
    cout <<"Build IR ArrayDec"<<endl;
#endif
    string arrayID = arrayDec.varDec->getID();
    if(typeid(*arrayDec.varDec) == typeid(ArrayDec)){//TODO
        cerr << "Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type." <<endl;
        exit(0);
    }
#ifdef BUILD_IR
        cout <<"\tNo array " << arrayID << " before. Newly inserted in irSymTable!" <<endl;
#endif


    string myPlace = newPlace();
    irSymbolTable->insert(pair<string, string>(arrayID, myPlace));

    int size = arrayDec.size;
    IRInstruction *newInstr = new IRInstruction(IR_DEC, to_string(size * 4), myPlace);
    this->irList->push_back(newInstr);

    return false;
}

bool IRBuilderVisitor::visit(Definition &definition) {
#ifdef BUILD_IR
    cout <<"Build IR Definition"<<endl;
#endif
//    TODO
//    if(typeid(*definition.specifier) == typeid(NormalStructSpecifier)){
//        definition.specifier->accept(*this);
//        string tag = ((NormalStructSpecifier*) definition.specifier)->tag->id;
//        map<string, string>::iterator it = irSymbolTable->find(tag);
//        it->second = newPlace();
//
//        for(Dec* dec : *definition.decList)
//    }
    return VisitorTrue::visit(definition);
}

bool IRBuilderVisitor::visit(Dec &Dec) {
#ifdef BUILD_IR
    cout <<"Build IR Dec"<<endl;
#endif
    return VisitorTrue::visit(Dec);
}

bool IRBuilderVisitor::visit(NormalDec &normalDec) {
#ifdef BUILD_IR
    cout <<"Build IR NormalDec"<<endl;
#endif
    string id = normalDec.varDec->getID();

    string var = newPlace();
    irSymbolTable->insert(pair<string, string>(id, var));
    if(typeid(*normalDec.varDec) == typeid(ArrayDec)) {
        if(typeid(*((ArrayDec*)(normalDec.varDec))->varDec) == typeid(ArrayDec)){
            cerr << "Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type." <<endl;
            exit(0);
        }
        int length = normalDec.varDec->typeSystem->getBufLength();
        IRInstruction *irInstruction = new IRInstruction(IR_DEC, to_string(length), var);
        irList->push_back(irInstruction);
    }

    return false;
}

bool IRBuilderVisitor::visit(InitializedDec &initializedDec) {
#ifdef BUILD_IR
    cout <<"Build IR InitializedDec"<<endl;
#endif

    string id = ((NormalVarDec *) initializedDec.varDec)->getID();

//    string var = findInIRSymTable(id);
    string var = newPlace();
    irSymbolTable->insert(pair<string, string>(id, var));

    string placeTemp = this->place;
    string myPlace = this->place = newPlace();
    initializedDec.initialValue->accept(*this);

    IRInstruction *irInstruction = new IRInstruction(IR_ASSIGN_SINGLE, myPlace, var);
    irList->push_back(irInstruction);

    return false;
}

bool IRBuilderVisitor::visit(Exp &exp) {
#ifdef BUILD_IR
    cout <<"Build IR Exp"<<endl;
#endif
    return VisitorTrue::visit(exp);
}

bool IRBuilderVisitor::visit(InfixExp &infixExp) {
#ifdef BUILD_IR
    cout <<"Build IR InfixExp"<<endl;
//    cout <<"\tLeft:"<< infixExp.leftSide->typeSystem->type << endl;
#endif
    if(infixExp.infixOp == INFIX_ASSIGN
       && typeid(*infixExp.leftSide) == typeid(StructExp)){
#ifdef BUILD_IR
        cout <<"\tLHS is struct"<<endl;
#endif
        StructExp* structExp = (StructExp*)(infixExp.leftSide);
        string placeTemp = this->place;
        string myPlace = newPlace(); //exp.id
        string baseAddr = this->place = newPlace();
        structExp->structExp->accept(*this);


        string structID = structExp->structID->id;
#ifdef BUILD_IR
        cout <<"\t\tLHS is struct "<< structID <<endl;
#endif
        int offset = ((StructType *) structExp->structExp->typeSystem)->getOffset(structID);
        IRInstruction *irInstr ;
//        if(typeid(*structExp->structExp) == typeid(IDExp)){
//            irInstr = new IRInstruction(IR_ASSIGN_PLUS, "&" +baseAddr, "#"+to_string(offset),
//                                        myPlace);
//        } else{
            irInstr = new IRInstruction(IR_ASSIGN_PLUS, baseAddr, "#"+to_string(offset),
                                        myPlace);
//        }
        irList->push_back(irInstr);

        string placeExp2 = this->place = newPlace();
        infixExp.rightSide->accept(*this);

        irInstr = new IRInstruction(IR_ASSIGN_SINGLE, placeExp2, "*"+baseAddr);
        irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE,"*"+baseAddr, placeTemp);
        irList->push_back(irInstr);

    }else if(infixExp.infixOp == INFIX_ASSIGN
            && typeid(*infixExp.leftSide) == typeid(ArrayExp)){
#ifdef BUILD_IR
        cout <<"\tLHS is array"<<endl;
#endif
        //1-dimensional array
        string arrayID = ((IDExp*)(((ArrayExp*)infixExp.leftSide)->arrayExp))->id;
        string var = findInIRSymTable(arrayID);
        string myPlace = newPlace();//indexPlace * 4
        string addrPlace = newPlace(); //addr

        string placeTemp = this->place;
        string indexPlace = this->place = newPlace();
        ((ArrayExp*)infixExp.leftSide)->arrayIndex->accept(*this);//stored in indexPlace

        IRInstruction *newInstr = new IRInstruction(IR_ASSIGN_MUL, indexPlace, "#4", myPlace);
        this->irList->push_back(newInstr);
        newInstr = new IRInstruction(IR_ASSIGN_PLUS, "&"+var,  myPlace, addrPlace);
        this->irList->push_back(newInstr);

        string placeExp2 = this->place = newPlace();
        infixExp.rightSide->accept(*this);

        IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_SINGLE, placeExp2, "*"+addrPlace);
        irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE,"*"+addrPlace, placeTemp);
        irList->push_back(irInstr);

    }else {
        // ID
        if (infixExp.infixOp == INFIX_ASSIGN) {
#ifdef BUILD_IR
            cout <<"\tLHS is ID,assign"<<endl;
#endif
            string id = ((IDExp *) infixExp.leftSide)->id;
            string var = findInIRSymTable(id);
            string placeTemp = this->place;
            string myPlace = this->place = newPlace();
            infixExp.rightSide->accept(*this);
            IRInstruction *irInstruction = new IRInstruction(IR_ASSIGN_SINGLE, myPlace, var);
            irList->push_back(irInstruction);
            irInstruction = new IRInstruction(IR_ASSIGN_SINGLE, var, placeTemp);
            irList->push_back(irInstruction);
        } else if (infixExp.infixOp == INFIX_PLUS || infixExp.infixOp == INFIX_MINUS
                   || infixExp.infixOp == INFIX_STAR|| infixExp.infixOp == INFIX_DIV) {
#ifdef BUILD_IR
            cout <<"\tLHS is ID, +-*/"<<endl;
#endif
            string placeTemp = this->place;
            string myPlace1 = this->place = newPlace();
            infixExp.leftSide->accept(*this);
            string myPlace2 = this->place = newPlace();
            infixExp.rightSide->accept(*this);

            IROperator op;
            switch (infixExp.infixOp) {
                case INFIX_PLUS: op = IR_ASSIGN_PLUS; break;
                case INFIX_MINUS:op = IR_ASSIGN_MINUS;break;
                case INFIX_STAR: op = IR_ASSIGN_MUL;  break;
                case INFIX_DIV:  op = IR_ASSIGN_DIV;  break;
                default: cerr << "Illegal operator at IRBuilderVisitor::InfixExp" << endl;break;
            }
            IRInstruction *irInstr = new IRInstruction(op, myPlace1, myPlace2, placeTemp);
            this->irList->push_back(irInstr);

        } else if (infixExp.infixOp == INFIX_AND || infixExp.infixOp == INFIX_OR
                   || infixExp.infixOp == INFIX_GE || infixExp.infixOp == INFIX_GT
                   || infixExp.infixOp == INFIX_LE || infixExp.infixOp == INFIX_LT
                   || infixExp.infixOp == INFIX_NE || infixExp.infixOp == INFIX_EQ){
#ifdef BUILD_IR
            cout <<"\tLHS is ID, relop+and+or"<<endl;
#endif
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
#ifdef BUILD_IR
        cout <<"\tLHS is ID, but nothing"<<endl;
#endif
    }
    return false;
}

bool IRBuilderVisitor::visit(PrefixExp &prefixExp) {

    if(prefixExp.prefixOp == PREFIX_MINUS){
#ifdef BUILD_IR
        cout <<"Build IR PrefixExp::MINUS"<<endl;
#endif
        string placeTemp = this->place;
        string myPlace = this->place = newPlace();
        prefixExp.exp->accept(*this);

        IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_MINUS, "#0", myPlace, placeTemp);
        this->irList->push_back(irInstr);
    }else if(prefixExp.prefixOp == PREFIX_NOT){
#ifdef BUILD_IR
        cout <<"Build IR PrefixExp::NOT"<<endl;
#endif
        string label1 = newLabel();
        string label2 = newLabel();

        string placeTemp = this->place;
        IRInstruction* irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#0", placeTemp);
        this->irList->push_back(irInstr);//code0

        translateCond(*(prefixExp.exp), label2, label1);//code1

        //code2
        irInstr = new IRInstruction(IR_LABEL, label1);
        this->irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "#1", placeTemp);
        this->irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_LABEL, label2);
        this->irList->push_back(irInstr);
    }else{
        cerr << "Illegal operator at IRBuilderVisitor::PrefixExp" << endl;
    }
    return false;
}

bool IRBuilderVisitor::visit(ParenthesizedExp &parenthesizedExp) {
#ifdef BUILD_IR
    cout <<"Build IR ParenthesizedExp"<<endl;
#endif

    return VisitorTrue::visit(parenthesizedExp);
}

bool IRBuilderVisitor::visit(ArrayExp &arrayExp) {
#ifdef BUILD_IR
    cout <<"Build IR Array : " << endl;// ((IDExp*)(arrayExp.arrayExp))->id <<endl;
#endif
    string arrayID = ((IDExp*)(arrayExp.arrayExp))->id;
    map<string, string>::iterator it = irSymbolTable->find(arrayID);
    if (it == irSymbolTable->end()) {
        cerr << "Error in IRBuilderVisitor::ArrayExp"<<endl;
    }
    else{
#ifdef BUILD_IR
        cout <<"\tWe already have array " << arrayID << " in irSymTable." <<endl;
#endif
        string var = it->second;
        string myPlace = newPlace();//indexPlace * 4
        string addrPlace = newPlace(); //addr

        string placeTemp = this->place;
        string indexPlace = this->place = newPlace();

        arrayExp.arrayIndex->accept(*this);//stored in indexPlace

        IRInstruction *newInstr = new IRInstruction(IR_ASSIGN_MUL, indexPlace, "#4", myPlace);
        this->irList->push_back(newInstr);

        newInstr = new IRInstruction(IR_ASSIGN_PLUS, "&"+var,  myPlace, addrPlace);
        this->irList->push_back(newInstr);

        newInstr = new IRInstruction(IR_ASSIGN_SINGLE, "*"+addrPlace, placeTemp);
        this->irList->push_back(newInstr);
    }
    return false;

}

bool IRBuilderVisitor::visit(IDExp &idExp) {
#ifdef BUILD_IR
    cout <<"Build IR IDExp : " << idExp.id <<endl;
#endif
    map<string, string>::iterator it = irSymbolTable->find(idExp.id);
    if (it == irSymbolTable->end()) {
#ifdef BUILD_IR
        cout <<"\tNo " << idExp.id << " before. Newly inserted in irSymTable!" <<endl;
#endif
        irSymbolTable->insert(pair<string, string>(idExp.id, this->place));
    } else {
#ifdef BUILD_IR
        cout <<"\tWe already have " << idExp.id << " in irSymTable." <<endl;
#endif
        string var = it->second;

        IRInstruction *newInstr = new IRInstruction(IR_ASSIGN_SINGLE, var, this->place);
        this->irList->push_back(newInstr);//TODO
    }
    return false;
}

bool topLevel = true;
bool topLevelTemp = true;
bool IRBuilderVisitor::visit(StructExp &structExp) {
#ifdef BUILD_IR
    cout <<"Build IR StructExp "<<endl;
#endif
    string structID = structExp.structID->id;


        string placeTemp = this->place;
        string myPlace = newPlace(); //exp.id
        string baseAddr = this->place = newPlace();
        structExp.structExp->accept(*this);

//        for(Symbol* symbol: *(structExp.structExp->symbolTable){
//            cout << symbol->id<<endl;
//        }
        SymbolHead* symbolHead;
        if(typeid(*structExp.structExp) == typeid(StructExp)){
            cout <<"\t\tFSW:"<<endl;
            for(SymbolHead* symbolTemp: (*((StructType *) structExp.structExp->typeSystem)->symbolTable)){
                cout <<"\t\tFSW:" << symbolTemp->getId()<<endl;
                if(symbolTemp->getId() == structID){
                    symbolHead = symbolTemp;
                    break;
                }
            }
            structExp.typeSystem = symbolHead->getTypeSystem();//findInSymTable(structID)->typeSystem;
        }
        else {

        }

#ifdef BUILD_IR
    cout <<"\tIR StructID : "<<structID<<endl;
//    cout << ((StructType *) structExp.typeSystem)->toString()<<endl;
    cout <<( typeid(*structExp.structExp) == typeid(IDExp) )<<endl;

#endif
//        Symbol* symbol = findInSymTable(structID);
//        if(symbol == nullptr) exit(0);
        int offset = ((StructType *) structExp.structExp->typeSystem)->getOffset(structID);//((StructType *) structExp.typeSystem)->getOffset(structID);
        cout <<offset<<endl;
        IRInstruction *irInstr ;

        if(typeid(*structExp.structExp) == typeid(IDExp)){
            irInstr = new IRInstruction(IR_ASSIGN_PLUS, "&" +baseAddr, "#"+to_string(offset),
                                        myPlace);
        } else{
            irInstr = new IRInstruction(IR_ASSIGN_PLUS, baseAddr, "#"+to_string(offset),
                                        myPlace);
        }
        irList->push_back(irInstr);
    cout <<"FSW"<<endl;
    if(topLevel) {
        topLevel = false;
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "*" + myPlace, placeTemp);
        irList->push_back(irInstr);

        topLevel = true;
    }else{
//        string placeTemp = this->place;
//        string myPlace = newPlace();
//        string baseAddr = this->place = newPlace();
//        structExp.structExp->accept(*this);
//
//        int offset = ((StructType *) structExp.structExp->typeSystem)->getOffset(structExp.structID->id);
//
//        IRInstruction *irInstr;
//        if(typeid(*structExp.structExp) == typeid(IDExp)){
//            irInstr = new IRInstruction(IR_ASSIGN_PLUS, "&" +baseAddr, "#"+to_string(offset),
//                                                       myPlace);
//        } else{
//            irInstr = new IRInstruction(IR_ASSIGN_PLUS, baseAddr, "#"+to_string(offset),
//                                        myPlace);
//        }
//
//        irList->push_back(irInstr);
        irInstr = new IRInstruction(IR_ASSIGN_SINGLE, myPlace, placeTemp);
        irList->push_back(irInstr);
    }
    return false;
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
#ifdef BUILD_IR
    cout <<"Build IR FloatExp "<<endl;
#endif
    return VisitorTrue::visit(floatExp);
}

bool IRBuilderVisitor::visit(FunExp &funExp) {
#ifdef BUILD_IR
    cout << "Build IR FunExp : " << endl;
#endif
    IRInstruction* irInstr;
    if(funExp.args->size() == 0){
        if(funExp.funID->id == FUN_READ){
            irInstr = new IRInstruction(IR_READ, this->place);
        }else{
            irInstr = new IRInstruction(IR_CALL,funExp.funID->id, this->place );
        }
        irList->push_back(irInstr);
    }else{
        vector<string>* argList = new vector<string>;
        string placeTemp = this->place;
        for(Exp* arg : *funExp.args){
            string myPlace = this->place = newPlace();
            arg->accept(*this);
            argList->insert(argList->begin(), myPlace); // inverted!
        }

        if(funExp.funID->id == FUN_WRITE){
            irInstr = new IRInstruction(IR_WRITE, argList->at(0));
            this->irList->push_back(irInstr);
        }else{
            int length = argList->size();
            for(int i = 0;i<length;i++){
                irInstr = new IRInstruction(IR_ARG, argList->at(i));
                this->irList->push_back(irInstr);
            }
            irInstr = new IRInstruction(IR_CALL,funExp.funID->id,placeTemp);
            this->irList->push_back(irInstr);
        }


    }

    return false;
}

bool IRBuilderVisitor::visit(Program &program) {
#ifdef BUILD_IR
    cout << "Build IR Program " << endl;
#endif
    return VisitorTrue::visit(program);
}

bool IRBuilderVisitor::visit(ExtDef &extDef) {
#ifdef BUILD_IR
    cout << "Build IR ExtDef" << endl;
#endif
    return VisitorTrue::visit(extDef);
}

bool IRBuilderVisitor::visit(DecDef &decDef) {
#ifdef BUILD_IR
    cout << "Build IR DecDef " << endl;
#endif
    return false;//TODO
}

bool IRBuilderVisitor::visit(TypeDef &typeDef) {
#ifdef BUILD_IR
    cout << "Build IR TypeDef " << endl;
#endif
    return false;//TODO
}

bool IRBuilderVisitor::visit(FunDef &funDef) {
#ifdef BUILD_IR
    cout << "Build IR FunDef " << endl;
#endif
    return VisitorTrue::visit(funDef);
}

bool IRBuilderVisitor::visit(Specifier &specifier) {
#ifdef BUILD_IR
    cout << "Build IR Specifier " << endl;
#endif
    return VisitorTrue::visit(specifier);
}

bool IRBuilderVisitor::visit(BasicSpecifier &basicSpecifier) {
#ifdef BUILD_IR
    cout << "Build IR BasicSpecifier " << endl;
#endif
    return VisitorTrue::visit(basicSpecifier);
}

bool IRBuilderVisitor::visit(StructSpecifier &structSpecifier) {
#ifdef BUILD_IR
    cout << "Build IR StructSpecifier " << endl;
#endif
    return VisitorTrue::visit(structSpecifier);
}

bool IRBuilderVisitor::visit(NormalStructSpecifier &normalStructSpecifier) {
#ifdef BUILD_IR
    cout << "Build IR NormalStructSpecifier " << endl;
#endif
//    string structID = normalStructSpecifier.tag->id;
//
//    int bufLength = normalStructSpecifier.typeSystem->getBufLength();
//    IRInstruction* irInstr = new IRInstruction(IR_DEC, to_string(bufLength),this->place);
//    irList->push_back(irInstr);

//    string myPlace = newPlace();TODO
//    irInstr = new IRInstruction(IR_ASSIGN_SINGLE, "&"+this->place, myPlace);
//    irList->push_back(irInstr);
//    cout << "\tFSW insert" << structID <<", "<<myPlace<<endl;
//    irSymbolTable->insert(pair<string,string>(structID, myPlace));

    string structID = normalStructSpecifier.tag->id;
    string myPlace = this->place;
    irSymbolTable->insert(pair<string,string>(structID, myPlace));
    int bufLength = normalStructSpecifier.typeSystem->getBufLength();
    IRInstruction* irInstr = new IRInstruction(IR_DEC, to_string(bufLength),myPlace);
    irList->push_back(irInstr);
    return false;
}

bool IRBuilderVisitor::visit(DefStructSpecifier &defStructSpecifier) {
#ifdef BUILD_IR
    cout << "Build IR DefStructSpecifier " << endl;
#endif

    return VisitorTrue::visit(defStructSpecifier);
}

bool IRBuilderVisitor::visit(Stmt &stmt) {
    return VisitorTrue::visit(stmt);
}

bool IRBuilderVisitor::visit(CompSt &compSt) {
#ifdef BUILD_IR
    cout << "Build IR CompSt " << endl;
#endif
    return VisitorTrue::visit(compSt);
}

bool IRBuilderVisitor::visit(ExpStmt &expStmt) {
#ifdef BUILD_IR
    cout << "Build IR ExpStmt " << endl;
#endif
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
#ifdef BUILD_IR
    cout <<"Build IR IfElseStmt"<<endl;
#endif

    string label1 = newLabel();
    string label2 = newLabel();
    string label3;
    translateCond(*(ifElseStmt.condition), label1, label2);//code1

    IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
    this->irList->push_back(irInstr);  //label1

    ifElseStmt.thenBody->accept(*this);//code2

    if(ifElseStmt.elseBody != nullptr){
        label3 = newLabel();
        irInstr = new IRInstruction(IR_GOTO, label3);
        this->irList->push_back(irInstr);  //goto label3
    }

    irInstr = new IRInstruction(IR_LABEL, label2);
    this->irList->push_back(irInstr);  //label2

    if(ifElseStmt.elseBody != nullptr){
        ifElseStmt.elseBody->accept(*this);//code3
        irInstr = new IRInstruction(IR_LABEL, label3);
        this->irList->push_back(irInstr);  // label3
    }


    return false;
}

bool IRBuilderVisitor::visit(WhileStmt &whileStmt) {
#ifdef BUILD_IR
    cout <<"Build IR IfElseStmt"<<endl;
#endif
    string label1 = newLabel();
    string label2 = newLabel();
    string label3 = newLabel();

    IRInstruction* irInstr = new IRInstruction(IR_LABEL, label1);
    this->irList->push_back(irInstr);  //label1

    translateCond(*(whileStmt.condition), label2, label3);//code1

    irInstr = new IRInstruction(IR_LABEL, label2);
    this->irList->push_back(irInstr);  //label2

    whileStmt.body->accept(*this);     // code2

    irInstr = new IRInstruction(IR_GOTO, label1);
    this->irList->push_back(irInstr);  //label1

    irInstr = new IRInstruction(IR_LABEL, label3);
    this->irList->push_back(irInstr);  //label3

    return false;
}

