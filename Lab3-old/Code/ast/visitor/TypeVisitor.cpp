//
// Created by 冯诗伟 on 2019-04-29.
//

#include "TypeVisitor.h"
#include "../utils/utils.h"
#include <set>
#include <stack>
#include <string>
#include <iostream>

using namespace std;

string TypeVisitor::genAnonymousName(ASTNode &node) {
    int i = node.beginLine;
    return string("AnoName") + to_string(i);
}


bool TypeVisitor::checkAndAddIntoSymbolTable(Symbol *symbol) {
    auto sym = this->symbolTable->find(symbol);
    if (sym != this->symbolTable->end()) {
        return false;
    }

    this->symbolTable->insert(symbol);
    return true;
}

bool TypeVisitor::checkAndAddIntoFunTable(Symbol *symbol) {
    auto sym = this->funTable->find(symbol);
    if (sym != this->funTable->end()) {
        return false;
    }
    this->funTable->insert(symbol);
    return true;
}

Symbol *TypeVisitor::lookupSymbolTable(Symbol *symbol) {
    auto result = this->symbolTable->find(symbol);
    if (result == this->symbolTable->end()) {
        return nullptr;
    }
    return *result;
}

TypeVisitor::TypeVisitor() {
    this->symbolTable = new set<Symbol *, decltype(op)>(op);
    this->funTable = new set<Symbol *, decltype(op)>(op);
}

//definition
bool TypeVisitor::visit(Definition &definition) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::Definition" <<endl;
#endif
    definition.specifier->accept(*this);

    for (Dec *dec : *definition.decList) {
        dec->typeSystem = definition.specifier->typeSystem;
        dec->accept(*this);

    }
    return VisitorFalse::visit(definition);
}
bool TypeVisitor::visit(DecDef &hlDef) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::DecDef" <<endl;
#endif
    hlDef.specifier->accept(*this);
    for (VarDec *varDec : *hlDef.extDecList) {
        varDec->typeSystem = hlDef.specifier->typeSystem;
        varDec->accept(*this);
    }
    for (VarDec *globalDec:*hlDef.extDecList) {
        auto id = globalDec->getID();
        Symbol tmp(id);
        auto globalSym = lookupSymbolTable(&tmp);
        if (globalSym != nullptr) {
            globalSym->endLine = GLOBAL;
        } else {
            cerr << "Error in TypeVisitor::visit(DecDef)" << endl;
        }

    }
    return VisitorFalse::visit(hlDef);
}
bool TypeVisitor::visit(FunDef &hlDef) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::FunDef" <<endl;
#endif
    hlDef.retSpecifier->accept(*this);
    TypeSystem* retSpecifier = hlDef.retSpecifier->typeSystem;

    hlDef.funDec->accept(*this);
    vector<TypeSystem *> paramsType;
    for (ParamDec *paramDec : *hlDef.funDec->paramList) {
        string paramID = paramDec->varDec->getID();
        Symbol tmp(paramID);
        Symbol* paramSymbol = lookupSymbolTable(&tmp);
        if (paramSymbol != nullptr) {
//            paramSymbol->endLineno = hlDef.endLineNo;
        } else {
            cerr << "error at typeVisitor.visit(FunDef&)" << endl;
        }
        paramsType.push_back(paramDec->typeSystem);
    }


    FunctionType* newType = new FunctionType(BASE_FUNCTION, retSpecifier, paramsType);
    hlDef.typeSystem = newType;
    Symbol* newSymbol = new Symbol(&hlDef, newType, hlDef.funDec->id->id);
    newSymbol->endLine = GLOBAL;

    if (!checkAndAddIntoFunTable(newSymbol)) {
#ifdef SEMANTIC
        //FUNC symbol Error
        printError(4, "Function " + newSymbol->id + " has been defined!", hlDef.beginLine);
#endif
    }

    hlDef.compSt->accept(*this);
    auto locals = hlDef.compSt->defList;
    for (Definition *localDef : *locals) {
        auto localDecs = localDef->decList;
        for (Dec *localDec:*localDecs) {
            string localId = localDec->getVarDec()->getID();
            Symbol tmp(localId);
            Symbol* localSym = lookupSymbolTable(&tmp);
            if (localSym != nullptr) {
//                localSym->endLineno = hlDef.endLineNo;
            } else {
                cerr << "error at typeVisitor.visit(FunDef&)" << endl;
            }

        }
    }
    return VisitorFalse::visit(hlDef);
}

