//
// Created by 冯诗伟 on 2019-04-29.
//

#include "TypeSystem.h"
#include <algorithm>
#include<iostream>

using namespace std;

string BaseTypeString[] = {"BASE_INT", "BASE_FLOAT",
                           "BASE_STRUCT","BASE_FUNCTION",
                           "BASE_ARRAY", "BASE_DEFAULT"};


ArrayType::ArrayType(TypeSystem *base, int width)
        : base(base), TypeSystem(BASE_ARRAY), width(width) {

}


TypeSystem::TypeSystem(BaseType type) : type(type) {
    this->size  = 4;
}

string TypeSystem::toString() {
    return string("BaseType: ") + BaseTypeString[this->type] + string("  size: ") + to_string(this->size);
}


// ArrayType
string ArrayType::toString() {
    return string("ArrayType: ") + this->base->toString() + string("  width: ") + to_string(this->width);
}

bool ArrayType::operator!=(const TypeSystem &typeSystem) const {
    return !operator==(typeSystem);
}

bool ArrayType::operator==(const TypeSystem &typeSystem) const {
    if (typeSystem.type != BASE_ARRAY) {
        return false;
    } else {
        return *((ArrayType &) typeSystem).base == *this->base;
    }
}

//StructType
string StructType::toString() {
    string ret = string("StructType: ") + " members: \n";

    for (SymbolHead *symbol:*this->symbolTable) {
        ret += "{ ";
        ret += symbol->getId();
        ret += " : ";
        ret += symbol->getTypeSystem()->toString();
        ret += ", offset: ";
        ret += to_string(this->getOffset(symbol->getId()));
        ret += " }\n";
    }
    return ret;
}

int StructType::getOffset(string id){
    int offset = 0;
    string symbolID;
    for(SymbolHead* symbol :*symbolTable){
        symbolID = symbol->getId();
//        cout << symbolID << ", "<<id<<endl;
        if(symbolID == id) {
            break;
        }
        offset += symbol->getTypeSystem()->getBufLength();
    }
    return offset;
}

StructType::StructType(BaseType type, const vector<TypeSystem *> &fields, string structName)
        : TypeSystem(type), structName(structName) {
    this->fields = new vector<TypeSystem *>;
    copy(fields.cbegin(), fields.cend(), back_inserter(*this->fields));
    //TODO fake size
    this->size = fields.size();
}

StructType::StructType(const StructType &structType) : TypeSystem(structType) {
    this->type = structType.type;
    if (structType.fields != this->fields) {
        this->fields = new vector<TypeSystem *>;
        copy(structType.fields->cbegin(), structType.fields->cend(), back_inserter(*this->fields));
    }

}

StructType &StructType::operator=(const StructType &structType) {
    this->type = structType.type;
    if (structType.fields != this->fields) {
        delete this->fields;
        this->fields = new vector<TypeSystem *>;
        copy(structType.fields->cbegin(), structType.fields->cend(), back_inserter(*this->fields));
    }
    return *this;
}

StructType::~StructType() {
    delete this->fields;
    delete this->symbolTable;
}

bool StructType::operator!=(const TypeSystem &typeSystem) const {
    return !operator==(typeSystem);
}

bool StructType::operator==(const TypeSystem &typeSystem) const {
    if (typeSystem.type != BASE_STRUCT) {
        return false;
    } else {
        bool fieldNumberEq = ((StructType &) typeSystem).fields->size() == this->fields->size();
        if (!fieldNumberEq) {
            return false;
        }
        for (int i = 0; i < this->fields->size(); i++) {
            TypeSystem *otherField = (*((StructType &) typeSystem).fields)[i];
            TypeSystem *thisField = (*this->fields)[i];
            if (*otherField != *thisField) {
                return false;
            }
        }
        return true;
    }
}


//FunctionType
string FunctionType::toString() {
    string ret = string("\n\tFunctionType: ");
    ret += string("\n\t\tReturn Specifier: ") + this->retType->toString() + " ";
    ret += "\n\t\tParams: \n\t\t";
    for (TypeSystem *params:*this->args) {
        ret += params->toString();
        ret += ", ";
    }
    return ret;
}

FunctionType::FunctionType(BaseType type,
                            TypeSystem *retType,
                            const vector<TypeSystem *> &args) : TypeSystem(type),retType(retType) {
    this->args = new vector<TypeSystem *>;
    copy(args.cbegin(), args.cend(), back_inserter(*this->args));

}

FunctionType::FunctionType(const FunctionType &functionType) : TypeSystem(functionType) {
    this->retType = functionType.retType;
    if (functionType.args != this->args) {
        this->args = new vector<TypeSystem *>;
        copy(functionType.args->cbegin(), functionType.args->cend(), back_inserter(*this->args));
    }
}

FunctionType &FunctionType::operator=(const FunctionType &functionType) {
    this->retType = functionType.retType;
    if (functionType.args != this->args) {
        delete this->args;
        this->args = new vector<TypeSystem *>;
        copy(functionType.args->cbegin(), functionType.args->cend(), back_inserter(*this->args));
    }
    return *this;
}

FunctionType::~FunctionType() {
    delete this->args;
}

bool FunctionType::operator!=(const TypeSystem &typeSystem) const {
    return !operator==(typeSystem);
}

bool FunctionType::operator==(const TypeSystem &typeSystem) const {
    if (typeSystem.type != BASE_FUNCTION) {
        return false;
    } else {
        bool retEq = *((FunctionType &) typeSystem).retType == *this->retType;
        auto otherArgs = ((FunctionType &) typeSystem).args;
        if (!retEq || otherArgs->size() != this->args->size()) {
            return false;
        }
        for (int i = 0; i < otherArgs->size(); i++) {
            if (*((*otherArgs)[i]) != *((*this->args)[i])) {
                return false;
            }
        }
    }
    return TypeSystem::operator==(typeSystem);
}
