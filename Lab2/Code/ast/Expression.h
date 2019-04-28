//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef CPP_SRC_EXPRESSION_H
#define CPP_SRC_EXPRESSION_H

#include "ASTNode.h"
#include "Visitor.h"
#include <string>
#include <vector>

using namespace std;

enum InfixOperator{
    INFIX_ASSIGN = 0,
    INFIX_AND, INFIX_OR,
    INFIX_GT,  INFIX_LT, INFIX_GE, INFIX_LE, INFIX_EQ, INFIX_NE,
    INFIX_PLUS, INFIX_STAR, INFIX_DIV
};

enum PrefixOperator{
    PREFIX_MINUS = 0, PREFIX_NOT
};

struct Exp : ASTNode{

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
    ~Exp() = default;
};

struct InfixExp : Exp {
    Exp* leftSide;
    Exp* rightSide;
    InfixOperator infixOp;

    InfixExp(Exp *leftSide, Exp *rightSide, InfixOperator infixOp) : leftSide(leftSide), rightSide(rightSide),
                                                                     infixOp(infixOp) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *)leftSide)->accept(visitor);
            ((ASTNode *)rightSide)->accept(visitor);
        }
    }

};

struct PrefixExp : Exp {
    Exp* exp;
    PrefixOperator prefixOp;

    PrefixExp(Exp *exp, PrefixOperator prefixOp) : exp(exp), prefixOp(prefixOp) {}
    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) exp)->accept(visitor);
        }
    }
};

struct ParenthesizedExp :Exp {
    Exp* exp;

    ParenthesizedExp(Exp *exp) : exp(exp) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) exp)->accept(visitor);
        }
    }
};

struct ArrayExp : Exp{
    Exp* arrayExp;
    Exp* arrayIndex;

    ArrayExp(Exp *arrayExp, Exp *arrayIndex) : arrayExp(arrayExp), arrayIndex(arrayIndex) {}

    void accept(Visitor &visitor){
        if(visitor.visit(*this)){
            ((ASTNode*) arrayID)->accept(visitor);
            ((ASTNode*) arrayIndex)->accept(visitor);
        }
    }
};

struct IDExp : Exp{
    string id;

    IDExp(const string &id) : id(id) {}
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

struct StructExp : Exp{
    Exp* structExp;
    IDExp* structID;

    StructExp(Exp *structExp, Exp *structId) : structExp(structExp), structID(structId) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) structExp)->accept(visitor);
            ((ASTNode *) structID)->accept(visitor);
        }
    }
};

struct IntExp: Exp{
    int value;

    IntExp(int value) : value(value) {}

    void accept(Visitor & visitor) override {
        visitor.visitor(*this);
    }
};

struct FloatExp : Exp{
    float value;

    FloatExp(float value) : value(value) {}

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

struct FunExp: Exp{
    IDExp* funID;
    vector<IDExp*> *args;

    FunExp(IDExp *funcId) : funID(funcId) {}

    FunExp(IDExp *funcId, vector<IDExp *> *args) : funcID(funcId), args(args) {}

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)funcID)->accept(visitor);
            for(Exp* exp: *args){
                ((ASTNode*) exp)->accept(visitor);
            }
        }
    }

    ~FunExp() {
        delete this->args;
    }
};

#endif //CPP_SRC_EXPRESSION_H
