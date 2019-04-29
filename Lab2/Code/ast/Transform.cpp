#include <iostream>
#include <vector>

#include "Transform.h"
//#include "../yystype.h"
#include "../syntax.tab.h"


using namespace std;

struct Program *transToAST(MultiNode *root)
{
#ifdef DEBUG
    cout << "Parse AST" << endl;
#endif
    Program *pg = transProgram(root);
    
// #ifdef DEBUG
//     cout << "Parse symbol" << endl;
// #endif
    // TypeVisitor visitor;
    // pg->accept(visitor);

    // ExpressionVisitor expressionVisitor;
    // expressionVisitor.symTable = visitor.symbolTable;
    // expressionVisitor.funcTable = visitor.funcTable;
    // pg->accept(expressionVisitor);

    // FunctionReturnVisitor functionReturnVisitor;
    // pg->accept(functionReturnVisitor);

    //    auto symbols = visitor.symbolTable;
    //    for (Symbol *symbol:*symbols) {
    //        cout << "Symbol: " << symbol->id << " type: " << symbol->typeElement->toString() << " lineno:"
    //             << symbol->beginLineno << endl;
    //    }
    //    PrintVisitor printVisitor;
    //    pg->accept(printVisitor);
    return pg;
}

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
        ExtDef *extDef = new FunDef(specifier, funDec, compSt);
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

    FunDec* funDec = new FunDec(idExp, *varList);
    return funDec;
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
        stmt = transExpStmt(node->childList[0]);
    }
    else if(!strcmp(node->name, COMPST)){
        //CompSt
        stmt = transCompSt(node->childList[0]);
    }
    else if(!strcmp(node->name, RETURN)){
        //RETURN Exp SEMI
        stmt = transRetStmt(node->childList[0]);
    }
    else if(!strcmp(node->name, IF)) {
        //IF LP Exp RP Stmt
        // IF LP Exp RP Stmt ELSE Stmt
        stmt = transIfElseStmt(node->childList[0]);
    }
    else if(!strcmp(node->name, WHILE)){
        //WHILE LP Exp RP Stmt
        stmt = transWhileStmt(node->childList[0]);
    }
    else{
        cout <<"Error in TransStmt()"<<endl;
        exit(-1);
    }
    return stmt;
}

ExpStmt *transExpStmt(MultiNode *node){
    //Exp SEMI
    Exp* exp = transExp(node->childList[0]);
    ExpStmt* expStmt = new ExpStmt(exp);
    return expStmt;
}

ReturnStmt *transRetStmt(MultiNode *node){
    //RETURN Exp SEMI
    Exp* exp = transExp(node->childList[1]);
    ReturnStmt* expStmt = new ReturnStmt(exp);
    return expStmt;
}

IfElseStmt *transIfElseStmt(MultiNode *node){
    //IF LP Exp RP Stmt
    //IF LP Exp RP Stmt ELSE Stmt
    Exp* condition = transExp(node->childList[2]);
    Stmt* thenBody = transStmt(node->childList[4]);
    Stmt* elseBody = nullptr;
    if(node->numChild == 7){
        elseBody = transStmt(node->childList[6]);
    }

    IfElseStmt* expStmt = new IfElseStmt(condition,thenBody,elseBody);
    return expStmt;
}

WhileStmt *transWhileStmt(MultiNode *node){
    //WHILE LP Exp RP Stmt
    Exp* condition = transExp(node->childList[2]);
    Stmt* body = transStmt(node->childList[4]);
    WhileStmt* expStmt = new WhileStmt(condition,body);
    return expStmt;
}

//local definition
vector<Definition *> *transDefList(MultiNode *node){
    vector<Definition*> *defList = new vector<Definition*>();
    if(!strcmp(node->name,EMPTY)){
        return defList;
    }
    else{
        //Def DefList
        Definition* def = transDef(node->childList[0]);
        vector<Definition*> *defListTail = transDefList(node->childList[1]);
        defListTail->insert(defListTail->begin(),def);
        return defListTail;
    }
}

Definition *transDef(MultiNode *node){
    //Specifier DecList SEMI
    Specifier *specifier = transSpecifier(node->childList[0]);
    vector<Dec*> *decList = transDecList(node->childList[1]);

    Definition* definition = new Definition(specifier, decList);
    return definition;
}

vector<Dec *> *transDecList(MultiNode *node){
    //Dec
    if(node->numChild == 1){
        Dec* dec = transDec(node->childList[0]);

        vector<Dec*> *decList = new vector<Dec*>();
        decList->insert(decList->begin(), dec);
        return decList;
    }
    else{
        //Dec COMMA DecList
        Dec* dec = transDec(node->childList[0]);
        vector<Dec*> *decListTail = transDecList(node->childList[2]);
        decListTail->insert(decListTail->begin(), dec);
        return decListTail;

    }
}

Dec *transDec(MultiNode *node){
    if(node->numChild == 1){
        //VarDec
        VarDec* varDec = transVarDec(node->childList[0]);
        Dec* dec = new NormalDec(varDec);
        return dec;
    }
    else{
        //VarDec ASSIGNOP Exp
        VarDec* varDec = transVarDec(node->childList[0]);
        Exp* exp = transExp(node->childList[2]);
        Dec* dec = new InitializedDec(varDec,exp);
        return dec;
    }

}


