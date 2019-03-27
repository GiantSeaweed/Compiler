%locations
%{ 
#include "lex.yy.c"
#include "tree.h"
void yyerror(char* msg);

%}

%union{
    int ival;
    float fval;
    char* sval;
    struct MultiNode *m_node;
}

%token <m_node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP
%token <m_node> PLUS MINUS STAR DIV AND OR DOT NOT TYPE
%token <m_node> LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%type <m_node> Program ExtDefList ExtDef ExtDecList
%type <m_node> Specifier StructSpecifier OptTag Tag
%type <m_node> VarDec FunDec VarList ParamDec
%type <m_node> CompSt StmtList Stmt
%type <m_node> DefList Def DecList Dec
%type <m_node> Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LC RC LB RB LP RP DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
Program : 
    ExtDefList{
        $$ = createMultiTree("Program");
        insertNon($$, $1);
        // printf("%d %s\n",yylineno, $$->name);
        printTree($$, 0);
    }
    ;
ExtDefList : 
    ExtDef ExtDefList{
        $$ = createMultiTree("ExtDefList");
        insertNon($$, $1);
        insertNon($$, $2);
    }
    | %empty {
        $$ = createMultiTree(EMPTY);
        // insertTerm($$, EMPTY, @$.first_line);
    }
    ;
ExtDef : 
    Specifier ExtDecList SEMI{
        $$ = createMultiTree("ExtDef");
        insertNon($$, $1);
        insertNon($$, $2);
        insertTerm($$, "SEMI", @3.first_line);
    }
    | Specifier SEMI{
        $$ = createMultiTree("ExtDef");
        insertNon($$, $1);
        insertTerm($$, "SEMI", @2.first_line);
    }
    | Specifier FunDec CompSt{
        $$ = createMultiTree("ExtDef");
        insertNon($$, $1);
        insertNon($$, $2);
        insertNon($$, $3);
    }
    | Specifier error{
        printf("Error Type B at Line %d: Expected \";\" after struct.\n", @1.last_line);
    }
    ;
ExtDecList : 
    VarDec{
        $$ = createMultiTree("ExtDecList");
        insertNon($$, $1);
    }
    | VarDec COMMA ExtDecList{
        $$ = createMultiTree("ExtDecList");
        insertNon($$, $1);
        insertTerm($$, "COMMA", @2.first_line);
        insertNon($$, $3);
    }
    | ExtDecList error ExtDecList{
        printf("Error Type B at Line %d: Missing \",\" in high-level declaration list.\n", @1.first_line);
    }
    ;

Specifier : 
    TYPE{
        $$ = createMultiTree("Specifier");
        insertTermAttr($$, "TYPE", yylval.sval, @1.first_line);
    }
    | StructSpecifier{
        $$ = createMultiTree("Specifier");
        insertNon($$, $1);
    }
    ;
StructSpecifier : 
    STRUCT OptTag LC DefList RC{
        $$ = createMultiTree("StructSpecifier");
        insertTerm($$, "STRUCT", yylineno);
        insertNon($$, $2);
        insertTerm($$, "LC", yylineno);
        insertNon($$, $4);
        insertTerm($$, "RC", yylineno);
    }
    | STRUCT Tag{
        $$ = createMultiTree("StructSpecifier");
        insertTerm($$, "STRUCT", yylineno);
        insertNon($$, $2);
    }
    ;
OptTag : 
    ID
    {
        $$ = createMultiTree("OptTag");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
    }
    | %empty
    {
        $$ = createMultiTree(EMPTY);
        // insertTerm($$, EMPTY, @$.first_line);
    }
    ;
Tag : 
    ID{
        $$ = createMultiTree("Tag");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
    }
    ;

VarDec : 
    ID{
        $$ = createMultiTree("VarDec");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
    }
    | VarDec LB INT RB{
        $$ = createMultiTree("VarDec");
        insertNon($$, $1);
        insertTerm($$, "LB", @2.first_line);
        insertTermAttr($$, "INT", yylval.sval, @3.first_line);
        insertTerm($$, "RB", @4.first_line);
    }
    ;
