#include <iostream>
#include <vector>

#include "Transform.h"
//#include "../yystype.h"
#include "../syntax.tab.h"


using namespace std;
// high level definition
Program *transProgram(MultiNode *node) {
    vector<ExtDef*> *extDefList = transExtDefList(node->childList[0]);

    Program* program = new Program(extDefList);

    return program;
}

vector<ExtDef *> *transExtDefList(MultiNode *node){
    vector<ExtDef*> *extDefListTail = new vector<ExtDef*>();
    if(!(strcmp(node->name,EMPTY)) ){
        return extDefListTail; //recursively
    }

    ExtDef* extDef = transExtDef(node->childList[0]);
    extDefListTail = transExtDefList(node->childList[1]);
    extDefListTail->insert(extDefListTail->begin(), extDef);

    return extDefListTail;
}

ExtDef *transExtDef(MultiNode *node){
    Specifier* specifier = transSpecifier(node->childList[0]);
    if(!strcmp(node->childList[1]->name, EXTDECLIST)){
        //Specifier ExtDecList SEMI
        vector<VarDec*> *varDecList = transExtDecList(node->childList[1]);
        ExtDef* extDef = new DecDef(specifier, varDecList);
        return extDef;
    }
    else if(!strcmp(node->childList[1]->name, SEMI)){
        //Specifier SEMI
        ExtDef* extDef = new TypeDef(specifier);
        return extDef;
    }
    else if(!strcmp(node->childList[1]->name, FUNDEC)) {
        //Specifier FunDec CompSt
        FunDec *funDec = transFunDec(node->childList[1]);
        CompSt *compSt = transCompSt(node->childList[2]);
        ExtDef *extDef = new FunDec(specifier, funDec, compSt);
        return extDef;

    }
}

vector<VarDec*> *transExtDecList(MultiNode* node){
    VarDec* varDec = transVarDec(node->childList[0]);
    if(node->numChild == 1){
        //VarDec
        vector<VarDec*> *extDefList = new vector<VarDec*>();
        extDefList->insert(extDefList->begin(),varDec);
        return extDefList;
    }
    else if(node->numChild == 3){
        //VarDec COMMA ExtDecList
        vector<VarDec*> *varDecTail = transExtDecList(node->childList[2]);
        varDecTail->insert(varDecTail->begin(), varDec);
        return varDecTail;
    }
}

//specifier
Specifier *transSpecifier(MultiNode *node){
    if(!strcmp(node->childList[0]->name, TYPE)){
        Specifier* specifier = transBasicSpecifier(node->childList[0]);
        return specifier;
    }
    else{
        Specifier* specifier = transStructSpecifier(node->childList[0]);
        return specifier;
    }
}

BasicSpecifier *transBasicSpecifier(MultiNode* node){
    if(!strcmp(node->name, INT)){
        BasicSpecifier* basicSpecifier = new BasicSpecifier(BASICTYPE_INT);
        return basicSpecifier;
    }
    else if(!strcmp(node->name, FLOAT)){
        BasicSpecifier* basicSpecifier = new BasicSpecifier(BASICTYPE_FLOAT);
        return basicSpecifier;
    }
}

StructSpecifier *transStructSpecifier(MultiNode* node){
    if(node->numChild == 2){
        //STRUCT Tag
        IDExp *tag = transTag(node->childList[1]);
        StructSpecifier* structSpecifier = new NormalStructSpecifier(tag);
        return structSpecifier;
    }
    else if(node->numChild == 5){
        //STRUCT OptTag LC DefList RC
        IDExp *optTag = transOptionalTag(node->childList[1]);
        vector<Definition*> *defList = transDefList(node->childList[3]);

        StructSpecifier* structSpecifier = new DefStructSpecifier(optTag, defList);
        return structSpecifier;
    }

    cout<<"Error in transStructSpecifier()"<<endl;
    exit(-1);
}

