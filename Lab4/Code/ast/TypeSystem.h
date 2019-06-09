//
// Created by 冯诗伟 on 2019-04-29.
//

#ifndef AST_TYPESYSTEM_H
#define AST_TYPESYSTEM_H
#include "SymbolHead.h"
#include <ostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

struct Symbol;

auto symbolIOp = [](SymbolHead *s1, SymbolHead *s2) -> bool {
    return s1->getId() < s2->getId();
};

enum BaseType {
    BASE_INT, BASE_FLOAT, BASE_STRUCT,
    BASE_FUNCTION, BASE_ARRAY, BASE_DEFAULT
};

extern string BaseTypeString[];

struct TypeSystem {
    BaseType type;
    int size;

    explicit TypeSystem(BaseType type);

    virtual int getBufLength(){
        return 4;
    }

    virtual string toString();

    virtual bool operator!=(const TypeSystem &typeSystem) const {
        return !operator==(typeSystem);
    }

    virtual bool operator==(const TypeSystem &typeSystem) const {
        return this->type == typeSystem.type;
    }

};

struct ArrayType : TypeSystem {
    TypeSystem *base;

    ///the width of this dimension, e.g. width(int[2]) = 2
    int width;

    ArrayType(TypeSystem *base, int width);

    int getBufLength() override {
        return base->getBufLength() * width;
    }

    string toString() override;

    bool operator!=(const TypeSystem &typeSystem) const override;

    bool operator==(const TypeSystem &typeSystem) const override;

};

struct StructType : TypeSystem {
    string structName;

    vector<TypeSystem *> *fields;

    set<SymbolHead *, decltype(symbolIOp)> *symbolTable;

    map<string, int> symbolOffset;

    int getBufLength() override{
        int length = 0;
        for(SymbolHead* symbol: *symbolTable){
            length += symbol->getTypeSystem()->getBufLength();
        }
        return length;
    }

    int getOffset(string id);

    StructType(BaseType type, const vector<TypeSystem *> &fields, string structName);

    StructType(const StructType &structType);

    string toString() override;


    StructType &operator=(const StructType &structType);

    bool operator!=(const TypeSystem &typeSystem) const override;

    bool operator==(const TypeSystem &typeSystem) const override;

    ~StructType();
};

struct FunctionType : TypeSystem {
    TypeSystem *retType;
    vector<TypeSystem *> *args;

    FunctionType(BaseType type, TypeSystem *retType, const vector<TypeSystem *> &args);

    FunctionType(const FunctionType &functionType);

    string toString() override;


    FunctionType &operator=(const FunctionType &functionType);

    bool operator!=(const TypeSystem &typeSystem) const override;

    bool operator==(const TypeSystem &typeSystem) const override;


    ~FunctionType();

};

#endif //AST_TYPESYSTEM_H