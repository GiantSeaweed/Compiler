#include <iostream>
#include <vector>

#include "Transform.h"
#include "type.h"
//#include "../yystype.h"
#include "../syntax.tab.h"
#include "visitor/TypeVisitor.h"
#include "visitor/ExpressionVisitor.h"
#include "visitor/FunReturnVisitor.h"
#include "visitor/PrintVisitor.h"

#include "ir/IRBuilderVisitor.h"
#include "ir/IRInstruction.h"

using namespace std;

void printNode(MultiNode* node){
    if(node == nullptr){
        cout<<"This is a nullptr!"<<endl;
        return;
    }
    cout<<"name:"<<node->name<<endl;
    cout<<"child:"<<node->numChild<<endl;
    for(int i =0 ;i<node->numChild;i++){
        cout << "\t" << i << ":" << node->childList[i]->name << "," << node->childList[i]->attr << endl;
    }
}

Program *transToAST(MultiNode *root)
{
#ifdef DEBUG
    cout << "Parsing AST" << endl;
#endif
    Program *program = transProgram(root);
    
 #ifdef DEBUG
     cout << "Get AST! Begin visiting!" << endl;
 #endif
     TypeVisitor visitor;
     program->accept(visitor);
#ifdef DEBUG
#endif
     auto symbols = visitor.symbolTable;
     for (Symbol *symbol : *symbols)
     {
         cout << "Symbol: " << symbol->id << "\ttype: " << symbol->typeSystem->toString() << " lineno:"
              << symbol->firstLine << endl;
     }
     symbols = visitor.funTable;
     for (Symbol *symbol : *symbols)
     {
         cout << "Symbol: " << symbol->id << "\ttype: " << symbol->typeSystem->toString() << " lineno:"
              << symbol->firstLine << endl;
     }

     cout << "******Finish TypeVisiting! Begin ExpVisiting!******" << endl;

     ExpressionVisitor expressionVisitor;
     expressionVisitor.symTable = visitor.symbolTable;
     expressionVisitor.funTable = visitor.funTable;
     program->accept(expressionVisitor);
#ifdef DEBUG
    cout << "******Finish ExpVisiting! Begin FunVisiting!********" << endl;
#endif

    FunReturnVisitor funReturnVisitor;
    program->accept(funReturnVisitor);

    IRBuilderVisitor irBuilderVisitor;//(visitor.symbolTable, visitor.funTable);
    irBuilderVisitor.symbolTable = visitor.symbolTable;
    irBuilderVisitor.funTable = visitor.funTable;
    program->accept(irBuilderVisitor);
    // cout<<irBuilderVisitor.irList->size()<<endl;
    irBuilderVisitor.printIRList();

    // PrintVisitor printVisitor;
    // program->accept(printVisitor);
    return program;
}

// high level definition
Program *transProgram(MultiNode *node) {
#ifdef DEBUG
    cout << "Parsing Program!" << endl;
    printNode(node);
#endif
    vector<ExtDef*> *extDefList = transExtDefList(node->childList[0]);
    Program* program = new Program(extDefList);
    program->beginLine = node->firstLine;
    return program;
}

vector<ExtDef *> *transExtDefList(MultiNode *node){
    vector<ExtDef*> *extDefListTail = new vector<ExtDef*>();

    if(!(strcmp(node->name,EMPTY)) ){
        return extDefListTail; //recursively
    }
#ifdef DEBUG
    cout << "Parsing ExtDefList" << endl;
#endif
    ExtDef* extDef = transExtDef(node->childList[0]);
    extDefListTail = transExtDefList(node->childList[1]);
    extDefListTail->insert(extDefListTail->begin(), extDef);
    return extDefListTail;
}

ExtDef *transExtDef(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing ExtDef" << endl;
#endif
    Specifier* specifier = transSpecifier(node->childList[0]);
    if(!strcmp(node->childList[1]->name, EXTDECLIST)){
        //Specifier ExtDecList SEMI
        vector<VarDec*> *varDecList = transExtDecList(node->childList[1]);
        ExtDef* extDef = new DecDef(specifier, varDecList);
        extDef->beginLine = node->firstLine;
        return extDef;
    }
    else if(!strcmp(node->childList[1]->name, SEMI_STR)){
        //Specifier SEMI
        ExtDef* extDef = new TypeDef(specifier);
        extDef->beginLine = node->firstLine;
        return extDef;
    }
    else if(!strcmp(node->childList[1]->name, FUNDEC)) {
        //Specifier FunDec CompSt
        FunDec *funDec = transFunDec(node->childList[1]);
        CompSt *compSt = transCompSt(node->childList[2]);
        ExtDef *extDef = new FunDef(specifier, funDec, compSt);
        extDef->beginLine = node->firstLine;
        return extDef;

    }


}

