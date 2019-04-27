//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef CPP_SRC_DECLARATOR_H
#define CPP_SRC_DECLARATOR_H

#include <string>
#include "ASTNode.h"
#include "Expression.h"
#include "Specifier.h"

using namespace std;

struct Declarator : ASTNode{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct VarDec : Declarator{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

    virtual std::string getID() = 0;
};

struct NormalVarDec : VarDec{
    IDExp* id;

    NormalVarDec(IDExp *id) : id(id) {}

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)id)->accept(visitor);
        }
    }

    string getID() override {
        return id->id;
    }

};

struct ArrayDec : VarDec{
    VarDec* varDec;
    int size; //for future use

    ArrayDec(VarDec *varDec, int size) : varDec(varDec), size(size) {}

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)id)->accept(visitor);
        }
    }

    string getID() override {
        return varDec->getID();
    }

};

struct ParamDec : Declarator{
    Specifier* specifier;
    VarDec* varDec;

    ParamDec(Specifier *specifier, VarDec *varDec) : specifier(specifier), varDec(varDec) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) specifier)->accept(visitor);
            ((ASTNode *) varDec)->accept(visitor);
        }
    }

};


struct FuncDec : Declarator{
    IDExp* id;
    vector<ParamDec*> paramList;

    FuncDec(IDExp *id, const vector<ParamDec *> &paramList) : id(id), paramList(paramList) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) id)->accept(visitor);
            for (ParamDec *param : *paramList) {
                ((ASTNode *) param)->accept(visitor);
            }
        }
    }


    ~FuncDec() {
        delete this->paramList;
    }

};


#endif //CPP_SRC_DECLARATOR_H
