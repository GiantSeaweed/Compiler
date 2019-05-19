cmake_minimum_required(VERSION 3.13)
project(ast)

set(CMAKE_CXX_STANDARD 11)

add_executable(ast Transform.cpp
        Declarator.h
        Definition.h
        Expression.h
        HighLevelDef.h
        Specifier.h
        Statement.h
        visitor/Visitor.h
        Transform.h
        ast.h
        type.h
        Symbol.h
        SymbolHead.h
        TypeSystem.h
        TypeSystem.cpp
        visitor/TypeVisitor.h
        visitor/TypeVisitor.cpp
        visitor/ExpressionVisitor.h
        visitor/ExpressionVisitor.cpp utils/utils.h utils/utils.cpp
        visitor/FunReturnVisitor.h visitor/FunReturnVisitor.cpp
        visitor/PrintVisitor.h visitor/PrintVisitor.cpp
        ir/IRInstruction.h ir/IRBuilderVisitor.h ir/IRBuilderVisitor.cpp ir/IRInstruction.cpp)