vector<VarDec*> *transExtDecList(MultiNode* node){
#ifdef DEBUG
    cout << "Parsing ExtDecList" << endl;
#endif
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
#ifdef DEBUG
    cout << "Parsing Specifier" << endl;
    printNode(node);
#endif
    if(!strcmp(node->childList[0]->name, TYPE_STR)){
        Specifier* specifier = transBasicSpecifier(node->childList[0]);
        specifier->beginLine = node->firstLine;
        return specifier;
    }
    else{
        Specifier* specifier = transStructSpecifier(node->childList[0]);
        specifier->beginLine = node->firstLine;
        return specifier;
    }
}

BasicSpecifier *transBasicSpecifier(MultiNode* node){
#ifdef DEBUG
    cout << "Parsing BasicSpecifier" << endl;
//    printNode(node);
#endif
    if(!strcmp(node->attr, "int")){
#ifdef DEBUG
        cout << "Parsing BasicSpecifier::INT" << endl;
#endif
        BasicSpecifier* basicSpecifier = new BasicSpecifier(BASICTYPE_INT);
        basicSpecifier->beginLine = node->firstLine;
        return basicSpecifier;
    }
    else if(!strcmp(node->attr, "float")){
#ifdef DEBUG
        cout << "Parsing BasicSpecifier::Float" << endl;
#endif
        BasicSpecifier* basicSpecifier = new BasicSpecifier(BASICTYPE_FLOAT);
        basicSpecifier->beginLine = node->firstLine;
        return basicSpecifier;
    }
}

StructSpecifier *transStructSpecifier(MultiNode* node){
#ifdef DEBUG
    cout << "Parsing StructSpecifier" << endl;
#endif
    if(node->numChild == 2){
        //STRUCT Tag
        IDExp *tag = transTag(node->childList[1]);
        StructSpecifier* structSpecifier = new NormalStructSpecifier(tag);
        structSpecifier->beginLine = node->firstLine;
        return structSpecifier;
    }
    else if(node->numChild == 5){
        //STRUCT OptTag LC DefList RC
        IDExp *optTag = transOptionalTag(node->childList[1]);
        vector<Definition*> *defList = transDefList(node->childList[3]);

        StructSpecifier* structSpecifier = new DefStructSpecifier(optTag, defList);
        structSpecifier->beginLine = node->firstLine;
        return structSpecifier;
    }

    cout<<"Error in transStructSpecifier()"<<endl;
    exit(-1);
}

IDExp *transOptionalTag(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing OptTag" << endl;
#endif
    //tag or \epsilon
    if(!strcmp(node->name, EMPTY)){
        return nullptr;
    }
    else{
        IDExp* tag = transIdExp(node->childList[0]);
        tag->beginLine = node->firstLine;
        return tag;
    }
}

IDExp *transTag(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing Tag" << endl;
#endif
    //tag
    IDExp* tag = transIdExp(node->childList[0]);
    tag->beginLine = node->firstLine;
    return tag;

}

//Declarator
VarDec *transVarDec(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing VarDec" << endl;
#endif
    if(node->numChild == 1){
        //ID
        IDExp* id = transIdExp(node->childList[0]);

        NormalVarDec* normalVarDec = new NormalVarDec(id);
        normalVarDec->beginLine = node->firstLine;
        return normalVarDec;
    }
    else{
        //VarDec LB INT RB
        VarDec* varDec = transVarDec(node->childList[0]);
        int intValue = atoi(node->childList[2]->attr);

        ArrayDec* arrayDec = new ArrayDec(varDec, intValue);
        arrayDec->beginLine = node->firstLine;
        return arrayDec;
    }
}

FunDec *transFunDec(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing FunDec" << endl;
#endif
    IDExp* idExp = transIdExp(node->childList[0]);
    //ID LP RP
    vector<ParamDec*> *varList = new vector<ParamDec*>();

    if(node->numChild == 4){
        //ID LP VarList RP
        varList = transVarList(node->childList[2]);
    }

    FunDec* funDec = new FunDec(idExp, varList);
    funDec->beginLine = node->firstLine;
    return funDec;
}

