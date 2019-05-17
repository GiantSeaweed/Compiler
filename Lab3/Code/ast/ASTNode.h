//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef AST_ASTNODE_H
#define AST_ASTNODE_H
#include "visitor/Visitor.h"
#include "TypeSystem.h"

struct ASTNode{
    int beginLine;
    int endLine;
    TypeSystem *typeSystem;
    virtual void accept(Visitor &visitor) = 0;
    virtual ~ASTNode(){};

};
#endif //AST_ASTNODE_H
