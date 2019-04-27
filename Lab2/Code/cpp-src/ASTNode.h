//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef CPP_SRC_ASTNODE_H
#define CPP_SRC_ASTNODE_H


struct ASTNode{
    int beginLine;
    int endLine;

    virtual void accept(Visitor &visitor) = 0;
    virtual ~ASTNode(){};

};
#endif //CPP_SRC_ASTNODE_H