ParamDec : 
    Specifier VarDec{
        $$ = createMultiTree("ParamDec");
        insertNon($$, $1);
        insertNon($$, $2);
    }
    ;
VarList : 
    ParamDec COMMA VarList{
        $$ = createMultiTree("VarList");
        insertNon($$, $1);
        insertTerm($$, "COMMA", @2.first_line);
        insertNon($$, $3);
    }
    | ParamDec{
        $$ = createMultiTree("VarList");
        insertNon($$, $1);
    }
    | VarList error VarList {
        printf("Error Type B at Line %d: Missing \",\" in variant list.\n", @1.first_line);
    }
    ;
FunDec : 
    ID LP VarList RP{
        $$ = createMultiTree("FunDec");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
    }
    | ID LP RP{
        $$ = createMultiTree("FunDec");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertTerm($$, "RP", @3.first_line);
    }
    ;

CompSt : 
    LC DefList StmtList RC{
        $$ = createMultiTree("CompSt");
        insertTerm($$, "LC", @1.first_line);
        insertNon($$, $2);
        insertNon($$, $3);
        insertTerm($$, "RC", @4.first_line);
    }
    | LC error RC{
        printf("Error Type B at Line %d: Unexpected DefList after StmtList.\n", @2.first_line);
    }
    ;
StmtList : 
    Stmt StmtList{
        $$ = createMultiTree("StmtList");
        insertNon($$, $1);
        insertNon($$, $2);
    }
    | %empty{
        $$ = createMultiTree(EMPTY);
        // insertTerm($$, EMPTY, @$.first_line);
    }
    | Stmt error{
        printf("Error Type B at Line %d: Something wrong among statements.\n", @1.last_line);
    }
    ;
