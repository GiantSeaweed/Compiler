//
// Created by 冯诗伟 on 2019-05-19.
//
#include "IRInstruction.h"
using namespace std;

string IRInstruction::toString() {
    string str;
    switch (this->op) {
        // src1
        case IR_LABEL:
            str = "LABEL " + src1 + " :";
            break;
        case IR_FUN:
            str = "FUNCTION " + src1 + " :";
            break;
        case IR_GOTO:
            str = "GOTO " + src1;
            break;
        case IR_RETURN:
            str = "RETURN " + src1;
            break;
        case IR_ARG:
            str = "ARG " + src1;
            break;
        case IR_PARAM:
            str = "PARAM " + src1;
            break;
        case IR_READ:
            str = "READ " + src1;
            break;
        case IR_WRITE:
            str = "WRITE " + src1;
            break;
        //src1 dest
        case IR_DEC:
            str = "DEC " + dest + " " + src1;
            break;
        case IR_CALL:
            str = dest + " := CALL " + src1;
            break;
        case IR_ASSIGN_GETADDR:
            str = dest + " := &" + src1;
            break;
        case IR_ASSIGN_LOAD:
            str = dest + " := *" + src1;
            break;
        case IR_ASSIGN_STORE:
            str = "*" + dest + " := " + src1;
            break;
        case IR_ASSIGN_SINGLE:
            str = dest + " := " + src1;
            break;
        //src1 src2 dest
        case IR_ASSIGN_PLUS:
            str = dest + " := " + src1 + " + " + src2;
            break;
        case IR_ASSIGN_MINUS:
            str = dest + " := " + src1 + " - " + src2;
            break;
        case IR_ASSIGN_MUL:
            str = dest + " := " + src1 + " * " + src2;
            break;
        case IR_ASSIGN_DIV:
            str = dest + " := " + src1 + " / " + src2;
            break;

        case IR_IF_GE:
            str = "IF " + src1 + " >= " + src2 + " GOTO " + dest;
            break;
        case IR_IF_GT:
            str = "IF " + src1 + " > " + src2 + " GOTO " + dest;
            break;
        case IR_IF_LE:
            str = "IF " + src1 + " <= " + src2 + " GOTO " + dest;
            break;
        case IR_IF_LT:
            str = "IF " + src1 + " < " + src2 + " GOTO " + dest;
            break;
        case IR_IF_NE:
            str = "IF " + src1 + " != " + src2 + " GOTO " + dest;
            break;
        case IR_IF_EQ:
            str = "IF " + src1 + " == " + src2 + " GOTO " + dest;
            break;

        default:
            cerr << "Invalid IR operator" << endl;
            break;
    }
    return str;
}


