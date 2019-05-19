//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_SYMBOL_H
#define AST_SYMBOL_H
#include <string>
#include "ASTNode.h"
#include "SymbolHead.h"
#include "TypeSystem.h"

using namespace std;

const int GLOBAL = -1;

struct Symbol : SymbolHead {
    int firstLine;
    int endLine;

    TypeSystem *typeSystem;
    string id;

    Symbol(const string &id) : id(id) {}

    Symbol(ASTNode *node,
            TypeSystem *typeSystem,
            string id) : typeSystem(typeSystem), id(id) {
        this->firstLine = node->beginLine;
//        this->endLine = node->endLine;
    }

    int getFirstLine() const override{
        return firstLine;
    }

    void setFirstLine(int firstLine) {
        this->firstLine = firstLine;
    }

    int getEndLine() const override{
        return endLine;
    }

    void setEndLine(int firstLine) {
        this->endLine = endLine;
    }


    string getId() override{
        return this->id;
    }

    TypeSystem *getTypeSystem() const {
        return typeSystem;
    }

    void setTypeSystem(TypeSystem *typeSystem) {
        this->typeSystem= typeSystem;
    }


};











#endif //AST_SYMBOL_H
