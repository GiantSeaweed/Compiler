//
// Created by 冯诗伟 on 2019-05-19.
//

#ifndef AST_IRINSTRUCTION_H
#define AST_IRINSTRUCTION_H

#include "../Symbol.h"
//#include <string>
//using namespace std;

enum IROperator{
    IR_LABEL, IR_FUN,IR_GOTO,
    IR_RETURN, IR_DEC,
    IR_ARG, IR_CALL,IR_PARAM,
    IR_READ, IR_WRITE,
    IR_ASSIGN_SINGLE,
    IR_ASSIGN_PLUS, IR_ASSIGN_MINUS, IR_ASSIGN_MUL, IR_ASSIGN_DIV,
    IR_ASSIGN_GETADDR, // x := &y
    IR_ASSIGN_LOAD,   //  x := *y
    IR_ASSIGN_STORE,   // *x := y
    IR_IF_GE, IR_IF_GT,
    IR_IF_LE, IR_IF_LT,
    IR_IF_NE, IR_IF_EQ
};



struct IRInstruction{
    IROperator op;
    string src1;
    string src2;
    string dest;

    IRInstruction(IROperator op,
                  string src1) : op(op), src1(src1) {}

    IRInstruction(IROperator op,
                  const string &src1, const string &dest) : op(op), src1(src1), dest(dest) {}

    IRInstruction(IROperator op,
                  const string &src1, const string &src2, const string &dest) : op(op), src1(src1), src2(src2), dest(dest) {}

    string toString();

    virtual ~IRInstruction() {}
};





#endif //AST_IRINSTRUCTION_H
