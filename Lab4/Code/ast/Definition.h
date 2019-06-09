//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef AST_DEFINITION_H
#define AST_DEFINITION_H

#include "ast.h"

// using namespace std;

struct Dec : ASTNode{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

    virtual VarDec *getVarDec() = 0;
};

struct NormalDec: Dec{
    VarDec* varDec;

    NormalDec(VarDec *varDec) : varDec(varDec) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) varDec)->accept(visitor);
        }
    }

    VarDec* getVarDec() override{
        return varDec;
    }
};

struct InitializedDec : Dec{
    VarDec *varDec;
    Exp* initialValue;

    InitializedDec(VarDec *varDec, Exp *initialValue) : varDec(varDec), initialValue(initialValue) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) varDec)->accept(visitor);
            ((ASTNode *)initialValue)->accept(visitor);
        }

    }

    VarDec *getVarDec() override {
        return varDec;
    }
};


struct Definition : ASTNode{
    Specifier *specifier;
    vector<Dec*> *decList;

    Definition(Specifier *specifier, vector<Dec *> *decList) : specifier(specifier), decList(decList) {}

    ~Definition() {
        delete this->decList;
    }

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*) specifier)->accept(visitor);
            for(Dec* dec : *decList){
                ((ASTNode*) dec)->accept(visitor);
            }
        }

    }
};

#endif //AST_DEFINITION_H