IDExp *transOptionalTag(MultiNode *node){
    //tag or \epsilon
    if(!strcmp(node->name, EMPTY)){
        return nullptr;
    }
    else{
        IDExp* tag = transIdExp(node->childList[0]);
        return tag;
    }
}

IDExp *transTag(MultiNode *node){
    //tag
    IDExp* tag = transIdExp(node->childList[0]);
    return tag;

}

//Declarator
VarDec *transVarDec(MultiNode *node){
    if(node->numChild == 1){
        //ID
        IDExp* id = transIdExp(node->childList[0]);

        NormalVarDec* normalVarDec = new NormalVarDec(id);
        return normalVarDec;
    }
    else{
        //VarDec LB INT RB
        VarDec* varDec = transVarDec(node->childList[0]);
        int intValue = atoi(node->childList[2]->attr);

        ArrayDec* arrayDec = new ArrayDec(varDec, intValue);
        return arrayDec;
    }
}

FunDec *transFunDec(MultiNode *node){
    IDExp* idExp = transIdExp(node->childList[0]);
    //ID LP RP
    vector<ParamDec*> *varList = new vector<ParamDec*>();

    if(node->numChild == 4){
        //ID LP VarList RP
        varList = transVarList(node->childList[2]);
    }

    FunDec* funDec = new FunDec(idExp, varList);
}

vector<ParamDec *> *transVarList(MultiNode *node){
    vector<ParamDec*>* varList = new vector<ParamDec*>();
    if(node->numChild == 1){
        //ParamDec
        ParamDec* paramDec = transParamDec(node->childList[0]);
        varList->insert(varList->begin(), paramDec);
    }
    else{
        //ParamDec COMMA VarList
        ParamDec* paramDec = transParamDec(node->childList[0]);
        varList = transVarList(node->childList[2]);
        varList->insert(varList->begin(), paramDec);
    }
    return varList;
}

ParamDec *transParamDec(MultiNode *node){
    //Specifier VarDec
    Specifier* specifier = transSpecifier(node->childList[0]);
    VarDec* varDec =transVarDec(node->childList[1]);

    ParamDec* paramDec = new ParamDec(specifier, varDec);
    return paramDec;
}


//Statements
CompSt *transCompSt(MultiNode *node){
    //LC DefList StmtList RC
    vector<Definition*> *defList = transDefList(node->childList[1]);
    vector<Stmt*> *stmtList = transStmtList(node->childList[2]);

    CompSt* compSt = new CompSt(defList, stmtList);
    return compSt;
}

vector<Stmt *> *transStmtList(MultiNode *node){
    vector<Stmt*> *stmtListTail = new vector<Stmt*>();
    if(!(strcmp(node->name,EMPTY)) ){
        return stmtListTail; //recursively
    }
    //Stmt StmtList
    Stmt* stmt = transStmt(node->childList[0]);
    stmtListTail = transStmtList(node->childList[1]);
    stmtListTail->insert(stmtListTail->begin(), stmt);

    return stmtListTail;
}

Stmt *transStmt(MultiNode *node){
    Stmt* stmt;
    if(!strcmp(node->name, EXP)){
        //Exp SEMI
        stmt = transExp(node->childList[0]);
    }
    else if(!strcmp(node->name, COMPST)){
        //CompSt
        stmt = transCompSt(node->childList[0]);
    }
    else if(!strcmp(node->name, RETURN)){
        //RETURN Exp SEMI
        stmt = transRetStmt(node->childList[0]);
    }
    else if(!strcmp(node->name, IF)){
        //IF LP Exp RP Stmt
        // IF LP Exp RP Stmt ELSE Stmt

    else if(!strcmp(node->name, WHILE)){
        //WHILE LP Exp RP Stmt
        stmt = transRetStmt(node->childList[0]);
    }
}

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

IntExp *transIntLiteral(MultiNode *node);

FloatExp *transFloatLiteral(MultiNode *node);

IDExp *transIdExp(MultiNode *node);

vector<Exp *> *transArgs(MultiNode *node);