bool TypeVisitor::visit(Program &hlDef) {
#ifdef DEBUG
    cout << "visiting Typevisitor::Program" <<endl;
#endif
    return true;
}

bool TypeVisitor::visit(TypeDef &hlDef) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::TypeDef" <<endl;
#endif
    return true;
}

//Specifier
bool TypeVisitor::visit(DefStructSpecifier &specifier) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::DefStructSpecifier" <<endl;
#endif
    //a global state indicating that we are analyzing inside a struct
    bool oriInStruct = this->inStruct;
    this->inStruct = true;


    string symbol;
    if (specifier.optTag == nullptr) {
        symbol = genAnonymousName(static_cast<ASTNode &>(specifier));
    } else {
        symbol = specifier.optTag->id;
    }
    auto dfList = specifier.defList;
    auto fieldList = new vector<TypeSystem *>;
    auto structSymTable = new set<SymbolHead *, decltype(symbolIOp)>(symbolIOp);

    for (Definition *df:*dfList) {
        df->accept(*this);
        for (Dec *d:*df->decList) {
            auto var = d->getVarDec();
            fieldList->push_back(var->typeSystem);
            auto sym = new Symbol(var, var->typeSystem, var->getID());
            auto toFind = structSymTable->find(sym);
            if (toFind != structSymTable->end()) {
#ifdef SEMANTIC
                //duplicate field
                printError(15, "Struct field " + sym->id + " is redefined!", sym->firstLine);
#endif
                delete sym;
            } else {
                structSymTable->insert(sym);
            }
        }
    }
    auto type = new StructType(BASE_STRUCT, *fieldList, symbol);
    type->symbolTable = structSymTable;

    specifier.typeSystem = type;

    auto result = new Symbol(&specifier, type, symbol);
    if (!checkAndAddIntoSymbolTable(result)) {
#ifdef SEMANTIC
        //duplicate define
        printError(16, "Struct name " + result->id + " redefined!", result->firstLine);
#endif
        delete result;
    }

    this->inStruct = oriInStruct;
    return VisitorFalse::visit(specifier);
}
bool TypeVisitor::visit(BasicSpecifier &specifier) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::BasicSpecifier" <<endl;
#endif
    BaseType baseType;
    switch (specifier.basicType) {
        case BASICTYPE_INT:
            baseType = BASE_INT;
            break;
        case BASICTYPE_FLOAT:
            baseType = BASE_FLOAT;
            break;
        default:
            cerr << "Error when visit primitive type specifier" << endl;
            exit(-1);
    }
    auto typeSystem = new TypeSystem(baseType);
    typeSystem->size = 4;

    specifier.typeSystem = typeSystem;

    return VisitorFalse::visit(specifier);
}
bool TypeVisitor::visit(NormalStructSpecifier &specifier) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::NormalStructSpecifier" <<endl;
#endif
    Symbol tmp(&specifier, nullptr, specifier.tag->id);
    auto pSymbol = lookupSymbolTable(&tmp);
    if (pSymbol == nullptr) {
#ifdef SEMANTIC
        //undefined reference error
        printError(17, "Struct " + tmp.id + " is undefined!", specifier.beginLine);
#endif
        specifier.typeSystem = new TypeSystem(BASE_DEFAULT);

    } else {
        specifier.typeSystem = pSymbol->typeSystem;
    }
    return VisitorFalse::visit(specifier);
}

string arrayPlaceHolder;


