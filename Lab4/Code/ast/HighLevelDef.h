//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef AST_HIGHLEVELDEF_H
#define AST_HIGHLEVELDEF_H


#include "ast.h"

struct ExtDef : ASTNode{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct DecDef : ExtDef {
    Specifier* specifier;
    vector<VarDec*> *extDecList;

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            if(visitor.visit(*this)){
                ((ASTNode*) specifier)->accept(visitor);
                for(VarDec* varDec: *extDecList){
                    ((ASTNode*)varDec)->accept(visitor);
                }
            }
        }
    }

    DecDef(Specifier *specifier, vector<VarDec *> *extDecList) : specifier(specifier), extDecList(extDecList) {}

    virtual ~DecDef() {
        delete this->extDecList;
    }

};

struct TypeDef: ExtDef{
    Specifier* specifier;

    TypeDef(Specifier *specifier) : specifier(specifier) {}

    void accept(Visitor& visitor){
        if(visitor.visit(*this)){
            ((ASTNode*)specifier)->accept(visitor);
        }
    }
};

struct FunDef : ExtDef{
    Specifier* retSpecifier;
    FunDec* funDec;
    CompSt * compSt;

    FunDef(Specifier *retSpecifier, FunDec *funDec, CompSt *compSt) : retSpecifier(retSpecifier), funDec(funDec),
                                                                         compSt(compSt) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) retSpecifier)->accept(visitor);
            ((ASTNode *) funDec)->accept(visitor);
            ((ASTNode *) compSt)->accept(visitor);
        }
    }

};

struct Program : ASTNode{
    vector<ExtDef*> *extDefList;

    Program(vector<ExtDef *> *extDefList) : extDefList(extDefList) {}

    void accept(Visitor &visitor) override {
#ifdef DEBUG
        cout<<""
#endif
        if (visitor.visit(*this)) {
            for (ExtDef *def:*extDefList) {
                ((ASTNode *) def)->accept(visitor);
            }
        }
    }

    virtual ~Program() {
        delete this->extDefList;
    }

};

#endif //AST_HIGHLEVELDEF_H
