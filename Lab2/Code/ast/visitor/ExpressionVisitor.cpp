//
// Created by fsw on 2019-04-28.
//

#include "Visitor.h"
#include "../utils/utils.h"
#include "../Expression.h"
#include "ExpressionVisitor.h"

//ExpressionVisitor::ExpressionVisitor(set<Symbol *, decltype(op)> *symTable) : symTable(symTable) {}

//extern void printError(int errNum, const string &message, int location);

bool ExpressionVisitor::inSymTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->symTable->find(&tmp);
    return sym != this->symTable->end();
}

bool ExpressionVisitor::inFuncTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->funcTable->find(&tmp);
    return sym != this->funcTable->end();
}

Symbol *ExpressionVisitor::findInSymTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->symTable->find(&tmp);
    return sym != this->symTable->end() ? *sym : nullptr;
}

Symbol *ExpressionVisitor::findInFuncTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->funcTable->find(&tmp);
    return sym != this->funcTable->end() ? *sym : nullptr;
}


bool ExpressionVisitor::visit(InfixExp &infixExpression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::InfixExp" <<endl;
#endif
    infixExpression.leftSide->accept(*this);
    infixExpression.rightSide->accept(*this);
    TypeSystem *lType = infixExpression.leftSide->typeSystem;
    TypeSystem *rType = infixExpression.rightSide->typeSystem;
    auto defaultType = new TypeSystem(BASE_DEFAULT);
    infixExpression.typeSystem = defaultType;
    bool lTypeCheck = lType->type != BASE_INT && lType->type != BASE_FLOAT;
    bool rTypeCheck = rType->type != BASE_INT && rType->type != BASE_FLOAT;
    if ((lTypeCheck || rTypeCheck) && infixExpression.infixOp != INFIX_ASSIGN) {
        //operation type error
        printError(7, "Operation between unsupported types", infixExpression.beginLine);

    } else if (infixExpression.infixOp == INFIX_ASSIGN && !infixExpression.leftSide->lval) {
        //lval Error
        printError(6, "Assign to a rval!", infixExpression.beginLine);
    } else if (*lType != *rType) {
        if (infixExpression.infixOp == INFIX_ASSIGN) {
            //assignment type Error
            printError(5, "Assign between different Types!", infixExpression.beginLine);
        } else {
            //type Error
            printError(7, "Operation between different Types!", infixExpression.beginLine);
        }

    } else if (infixExpression.infixOp == INFIX_AND || infixExpression.infixOp == INFIX_OR) {
        if (!(lType->type == BASE_INT && rType->type == BASE_INT)) {
            //logic Error
            printError(7, "Use logical operation between non-integers!", infixExpression.beginLine);
        } else {
            infixExpression.typeSystem = infixExpression.leftSide->typeSystem;
        }
    } else {
        delete defaultType;
        if ( infixExpression.infixOp == INFIX_RELOP){
//                infixExpression.infixOp == INFIX_NE
//            || infixExpression.infixOp == INFIX_EQ
//            || infixExpression.infixOp == INFIX_LT
//            || infixExpression.infixOp == INFIX_LE
//            || infixExpression.infixOp == INFIX_GT
//            || infixExpression.infixOp == INFIX_GE) {
            infixExpression.typeSystem = new TypeSystem(BASE_INT);
        } else {
            infixExpression.typeSystem = infixExpression.leftSide->typeSystem;
        }
    }
    infixExpression.lval = false;
    return false;
}

bool ExpressionVisitor::visit(ParenthesizedExp &parenthesizedExpression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::ParenthesizedExp" <<endl;
#endif
    parenthesizedExpression.exp->accept(*this);
    parenthesizedExpression.typeSystem = parenthesizedExpression.exp->typeSystem;
    parenthesizedExpression.lval = parenthesizedExpression.exp->lval;
    return VisitorTrue::visit(parenthesizedExpression);
}

bool ExpressionVisitor::visit(PrefixExp &prefixExpression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::PrefixExp" <<endl;
#endif
    prefixExpression.exp->accept(*this);
    prefixExpression.lval = false;
    prefixExpression.typeSystem = prefixExpression.exp->typeSystem;
    if (prefixExpression.prefixOp == PREFIX_NOT) {
        if (prefixExpression.exp->typeSystem->type != BASE_INT) {
            //logic Error
            printError(7, "Use logical operation to non-integer!", prefixExpression.beginLine);
        }
    }
    return false;
}