vector<ParamDec *> *transVarList(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing VarList" << endl;
#endif
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
#ifdef DEBUG
    cout << "Parsing ParamDec" << endl;
#endif
    //Specifier VarDec
    Specifier* specifier = transSpecifier(node->childList[0]);
    VarDec* varDec =transVarDec(node->childList[1]);

    ParamDec* paramDec = new ParamDec(specifier, varDec);
    paramDec->beginLine = node->firstLine;
    return paramDec;
}


//Statements
CompSt *transCompSt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing CompSt" << endl;
#endif
    //LC DefList StmtList RC
    vector<Definition*> *defList = transDefList(node->childList[1]);
    vector<Stmt*> *stmtList = transStmtList(node->childList[2]);

    CompSt* compSt = new CompSt(defList, stmtList);
    compSt->beginLine = node->firstLine;
    return compSt;
}

vector<Stmt *> *transStmtList(MultiNode *node){

    vector<Stmt*> *stmtListTail = new vector<Stmt*>();
    if(!(strcmp(node->name,EMPTY)) ){
        return stmtListTail; //recursively
    }
#ifdef DEBUG
    cout << "Parsing StmtList" << endl;
#endif
    //Stmt StmtList
    Stmt* stmt = transStmt(node->childList[0]);
    stmtListTail = transStmtList(node->childList[1]);
    stmtListTail->insert(stmtListTail->begin(), stmt);

    return stmtListTail;
}

Stmt *transStmt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing Stmt" << endl;
    // printNode(node);
    // cout << strcmp(node->name, RETURN_STR)<<endl;
#endif
    Stmt *stmt;
    if(!strcmp(node->childList[0]->name, EXP)){
        //Exp SEMI
        stmt = transExpStmt(node);
    }
    else if (!strcmp(node->childList[0]->name, COMPST)){
        //CompSt
        stmt = transCompSt(node->childList[0]);
    }
    else if (!strcmp(node->childList[0]->name, RETURN_STR)){
        //RETURN Exp SEMI
        stmt = transRetStmt(node);
    }
    else if (!strcmp(node->childList[0]->name, IF_STR)){
        //IF LP Exp RP Stmt
        // IF LP Exp RP Stmt ELSE Stmt
        stmt = transIfElseStmt(node);
    }
    else if (!strcmp(node->childList[0]->name, WHILE_STR)){
        //WHILE LP Exp RP Stmt
        stmt = transWhileStmt(node);
    }
    else{
        cout <<"Error in TransStmt()"<<endl;
        exit(-1);
    }
    stmt->beginLine = node->firstLine;

    return stmt;
}

ExpStmt *transExpStmt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing ExpStmt" << endl;
#endif
    //Exp SEMI
    Exp* exp = transExp(node->childList[0]);
    ExpStmt* expStmt = new ExpStmt(exp);
    expStmt->beginLine = node->firstLine;
    return expStmt;
}

ReturnStmt *transRetStmt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing RetStmt" << endl;
#endif
    //RETURN Exp SEMI
    Exp* exp = transExp(node->childList[1]);
    ReturnStmt* expStmt = new ReturnStmt(exp);
    expStmt->beginLine = node->firstLine;
    return expStmt;
}

IfElseStmt *transIfElseStmt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing IfElseStmt" << endl;
#endif
    //IF LP Exp RP Stmt
    //IF LP Exp RP Stmt ELSE Stmt
    Exp* condition = transExp(node->childList[2]);
    Stmt* thenBody = transStmt(node->childList[4]);
    Stmt* elseBody = nullptr;
    if(node->numChild == 7){
        elseBody = transStmt(node->childList[6]);
    }

    IfElseStmt* expStmt = new IfElseStmt(condition,thenBody,elseBody);
    expStmt->beginLine = node->firstLine;
    return expStmt;
}

WhileStmt *transWhileStmt(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing WhileStmt" << endl;
#endif
    //WHILE LP Exp RP Stmt
    Exp* condition = transExp(node->childList[2]);
    Stmt* body = transStmt(node->childList[4]);
    WhileStmt* expStmt = new WhileStmt(condition,body);
    expStmt->beginLine = node->firstLine;
    return expStmt;
}

//local definition
vector<Definition *> *transDefList(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing DefList" << endl;
#endif
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
#ifdef DEBUG
    cout << "Parsing Def" << endl;
#endif
    //Specifier DecList SEMI
    Specifier *specifier = transSpecifier(node->childList[0]);
    vector<Dec*> *decList = transDecList(node->childList[1]);

    Definition* definition = new Definition(specifier, decList);
    definition->beginLine = node->firstLine;
    return definition;
}

