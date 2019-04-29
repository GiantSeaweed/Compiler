//
// Created by 冯诗伟 on 2019-04-28.
//

#ifndef AST_TRANSFORM_H
#define AST_TRANSFORM_H

#include "ast.h"
#include "type.h"
#include "../tree.h"
#include <vector>
#include <cstring>
#include <iostream>
#include "../syntax.tab.h"
// #include "visitor/PrintVisitor.h"
// #include "visitor/TypeVisitor.h"
// #include "visitor/ExpressionVisitor.h"
// #include "visitor/FunctionReturnVisitor.h"

using namespace std;




// Highlevel definition
Program *transProgram(MultiNode *node);

vector<ExtDef *> *transExtDefList(MultiNode *node);

ExtDef *transExtDef(MultiNode *extDef);

vector<VarDec*> *transExtDecList(MultiNode *node);

//Specifier
Specifier *transSpecifier(MultiNode *node);

BasicSpecifier *transBasicSpecifier(MultiNode *node);

StructSpecifier *transStructSpecifier(MultiNode *node);

IDExp *transOptionalTag(MultiNode *node);

IDExp *transTag(MultiNode *node);

//Declarator
VarDec *transVarDec(MultiNode *node);

FunDec *transFunDec(MultiNode *node);

vector<ParamDec *> *transVarList(MultiNode *node);

ParamDec *transParamDec(MultiNode *node);

//Statements
CompSt *transCompSt(MultiNode *node);

vector<Stmt *> *transStmtList(MultiNode *node);

Stmt *transStmt(MultiNode *node);

ExpStmt *transExpStmt(MultiNode *node);

ReturnStmt *transRetStmt(MultiNode *node);

IfElseStmt *transIfElseStmt(MultiNode *node);

WhileStmt *transWhileStmt(MultiNode *node);

//local definition
vector<Definition *> *transDefList(MultiNode *node);

Definition *transDef(MultiNode *node);

vector<Dec *> *transDecList(MultiNode *node);

Dec *transDec(MultiNode *node);


//Expression
Exp *transExp(MultiNode *node);

InfixExp *transInfixExp(MultiNode *node);

ParenthesizedExp *transParenthesizedExp(MultiNode *node);

PrefixExp *transPrefixExp(MultiNode *node);

FunExp *transFunExp(MultiNode *node);

ArrayExp *transArrayExp(MultiNode *node);

StructExp *transStructExp(MultiNode *node);

IntExp *transIntValue(MultiNode *node);

FloatExp *transFloatValue(MultiNode *node);

IDExp *transIdExp(MultiNode *node);

vector<Exp *> *transArgs(MultiNode *node);



#endif //AST_TRANSFORM_H
