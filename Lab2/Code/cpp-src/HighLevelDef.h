//
// Created by 冯诗伟 on 2019-04-27.
//

#ifndef CPP_SRC_HIGHLEVELDEF_H
#define CPP_SRC_HIGHLEVELDEF_H

#include<vector>
#include "ASTNode.h"
using namespace std;

struct Program : ASTNode{


};


struct ExtDef : ASTNode {
    void accept(Visitor &visitor) override {
        visitor.visit(*this);
    }
}

struct StructDef: ExtDef{

};

struct FuncDef : ExtDef{


};


struct

#endif //CPP_SRC_HIGHLEVELDEF_H