vector<Dec *> *transDecList(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing DecLIst" << endl;
#endif
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
#ifdef DEBUG
    cout << "Parsing Dec" << endl;
    printNode(node);
    // cout << "\tchild " << node->numChild << endl;
    // cout << "\t\t" << node->childList[2]->numChild <<endl;
#endif
    if(node->numChild == 1){
        //VarDec
        VarDec* varDec = transVarDec(node->childList[0]);
        Dec* dec = new NormalDec(varDec);
        dec->beginLine = node->firstLine;
        return dec;
    }
    else{
        //VarDec ASSIGNOP Exp
        VarDec* varDec = transVarDec(node->childList[0]);
        Exp* exp = transExp(node->childList[2]);
        Dec* dec = new InitializedDec(varDec,exp);
        dec->beginLine = node->firstLine;
        return dec;
    }

}


//Expression
Exp *transExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing Exp" << endl;
#endif
    Exp* exp;
    if(node->numChild == 3
    && strcmp(node->childList[0]->name,EXP) == 0
    && strcmp(node->childList[2]->name,EXP) == 0){
        //infix
#ifdef DEBUG
        cout << "Parsing Exp::Infix" << endl;
#endif
        exp = transInfixExp(node);
    }
    else if(strcmp(node->childList[0]->name, LP_STR) == 0){
        //LP Exp RP
#ifdef DEBUG
        cout << "Parsing Exp::( exp )" << endl;
#endif
        exp = transParenthesizedExp(node);
    }
    else if(strcmp(node->childList[0]->name, MINUS_STR) == 0
         || strcmp(node->childList[0]->name, NOT_STR) == 0){
        //MINUS Exp
        //NOT Exp
#ifdef DEBUG
        cout << "Parsing Exp::Prefix" << endl;
#endif
        exp = transPrefixExp(node);
    }
    else if(node->numChild > 1
         && strcmp(node->childList[0]->name, ID_STR) == 0){
        //ID LP Args RP
        //ID LP RP
#ifdef DEBUG
        cout << "Parsing Exp::FunExp" << endl;
#endif
        exp = transFunExp(node);
    }
    else if(node->numChild == 4
        &&  strcmp(node->childList[1]->name, LB_STR) == 0){
        //Exp LB Exp RB
#ifdef DEBUG
        cout << "Parsing Exp::Array" << endl;
#endif
        exp = transArrayExp(node);
    }
    else if(node->numChild == 3
        && strcmp(node->childList[1]->name, DOT_STR) == 0){
        //Exp DOT ID
#ifdef DEBUG
        cout << "Parsing Exp::Struct" << endl;
#endif
        exp = transStructExp(node);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, ID_STR) == 0){
#ifdef DEBUG
        cout << "Parsing Exp::ID" << endl;
#endif
        //hao keng a !
        exp = transIdExp(node->childList[0]);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, INT_STR) == 0){
#ifdef DEBUG
        cout << "Parsing Exp::INT" << endl;
#endif
        exp = transIntValue(node->childList[0]);
    }
    else if(node->numChild == 1
            && strcmp(node->childList[0]->name, FLOAT_STR) == 0){
#ifdef DEBUG
        cout << "Parsing Exp::FLOAT" << endl;
#endif
        exp = transFloatValue(node->childList[0]);
    }else{
        cout << "Magic Exp" <<endl;
        exit(-1);
    }
    exp->beginLine = node->firstLine;
    return exp;
}

