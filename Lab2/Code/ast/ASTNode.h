//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef AST_ASTNODE_H
#define AST_ASTNODE_H
#include "Visitor.h"

struct ASTNode{
    int beginLine;
    int endLine;

    virtual void accept(Visitor &visitor) = 0;
    // virtual ~ASTNode(){};

};
#endif //AST_ASTNODE_H
