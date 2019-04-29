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
        Visitor.h Transform.h ast.h type.h VisitorExpression.h Symbol.h SymbolHead.h TypeSystem.h TypeSystem.cpp VisitorType.h VisitorType.cpp)