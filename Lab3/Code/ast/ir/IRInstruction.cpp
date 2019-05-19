//
// Created by 冯诗伟 on 2019-05-19.
//
#include "IRInstruction.h"
using namespace std;

string IRInstruction::toString() {
    switch (this->op) {
        case IR_LABEL:
            break;
        case IR_FUN:
            break;
        case IR_GOTO:
            break;
        case IR_IF:
            break;
        case IR_RETURN:
            break;
        case IR_DEC:
            break;
        case IR_ARG:
            break;
        case IR_CALL:
            break;
        case IR_PARAM:
            break;
        case IR_READ:
            break;
        case IR_WRITE:
            break;
        case IR_ASSIGN_SINGLE:
            break;
        case IR_ASSIGN_PLUS:
            break;
        case IR_ASSIGN_MINUS:
            break;
        case IR_ASSIGN_MUL:
            break;
        case IR_ASSIGN_DIV:
            break;
        case IR_ASSIGN_GETADDR:
            break;
        case IR_ASSIGN_LOAD:
            break;
        case IR_ASSIGN_STORE:
            break;

        default:
            cerr << "Invalid IR operator" << endl;
            break;
    }
}