//Expression
Exp *transExp(MultiNode *node){
    Exp* exp;
    if(node->numChild == 3
    && strcmp(node->childList[0]->name,EXP) == 0
    && strcmp(node->childList[2]->name,EXP) == 0){
        //infix
        exp = transInfixExp(node);
    }
    else if(strcmp(node->childList[0]->name, LP) == 0){
        //LP Exp RP
        exp = transParenthesizedExp(node);
    }
    else if(strcmp(node->childList[0]->name, MINUS) == 0
         || strcmp(node->childList[0]->name, NOT) == 0){
        //MINUS Exp
        //NOT Exp
        exp = transPrefixExp(node);
    }
    else if(node->numChild > 1
         && strcmp(node->childList[0]->name, ID) == 0){
        //ID LP Args RP
        //ID LP RP
        exp = transFunExp(node);
    }
    else if(strcmp(node->childList[1]->name, LB) == 0){
        //Exp LB Exp RB
        exp = transArrayExp(node);
    }
    else if(strcmp(node->childList[1]->name, DOT) == 0){
        //Exp DOT ID
        exp = transStructExp(node);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, ID) == 0){
        exp = transIdExp(node);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, INT) == 0){
        exp = transIntValue(node);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, FLOAT) == 0){
        exp = transFloatValue(node);
    }

    return exp;
}

InfixExp *transInfixExp(MultiNode *node){
    //Exp Op Exp
    //TODO
    Exp* leftSide = transExp(node->childList[0]);
    Exp* rightSide = transExp(node->childList[2]);
    char* op = node->childList[1]->name;
    InfixOperator infixOperator;
    if(!strcmp(op,ASSIGNOP)){
        infixOperator = INFIX_ASSIGN;
    }else if(!strcmp(op,AND)){
        infixOperator = INFIX_AND;
    }else if(!strcmp(op,OR)){
        infixOperator = INFIX_OR;
    }else if(!strcmp(op,RELOP)){
        infixOperator = INFIX_RELOP;
    }else if(!strcmp(op,PLUS)){
        infixOperator = INFIX_PLUS;
    }else if(!strcmp(op,MINUS)){
        infixOperator = INFIX_MINUS;
    }else if(!strcmp(op,STAR)) {
        infixOperator = INFIX_STAR;
    }else if(!strcmp(op,DIV)) {
        infixOperator = INFIX_DIV;
    }else{
        cout<<"Illegal op"<<endl;
        exit(-1);
    }
    InfixExp* infixExp = new InfixExp(leftSide,infixOperator,rightSide);
    return infixExp;
}

ParenthesizedExp *transParenthesizedExp(MultiNode *node){
    //LP Exp RP
    Exp* exp = transExp(node->childList[1]);
    ParenthesizedExp* parenthesizedExp = new ParenthesizedExp(exp);
    return parenthesizedExp;
}

PrefixExp *transPrefixExp(MultiNode *node){
    //MINUS Exp
    //NOT Exp
    if(!strcmp(node->childList[0]->name,NOT)){
        //NOT Exp
        Exp* exp = transExp(node->childList[1]);

        PrefixExp* prefixExp = new PrefixExp(exp, PREFIX_NOT);
        return prefixExp;
    }
    else{
        //MINUS Exp
        Exp* exp = transExp(node->childList[1]);

        PrefixExp* prefixExp = new PrefixExp(exp, PREFIX_MINUS);
        return prefixExp;
    }
}

FunExp *transFunExp(MultiNode *node){
    //ID LP Args RP
    //ID LP RP
    if(node->numChild == 3){
        IDExp* idExp = transIdExp(node->childList[0]);
        FunExp* funExp = new FunExp(idExp);
        return funExp;
    }
    else{
        IDExp* idExp = transIdExp(node->childList[0]);
        vector<Exp*> *args = transArgs(node->childList[2]);

        FunExp* funExp = new FunExp(idExp, args);
        return funExp;
    }
}

ArrayExp *transArrayExp(MultiNode *node){
    //Exp LB Exp RB
    Exp* idExp = transExp(node->childList[0]);
    Exp* index = transExp(node->childList[2]);

    ArrayExp* arrayExp = new ArrayExp(idExp, index);
    return arrayExp;
}

StructExp *transStructExp(MultiNode *node){
    //Exp DOT ID
    Exp* exp = transExp(node->childList[0]);
    IDExp* idExp = transIdExp(node->childList[2]);

    StructExp* structExp = new StructExp(exp,idExp);
    return structExp;
}

IntExp *transIntValue(MultiNode *node){
    IntExp* intExp = new IntExp(atoi(node->attr));
    return intExp;
}

FloatExp *transFloatValue(MultiNode *node){
    FloatExp* floatExp = new FloatExp(node->attr);
    return floatExp;
}

IDExp *transIdExp(MultiNode *node) {
    IDExp *idExp = new IDExp(node->attr);
    return idExp;
}

vector<Exp *> *transArgs(MultiNode *node){
    vector<Exp*> *args = new vector<Exp*>();
    if(node->numChild == 1){
        //Exp
        Exp* exp = transExp(node->childList[0]);
        args->insert(args->begin(),exp);
        return args;
    }
    else{
        //Exp COMMA Args
        Exp* exp = transExp(node->childList[0]);
        args = transArgs(node->childList[2]);
        args->insert(args->begin(),exp);
        return args;
    }
}