//declarator
bool TypeVisitor::visit(ArrayDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::ArrayDec" <<endl;
#endif
    bool topLevel = declarator.typeSystem->type != BASE_ARRAY;
    TypeSystem *oriType;

    if (topLevel) {
        oriType = declarator.typeSystem;
    }
    ArrayType tmp(oriType, -1);
    declarator.varDec->typeSystem = &tmp;
    declarator.varDec->accept(*this);

    TypeSystem* realType = declarator.varDec->typeSystem;
    ArrayType* newType = new ArrayType(realType, declarator.size);
    newType->size = declarator.size * realType->size;
    declarator.typeSystem = newType;
    if (topLevel) {
        auto result = new Symbol(&declarator, declarator.typeSystem, arrayPlaceHolder);
        arrayPlaceHolder = "";
        if (!checkAndAddIntoSymbolTable(result)) {
#ifdef SEMANTIC

            if (!this->inStruct) {
                //redefined vars
                printError(3, "Var " + result->id + " is redefined!", declarator.beginLine);
            } else {
                //redefined field
                printError(15, "Struct field " + result->id + " is redefined!", declarator.beginLine);
            }
#endif
        }
    }
    return VisitorFalse::visit(declarator);
}

bool TypeVisitor::visit(InitializedDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::InitializedDec" <<endl;
#endif
    declarator.varDec->typeSystem = declarator.typeSystem;
    declarator.varDec->accept(*this);
    //to deal with array type, check the initiator
    auto realType = declarator.varDec->typeSystem;

    declarator.typeSystem = realType;
#ifdef SEMANTIC
    if (this->inStruct) {
        //redefined vars
        printError(15, "Init field " + declarator.varDec->getID() + " in struct", declarator.beginLine);
    }
#endif
    return VisitorFalse::visit(declarator);
}
bool TypeVisitor::visit(FunDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::FunDec" <<endl;
#endif
    for (ParamDec *d : *declarator.paramList) {
        d->accept(*this);
    }
    return VisitorFalse::visit(declarator);
}
bool TypeVisitor::visit(ParamDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::ParamDec" <<endl;
#endif
    declarator.specifier->accept(*this);
    auto type = declarator.specifier->typeSystem;
    declarator.varDec->typeSystem = type;
    declarator.varDec->accept(*this);
    declarator.typeSystem = declarator.varDec->typeSystem;
    return VisitorFalse::visit(declarator);
}
bool TypeVisitor::visit(NormalVarDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::NormalVarDec" <<endl;
#endif
    if (declarator.typeSystem->type == BASE_ARRAY) {
        auto realType = ((ArrayType *) declarator.typeSystem)->base;
        declarator.typeSystem = realType;
        arrayPlaceHolder = declarator.id->id;
        return VisitorFalse::visit(declarator);
    }
    auto type = declarator.typeSystem;

    Symbol* result = new Symbol(&declarator, type, declarator.id->id);
    if (!this->inStruct) {
        if (!checkAndAddIntoSymbolTable(result)) {
#ifdef SEMANTIC
            //redefined vars
            printError(3, "Var " + result->id + " is redefined!", declarator.beginLine);
#endif
        }
    }

    return VisitorFalse::visit(declarator);
}
bool TypeVisitor::visit(NormalDec &declarator) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::NormakDec" <<endl;
#endif
    declarator.varDec->typeSystem = declarator.typeSystem;
    declarator.varDec->accept(*this);

    return VisitorFalse::visit(declarator);
}

//statement
bool TypeVisitor::visit(IfElseStmt &stmt) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::IfElseStmt" <<endl;
#endif
    stmt.thenBody->accept(*this);
    if(stmt.elseBody != nullptr){
        stmt.elseBody->accept(*this);
    }
    return VisitorFalse::visit(stmt);
}

bool TypeVisitor::visit(WhileStmt &stmt) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::WhileStmt" <<endl;
#endif
    stmt.body->accept(*this);
    return VisitorFalse::visit(stmt);
}

bool TypeVisitor::visit(CompSt &stmt) {
#ifdef DEBUG
    cout << "visiting TypeVisitor::CompSt" <<endl;
#endif
    for (Definition *d:*stmt.defList) {
        d->accept(*this);
        auto decs = d->decList;
        for (Dec *dec1:*decs) {
            string id = dec1->getVarDec()->getID();
            Symbol tmp(id);
            Symbol* sym = lookupSymbolTable(&tmp);
            if (sym != nullptr) {
//                sym->endLineno = stmt.endLineNo;
            }
        }
    }
    for (Stmt *s:*stmt.stmtList) {
        s->accept(*this);
    }

    return VisitorFalse::visit(stmt);
}




