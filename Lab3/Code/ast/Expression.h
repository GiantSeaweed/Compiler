//
// Created by 冯诗伟 on 2019-04-27.
//



#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include "ASTNode.h"
#include "visitor/Visitor.h"
#include <vector>
#include <string>
#include <cstring>
using namespace std;

enum InfixOperator
{
    INFIX_ASSIGN = 0,
    INFIX_AND,
    INFIX_OR,
    INFIX_PLUS,
    INFIX_MINUS,
    INFIX_STAR, //5
    INFIX_DIV,
    INFIX_GT, //> 7
    INFIX_LT, //< 8
    INFIX_GE, //>= 9
    INFIX_LE, //<= 10
    INFIX_EQ, //== 11
    INFIX_NE, //!= 12
    INFIX_RELOP
};

enum PrefixOperator{
    PREFIX_MINUS = 0, PREFIX_NOT
};

struct Exp : ASTNode{
    bool lval ;
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
    ~Exp() = default;
};

struct IDExp : Exp{
    string id;

    IDExp(const string &id) : id(std::move(id)) {}
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }

};

struct InfixExp : Exp {
    Exp* leftSide;
    Exp* rightSide;
    InfixOperator infixOp;

    InfixExp(Exp *leftSide, InfixOperator infixOp, Exp *rightSide) : leftSide(leftSide),
                                                                      infixOp(infixOp), rightSide(rightSide) {}

    void accept(Visitor &visitor) override {
        // cout << "FSW " << infixOp << endl;
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
    vector<Exp*> *args;

    FunExp(IDExp *funId) : funID(funId) {}

    FunExp(IDExp *funId, vector<Exp *> *args) : funID(funId), args(args) {}

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