bool ExpressionVisitor::visit(IDExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::IDExp" <<endl;
#endif
    // if (expression.id.length() == 0)
        // return false;

    bool isInSymTable = inSymTable(expression.id);
    bool isInFuncTable = inFuncTable(expression.id);
    if (!isInSymTable && !isInFuncTable) {
        //undefined Error
        if (expression.typeSystem == nullptr || expression.typeSystem->type != BASE_FUNCTION) {
            // cout<<"FSW"<< expression.id <<", length="<<expression.id.length()<<  endl;
            printError(1, "id " + expression.id + " not defined", expression.beginLine);
        }
        expression.lval = true;
        expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    } else {
        auto sym = isInSymTable ? findInSymTable(expression.id) : findInFuncTable(expression.id);
        // cout <<"***"<<expression.id<<"***"<<endl;
        // cout << "SymFirst:"<<sym->firstLine<<",SymEnd:"<<sym->endLine<<endl;
        // cout <<"ExpFirst:" << expression.beginLine << ",ExpEnd:"<<expression.endLine<<endl;
        if (sym->firstLine <= expression.beginLine
            && (sym->endLine >= expression.endLine ||
             sym->endLine == GLOBAL ||
                sym->typeSystem->type == BASE_FUNCTION || sym->typeSystem->type == BASE_STRUCT)) {
            expression.typeSystem = sym->typeSystem;
            expression.lval = false;
            if (expression.typeSystem->type != BASE_FUNCTION) {
                expression.lval = true;
            }
        } else {
            //undefined Error
            printError(1, "id " + expression.id + " not defined", expression.beginLine);
            expression.lval = true;
            expression.typeSystem = new TypeSystem(BASE_DEFAULT);
        }
    }


    return false;
}

bool ExpressionVisitor::visit(IntExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::IntExp" <<endl;
#endif
    expression.typeSystem = new TypeSystem(BASE_INT);
    expression.lval = false;
    return false;
}

bool ExpressionVisitor::visit(FloatExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::FloatExp" <<endl;
#endif
    expression.typeSystem = new TypeSystem(BASE_FLOAT);
    expression.lval = false;
    return false;
}

bool ExpressionVisitor::visit(FunExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::FunExp" <<endl;
#endif
    expression.funID->typeSystem = new TypeSystem(BASE_FUNCTION);
    expression.funID->accept(*this);
    auto funcSym = findInFuncTable(expression.funID->id);
    expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    if (expression.funID->typeSystem->type == BASE_DEFAULT) {
        //undefined method Error
        printError(2, "Function " + expression.funID->id + " undefined!", expression.beginLine);
    } else if (funcSym == nullptr) {
        //invocation Error
        printError(11, "Name " + expression.funID->id + " is not a function!", expression.beginLine);
    } else {
        auto methodType = (FunctionType *) funcSym->typeSystem;
        if (expression.args->size() != methodType->args->size()) {
            //param and args Error
            printError(9, "Name " + expression.funID->id + " has unmatched arguments!", expression.beginLine);
        } else {
            for (int i = 0; i < methodType->args->size(); i++) {
                (*expression.args)[i]->accept(*this);
                auto argType = (*expression.args)[i]->typeSystem;
                auto paramType = (*methodType->args)[i];
                if (*argType != *paramType) {
                    //params and args Error
                    printError(9, "Argument " + to_string(i) + " has unmatched argument type!", expression.beginLine);
                    break;
                }
            }
        }
        delete expression.typeSystem;
        expression.typeSystem = methodType->retType;
    }

    expression.lval = false;
    return false;
}

bool ExpressionVisitor::visit(ArrayExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::ArrayExp" <<endl;
#endif
    expression.arrayExp->accept(*this);
    if (expression.arrayExp->typeSystem->type != BASE_ARRAY) {
        //arrayAccess Error
        printError(10, "Try to access non-array, or access have wrong dimension", expression.beginLine);
        expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    } else {
        auto arrayType = ((ArrayType &) *expression.arrayExp->typeSystem);
        expression.typeSystem = arrayType.base;
    }
    expression.arrayIndex->accept(*this);
    if (expression.arrayIndex->typeSystem->type != BASE_INT) {
        //float arrayAccess Error
        printError(12, "Access an array with wrong typed expression", expression.beginLine);
    }

    expression.lval = true;
    return false;
}

bool ExpressionVisitor::visit(StructExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::StructExp" <<endl;
#endif
    expression.structExp->accept(*this);
    expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    if (expression.structExp->typeSystem->type != BASE_STRUCT) {
        //field access type error
        printError(13, "Access a non-structural type", expression.beginLine);

    } else {
        auto &structType = ((StructType &) *(expression.structExp->typeSystem));
        auto fieldTable = structType.symbolTable;
        Symbol tmp(expression.structID->id);
        auto field = fieldTable->find(&tmp);
        if (field == fieldTable->end()) {
            //non exist field
            printError(14, "Access an undefined field " + expression.structID->id, expression.beginLine);

        } else {
            auto toAccess = *field;
            delete expression.typeSystem;
            expression.typeSystem = toAccess->getTypeSystem();
            expression.lval = true;
        }
    }
    return false;
}

bool ExpressionVisitor::visit(DefStructSpecifier &specifier) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::DefStructSpecifier" <<endl;
#endif
    return false;
}


