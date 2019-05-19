//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef AST_SPECIFIER_H
#define AST_SPECIFIER_H

#include "ast.h"
// using namespace std;

enum BasicType{
    BASICTYPE_INT, BASICTYPE_FLOAT
};

struct Specifier : ASTNode{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct BasicSpecifier : Specifier{
    BasicType basicType;

    BasicSpecifier(BasicType basicType) : basicType(basicType) {}

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};



struct StructSpecifier : Specifier{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct NormalStructSpecifier : StructSpecifier{
    IDExp* tag;

    NormalStructSpecifier(IDExp *tag) : tag(tag) {}

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)tag)->accept(visitor);
        }
    }
};

struct DefStructSpecifier : StructSpecifier{
    IDExp* optTag;
    vector<Definition*> *defList;

    DefStructSpecifier(IDExp *optTag, vector<Definition *> *defList) : optTag(optTag), defList(defList){}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            if (optTag != nullptr) {
                ((ASTNode *) optTag)->accept(visitor);
            }
            for (Definition *d: *defList) {
                ((ASTNode *) d)->accept(visitor);
            }
        }
    }
};

#endif //AST_SPECIFIER_H