Stmt : 
    Exp SEMI{
        $$ = createMultiTree("Stmt");
        insertNon($$, $1);
        insertTerm($$, "SEMI", @2.first_line);
    }
    | CompSt
    {
        $$ = createMultiTree("Stmt");
        insertNon($$, $1);
    }
    | RETURN Exp SEMI
    {
        $$ = createMultiTree("Stmt");
        insertTerm($$, "RETURN", @1.first_line);
        insertNon($$, $2);
        insertTerm($$, "SEMI", @3.first_line);
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    {
        $$ = createMultiTree("Stmt");
        insertTerm($$, "IF", @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
        insertNon($$, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt
    {
        $$ = createMultiTree("Stmt");
        insertTerm($$, "IF", @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
        insertNon($$, $5);
        insertTerm($$, "ELSE", @6.first_line);
        insertNon($$, $7);
    }
    | WHILE LP Exp RP Stmt
    {
        $$ = createMultiTree("Stmt");
        insertTerm($$, "WHILE", @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
        insertNon($$, $5);
    }
    | Exp error
    {
        printf("Error Type B at Line %d: Missing \";\".\n", @1.first_line);
    }
    | RETURN error
    {
        printf("Error Type B at Line %d: Missing return expression.\n", @1.first_line);
    }
    | RETURN Exp error
    {
        printf("Error Type B at Line %d: Missing \";\".\n", @1.first_line);
    }
    ;

DefList : 
    Def DefList
    {
        $$ = createMultiTree("DefList");
        insertNon($$, $1);
        insertNon($$, $2);
    }
    | %empty
    {
        $$ = createMultiTree(EMPTY);
        // insertTerm($$, EMPTY, @$.first_line);
    }
    
    ;
Def : 
    Specifier DecList SEMI
    {
        $$ = createMultiTree("Def");
        insertNon($$, $1);
        insertNon($$, $2);
        insertTerm($$, "SEMI", @3.first_line);
    }
    | Specifier DecList error
    {
        printf("Error Type B at Line %d: Missing \";\".\n", @1.first_line);
    }
    | Specifier error
    {
        printf("Error Type B at Line %d: Missing \",\".\n", @1.first_line);
    }    
    ;
DecList : 
    Dec
    {
        $$ = createMultiTree("DecList");
        insertNon($$, $1);
    }
    | Dec COMMA DecList
    {
        $$ = createMultiTree("DecList");
        insertNon($$, $1);
        insertTerm($$, "COMMA", yylineno);
        insertNon($$, $3);
    }
    
    
    ;
Dec : 
    VarDec{
        $$ = createMultiTree("Dec");
        insertNon($$, $1);
    }
    | VarDec ASSIGNOP Exp
    {
        $$ = createMultiTree("Dec");
        insertNon($$, $1);
        insertTerm($$, "ASSIGNOP", yylineno);
        insertNon($$, $3);
    }
    | VarDec ASSIGNOP error
    {
        printf("Error Type B at Line %d: Expected expression.\n", @1.first_line);
    }
    | error ASSIGNOP Exp
    {
        printf("Error Type B at Line %d: Expected identifier.\n", @1.first_line);
    }
    ;

Exp : 
    Exp ASSIGNOP Exp{
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "ASSIGNOP", @2.first_line);
        insertNon($$, $3);
    }
    | Exp ASSIGNOP error
    {
        printf("Error Type B at Line %d: Expected expression.\n", @1.first_line);
    }
    
    | Exp AND Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "AND", @2.first_line);
        insertNon($$, $3);
    }
    | Exp OR Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "OR", @2.first_line);
        insertNon($$, $3);
    }
    | Exp RELOP Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "RELOP", @2.first_line);
        insertNon($$, $3);
    }
    | Exp PLUS Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "PLUS", @2.first_line);
        insertNon($$, $3);
    }
    | Exp MINUS Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "MINUS", @2.first_line);
        insertNon($$, $3);
    }
    | Exp STAR Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "STAR", @2.first_line);
        insertNon($$, $3);
    }
    | Exp DIV Exp
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "DIV", @2.first_line);
        insertNon($$, $3);
    }
    | LP Exp RP
    {
        $$ = createMultiTree("Exp");
        insertTerm($$, "LP", @1.first_line);
        insertNon($$, $2);
        insertTerm($$, "RP", @3.first_line);
    }
    | MINUS Exp
    {
        $$ = createMultiTree("Exp");
        insertTerm($$, "MINUS", @1.first_line);
        insertNon($$, $2);
        
    }
    | NOT Exp
    {
        $$ = createMultiTree("Exp");
        insertTerm($$, "NOT", @1.first_line);
        insertNon($$, $2);
    }
    | ID LP Args RP
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
    }
    | ID LP RP
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertTerm($$, "RP", @3.first_line);
    }
    | Exp LB Exp RB
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "LB", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RB", @4.first_line);
    }
    | Exp DOT ID
    {
        $$ = createMultiTree("Exp");
        insertNon($$, $1);
        insertTerm($$, "DOT", @2.first_line);
        insertTermAttr($$, "ID", yylval.sval, @3.first_line);
    }
    | ID
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "ID", yylval.sval, @1.first_line);
    }
    | INT
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "INT", yylval.sval, @1.first_line);
    }
    | FLOAT
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "FLOAT", yylval.sval, @1.first_line);
    }
    ;
Args : 
    Exp COMMA Args
    {
        $$ = createMultiTree("Args");
        insertNon($$, $1);
        insertTerm($$, "COMMA", yylineno);
        insertNon($$, $3);

    }
    | Exp
    {
        $$ = createMultiTree("Args");
        insertNon($$, $1);
    }
    | Args error Args
    {
        printf("Error Type B at Line %d: Missing \",\" in argument list.\n", @1.first_line);
    }
    ;

%%
void yyerror(char* msg) {
    // fprintf(stderr, "error: %s; %s %d\n", msg, yytext, yylineno);
}