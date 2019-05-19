//
// Created by fsw on 2019-04-28.
//

#include "Visitor.h"
#include "../utils/utils.h"
#include "../Expression.h"
#include "ExpressionVisitor.h"


bool ExpressionVisitor::inSymTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->symTable->find(&tmp);
    return sym != this->symTable->end();
}

bool ExpressionVisitor::inFunTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->funTable->find(&tmp);
    return sym != this->funTable->end();
}

Symbol *ExpressionVisitor::findInSymTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->symTable->find(&tmp);
    return sym != this->symTable->end() ? *sym : nullptr;
}

Symbol *ExpressionVisitor::findInFunTable(const string &id) {
    Symbol tmp(id);
    auto sym = this->funTable->find(&tmp);
    return sym != this->funTable->end() ? *sym : nullptr;
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
    bool lTypeCheck = lType->type != BASE_INT && lType->type != BASE_FLOAT && lType->type != BASE_DEFAULT;
    bool rTypeCheck = rType->type != BASE_INT && rType->type != BASE_FLOAT && lType->type != BASE_DEFAULT;
#ifdef DEBUG
    cout << infixExpression.leftSide->typeSystem->toString()<<" at line" << infixExpression.beginLine << endl;
    cout << infixExpression.rightSide->typeSystem->toString()<<endl;
    cout << infixExpression.infixOp << ","<<infixExpression.leftSide->lval <<endl;
#endif
    if ((lTypeCheck || rTypeCheck) && infixExpression.infixOp != INFIX_ASSIGN) {
#ifdef SEMANTIC
        //operation type error
        printError(7, "Operation between unsupported types", infixExpression.beginLine);
#endif
    } else if (infixExpression.infixOp == INFIX_ASSIGN &&
               !infixExpression.leftSide->lval  ) {
#ifdef SEMANTIC
        //lval Error
        printError(6, "Assign to a right-value!", infixExpression.beginLine);
#endif
    } else if (*lType != *rType) {
#ifdef SEMANTIC
        if (infixExpression.infixOp == INFIX_ASSIGN) {
            //assignment type Error
            printError(5, "Assign between different types!", infixExpression.beginLine);
        } else {
            //type Error
            printError(7, "Operation between different types!", infixExpression.beginLine);
        }
#endif

    } else if (infixExpression.infixOp == INFIX_AND || infixExpression.infixOp == INFIX_OR) {
        if (!(lType->type == BASE_INT && rType->type == BASE_INT)) {
#ifdef SEMANTIC
            //logic Error
            printError(7, "Use logical operation between non-integers!", infixExpression.beginLine);
#endif
        } else {
            infixExpression.typeSystem = infixExpression.leftSide->typeSystem;
        }
    } else {
        delete defaultType;
        if ( infixExpression.infixOp == INFIX_RELOP){
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

#ifdef SEMANTIC
    if (prefixExpression.prefixOp == PREFIX_NOT) {
        if (prefixExpression.exp->typeSystem->type != BASE_INT) {
            //logic Error
            printError(7, "Use logical operation to non-integers!", prefixExpression.beginLine);
        }
    }
#endif
    return false;
}

bool ExpressionVisitor::visit(IDExp &expression) {
#ifdef DEBUG
    cout << "visiting ExpVisitor::IDExp "<<expression.id <<endl;
#endif
    bool isInSymTable = inSymTable(expression.id);
    bool isInFunTable = inFunTable(expression.id);
    if (!isInSymTable && !isInFunTable) {
#ifdef SEMANTIC
        //undefined Error
        if (expression.typeSystem == nullptr || expression.typeSystem->type != BASE_FunTION) {
            printError(1, "id " + expression.id + " is not defined", expression.beginLine);
        }
#endif
        expression.lval = true;
        expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    } else {
        auto sym = isInSymTable ? findInSymTable(expression.id) : findInFunTable(expression.id);
        if (sym->firstLine <= expression.beginLine
            && (sym->endLine >= expression.endLine ||
             sym->endLine == GLOBAL ||
                sym->typeSystem->type == BASE_FunTION || sym->typeSystem->type == BASE_STRUCT)) {
            expression.typeSystem = sym->typeSystem;
            expression.lval = false;
            if (expression.typeSystem->type != BASE_FunTION) {
                expression.lval = true;
            }
        } else {
#ifdef SEMANTIC
            //undefined Error
            printError(1, "id " + expression.id + " not defined", expression.beginLine);
#endif
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
    expression.funID->typeSystem = new TypeSystem(BASE_FunTION);
    expression.funID->accept(*this);

    auto funcSym = findInFunTable(expression.funID->id);
    expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    if (expression.funID->typeSystem->type == BASE_DEFAULT) {
#ifdef SEMANTIC
        //undefined method Error
        printError(2, "FunTion " + expression.funID->id + " undefined!", expression.beginLine);
#endif
    } else if (funcSym == nullptr) {
#ifdef SEMANTIC
        //invocation Error
        printError(11, "Name " + expression.funID->id + " is not a funTion!", expression.beginLine);
#endif
    } else {
        auto methodType = (FunTionType *) funcSym->typeSystem;
        if (expression.args->size() != methodType->args->size()) {
#ifdef SEMANTIC
            //param and args Error
            printError(9, "Name " + expression.funID->id + " has unmatched arguments!", expression.beginLine);
#endif
        } else {
            for (int i = 0; i < methodType->args->size(); i++) {
                (*expression.args)[i]->accept(*this);
                auto argType = (*expression.args)[i]->typeSystem;
                auto paramType = (*methodType->args)[i];
                if (*argType != *paramType) {
#ifdef SEMANTIC
                    //params and args Error
                    printError(9, "Argument " + to_string(i) + " has unmatched argument type!", expression.beginLine);
#endif
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
#ifdef SEMANTIC
        //arrayAccess Error
        printError(10, "Try to access non-array, or access have wrong dimension", expression.beginLine);
#endif
        expression.typeSystem = new TypeSystem(BASE_DEFAULT);
    } else {
        auto arrayType = ((ArrayType &) *expression.arrayExp->typeSystem);
        expression.typeSystem = arrayType.base;
    }
    expression.arrayIndex->accept(*this);
#ifdef SEMANTIC
    if (expression.arrayIndex->typeSystem->type != BASE_INT) {
        //float arrayAccess Error
        printError(12, "Access an array with wrong typed expression", expression.beginLine);
    }
#endif
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
#ifdef SEMANTIC
        //field access type error
        printError(13, "Access a non-structural type", expression.beginLine);
#endif
    } else {
        auto &structType = ((StructType &) *(expression.structExp->typeSystem));
        auto fieldTable = structType.symbolTable;
        Symbol tmp(expression.structID->id);
        auto field = fieldTable->find(&tmp);
        if (field == fieldTable->end()) {
#ifdef SEMANTIC
            //non exist field
            printError(14, "Access an undefined field " + expression.structID->id, expression.beginLine);
#endif
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


