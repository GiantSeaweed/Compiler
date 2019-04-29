//
// Created by 冯诗伟 on 2019-04-27.
//



#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include "ast.h"

// using namespace std;

enum InfixOperator{
    INFIX_ASSIGN = 0,
    INFIX_AND, INFIX_OR,
    INFIX_RELOP,
    INFIX_PLUS, INFIX_MINUS, INFIX_STAR, INFIX_DIV
};//INFIX_GT,  INFIX_LT, INFIX_GE, INFIX_LE, INFIX_EQ, INFIX_NE,

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

    InfixExp(Exp *leftSide, InfixOperator infixOp, Exp *rightSide) : leftSide(leftSide),
                                                                      infixOp(infixOp), rightSide(rightSide) {}

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
            ((ASTNode*) arrayExp)->accept(visitor);
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

    StructExp(Exp *structExp, IDExp *structId) : structExp(structExp), structID(structId) {}

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
        visitor.visit(*this);
    }
};

struct FloatExp : Exp{
    string value;

    FloatExp(string value) : value(value) {}

    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

struct FunExp: Exp{
    IDExp* funID;
    vector<IDExp*> *args;

    FunExp(IDExp *funId) : funID(funId) {}

    FunExp(IDExp *funId, vector<IDExp *> *args) : funID(funId), args(args) {}

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)funID)->accept(visitor);
            for(Exp* exp: *args){
                ((ASTNode*) exp)->accept(visitor);
            }
        }
    }

    ~FunExp() {
        delete this->args;
    }
};

#endif //AST_EXPRESSION_H
