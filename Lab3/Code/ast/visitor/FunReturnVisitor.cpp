//
// Created by 冯诗伟 on 2019-05-01.
//

#include "../type.h"
#include "FunReturnVisitor.h"
#include "../ASTNode.h"
#include "../utils/utils.h"
#include "../Statement.h"


bool FunReturnVisitor::visit(ReturnStmt &stmt) {
#ifdef DEBUG
    cout << "visiting FunReturnVisitor::ReturnStmt" << endl;
#endif
    if (inFunction) {
        // cout << this->retType->type << " " << stmt.exp->typeSystem->type <<endl;
        if (*this->retType != *stmt.exp->typeSystem)
        {
            printError(8, "Return type cannot match function return type!", stmt.beginLine);
        }
    }
    return false;
}

bool FunReturnVisitor::visit(FunDef &hlDef) {
#ifdef DEBUG
    cout << "visiting FunReturnVisitor::FunDef" << endl;
#endif
    bool oriInFunction = this->inFunction;
    this->inFunction = true;
    auto oriRetElement = this->retType;
    this->retType = hlDef.retSpecifier->typeSystem;
    hlDef.compSt->accept(*this);
    this->retType = oriRetElement;
    this->inFunction = oriInFunction;
    return false;
}
