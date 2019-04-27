cmake_minimum_required(VERSION 3.13)
project(cpp_src)

set(CMAKE_CXX_STANDARD 11)

add_executable(cpp_src main.cpp
                HighLevelDef.h
                Definition.h
                Specifier.h
                Declarator.h
                Statement.h
                Expression.h
                ASTNode.h)