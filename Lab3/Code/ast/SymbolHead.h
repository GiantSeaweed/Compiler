//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_SYMBOLHEAD_H
#define AST_SYMBOLHEAD_H
#include <string>

using namespace std;

struct TypeSystem;

struct SymbolHead {
    virtual string getId() = 0;

    virtual int getFirstLine() const = 0;

    virtual void setFirstLine(int firstLine) = 0;

    virtual int getEndLine() const = 0;

    virtual void setEndLine(int endLine) = 0;

    virtual TypeSystem *getTypeSystem() const = 0;

    virtual void setTypeSystem(TypeSystem *typeElement) = 0;

};

#endif //AST_SYMBOLHEAD_H
