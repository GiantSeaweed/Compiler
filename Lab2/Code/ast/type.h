//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_TYPE_H
#define AST_TYPE_H
// #include "Declarator.h"
struct Declarator;
struct VarDec;
struct NormalVarDec;
struct ArrayDec;
struct ParamDec;
struct FunDec;

// #include "Definition.h"
struct Dec;
struct NormalDec;
struct InitializedDec;
struct Definition;

// #include "Specifier.h"
struct Specifier;
struct BasicSpecifier;
struct StructSpecifier;
struct NormalStructSpecifier;
struct DefStructSpecifier;

// #include "Expression.h"
struct Exp;
struct InfixExp;
struct PrefixExp;
struct ParenthesizedExp;
struct ArrayExp;
struct IDExp;
struct StructExp;
struct IntExp;
struct FloatExp;
struct FunExp;

// #include "HighLevelDef.h"
struct ExtDef;
struct DecDef;
struct TypeDef;
struct FunDef;
struct Program;

// #include "Statement.h"
struct Stmt;
struct CompSt;
struct ExpStmt;
struct ReturnStmt;
struct IfElseStmt;
struct WhileStmt;

struct ASTNode;
struct Visitor;

#endif //AST_TYPE_H