InfixExp *transInfixExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing InfixExp" << endl;
#endif
    //Exp Op Exp
    //TODO
    Exp* leftSide = transExp(node->childList[0]);
    Exp* rightSide = transExp(node->childList[2]);
    char* op = node->childList[1]->name;
    InfixOperator infixOperator;
    if(!strcmp(op,ASSIGNOP_STR)){
        infixOperator = INFIX_ASSIGN;
    }else if(!strcmp(op,AND_STR)){
        infixOperator = INFIX_AND;
    }else if(!strcmp(op,OR_STR)){
        infixOperator = INFIX_OR;
    }else if(!strcmp(op,RELOP_STR)){
        char* attr = node->childList[1]->attr;
        if(!strcmp(attr, ">")){
            infixOperator = INFIX_GT;
        }else if(!strcmp(attr, ">=")){
            infixOperator = INFIX_GE;
        }else if(!strcmp(attr, "<")){
            infixOperator = INFIX_LT;
        }else if(!strcmp(attr, "<=")){
            infixOperator = INFIX_LE;
        }else if(!strcmp(attr, "==")){
            infixOperator = INFIX_EQ;
        }else if(!strcmp(attr, "!=")){
            infixOperator = INFIX_NE;
        }
        // infixOperator = INFIX_RELOP;
    }else if(!strcmp(op,PLUS_STR)){
        infixOperator = INFIX_PLUS;
    }else if(!strcmp(op,MINUS_STR)){
        infixOperator = INFIX_MINUS;
    }else if(!strcmp(op,STAR_STR)) {
        infixOperator = INFIX_STAR;
    }else if(!strcmp(op,DIV_STR)) {
        infixOperator = INFIX_DIV;
    }else{
        cout<<"Illegal op"<<endl;
        exit(-1);
    }
    InfixExp* infixExp = new InfixExp(leftSide,infixOperator,rightSide);
    infixExp->beginLine = node->firstLine;
    return infixExp;
}

ParenthesizedExp *transParenthesizedExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing parenthesizedExp" << endl;
#endif
    //LP Exp RP
    Exp* exp = transExp(node->childList[1]);
    ParenthesizedExp* parenthesizedExp = new ParenthesizedExp(exp);
    parenthesizedExp->beginLine = node->firstLine;
    return parenthesizedExp;
}

PrefixExp *transPrefixExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing PrefixExp" << endl;
#endif
    //MINUS Exp
    //NOT Exp
    if(!strcmp(node->childList[0]->name,NOT_STR)){
        //NOT Exp
        Exp* exp = transExp(node->childList[1]);

        PrefixExp* prefixExp = new PrefixExp(exp, PREFIX_NOT);
        prefixExp->beginLine = node->firstLine;
        return prefixExp;
    }
    else{
        //MINUS Exp
        Exp* exp = transExp(node->childList[1]);

        PrefixExp* prefixExp = new PrefixExp(exp, PREFIX_MINUS);
        prefixExp->beginLine = node->firstLine;
        return prefixExp;
    }
}

FunExp *transFunExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing FunExp" << endl;
#endif
    //ID LP Args RP
    //ID LP RP
    if(node->numChild == 3){
        IDExp* idExp = transIdExp(node->childList[0]);
        vector<Exp*> *args = new vector<Exp*>;
        FunExp* funExp = new FunExp(idExp,args);
//        FunExp* funExp = new FunExp(idExp);

        funExp->beginLine = node->firstLine;
        return funExp;
    }
    else{
        IDExp* idExp = transIdExp(node->childList[0]);
        vector<Exp*> *args = transArgs(node->childList[2]);

        FunExp* funExp = new FunExp(idExp, args);
        funExp->beginLine = node->firstLine;
        return funExp;
    }
}

ArrayExp *transArrayExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing ArrayExp" << endl;
#endif
    //Exp LB Exp RB
    Exp* idExp = transExp(node->childList[0]);
    Exp* index = transExp(node->childList[2]);

    ArrayExp* arrayExp = new ArrayExp(idExp, index);
    arrayExp->beginLine = node->firstLine;
    return arrayExp;
}

StructExp *transStructExp(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing StructExp" << endl;
#endif
    //Exp DOT ID
    Exp* exp = transExp(node->childList[0]);
    IDExp* idExp = transIdExp(node->childList[2]);

    StructExp* structExp = new StructExp(exp,idExp);
    structExp->beginLine = node->firstLine;
    return structExp;
}

IntExp *transIntValue(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing IntValue" << endl;
    cout << "\tIntValue:" << atoi(node->attr)<<endl;
#endif
    IntExp *intExp = new IntExp(atoi(node->attr));
    intExp->beginLine = node->firstLine;
    return intExp;
}

FloatExp *transFloatValue(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing FloatValue" << endl;
#endif
    FloatExp* floatExp = new FloatExp(node->attr);
    floatExp->beginLine = node->firstLine;
    return floatExp;
}

IDExp *transIdExp(MultiNode *node) {
#ifdef DEBUG
    cout << "Parsing IDExp:" << node->attr<< endl;
#endif
    IDExp *idExp = new IDExp(node->attr);
    idExp->beginLine = node->firstLine;
    return idExp;
}

vector<Exp *> *transArgs(MultiNode *node){
#ifdef DEBUG
    cout << "Parsing Args" << endl;
#endif
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


