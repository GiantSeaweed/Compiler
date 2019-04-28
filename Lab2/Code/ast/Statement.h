//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef CPP_SRC_STATEMENT_H
#define CPP_SRC_STATEMENT_H

#include "ASTNode.h"
#include "Expression.h"
#include "Definition.h"
#include "Visitor.h"
#include <vector>

using namespace std;

struct Stmt : ASTNode{
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
};

struct CompSt : Stmt{
    vector<Definition*> *defList;
    vector<Stmt*> *stmtList;

    CompSt(vector<Definition *> *defList, vector<Stmt *> *stmtList) : defList(defList), stmtList(stmtList) {}

    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            for (Definition *def : *defList) {
                ((ASTNode *) def)->accept(visitor);
            }

            for (Stmt *stmt : *stmtList) {
                ((ASTNode *) stmt)->accept(visitor);
            }
        }
    }

    virtual ~CompSt() {
        delete this->defList;
        delete this->stmtList;

    }
};

struct ExpStmt : Stmt{
    Exp* exp;
    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) exp)->accept(visitor);
        }
    }

    ExpStmt(Exp *exp) : exp(exp) {}
};

struct ReturnStmt : Stmt{
    Exp* exp;
    void accept(Visitor &visitor) override {
        if (visitor.visit(*this)) {
            ((ASTNode *) exp)->accept(visitor);
        }
    }

    ReturnStmt(Exp *exp) : exp(exp) {}
};


struct IfElseStmt : Stmt {
    Exp* condition;
    Stmt* thenBody;
    Stmt* elseBody;

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*)condition)->accept(visitor);
            ((ASTNode*)thenBody)->accept(visitor);
            ((ASTNode*)elseBody)->accept(visitor);
        }
    }
    IfElseStmt(Exp *condition, Stmt *thenBody, Stmt *elseBody) : condition(condition), thenBody(thenBody),
                                                                 elseBody(elseBody) {}
};

struct WhileStmt : Stmt {
    Exp* condition;
    Stmt* body;

    void accept(Visitor &visitor) override {
        if(visitor.visit(*this)){
            ((ASTNode*) condition)->accept(visitor);
            ((ASTNode*) condition)->accept(visitor);
        }
    }

    WhileStmt(Exp *condition, Stmt *body) : condition(condition), body(body) {}
};

#endif //CPP_SRC_STATEMENT_H
