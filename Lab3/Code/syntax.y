%locations
%{ 
#include "lex.yy.c"
#include "tree.h"
#include "syntax.tab.h"

#include "error.h"
void yyerror(char* msg);
int yylex(void);

struct MultiNode *root = 0;
int HAS_ERROR = 0;

%}

%union{
    int ival;
    float fval;
    char* sval;
    char* relop_text[2];
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
        if(HAS_ERROR == 0){
            // printTree($$, 0);
            //transToAST($$);
            root = $$;
        }
            
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
    | Specifier FunDec SEMI {
        errorReport(ERR_FUNDEC, @1.first_line);
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
        errorReport(ERR_MISS_COMMA_HIGH, @1.first_line);
        // printf("Error Type B at Line %d: Missing \",\" in high-level declaration list.\n", @1.first_line);
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
    | STRUCT OptTag LC DefList error {
        errorReport(ERR_MISS_RC, @4.last_line+1);
        // printf("Error Type B at Line %d: Missing \"}\".\n", @4.last_line+1);
    }
    | STRUCT error {
        errorReport(ERR_MISS_STRUCTTAG, @1.last_line);
        // printf("Error Type B at Line %d: Missing tag of struct.\n", @1.last_line);
    }
    ;
OptTag : 
    ID
    {
        $$ = createMultiTree("OptTag");
        insertTermAttr($$, "ID", $1->name, @1.first_line);
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
        insertTermAttr($$, "ID", $1->name, @1.first_line);
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
    | VarDec LB error {
        errorReport(ERR_VARDEC, @1.first_line);
        // printf("Error Type B at Line %d: Something wrong about the declaration after \"[\".\n", @1.first_line);
    }
    | VarDec LB INT error {
        errorReport(ERR_MISS_RB,@1.first_line);
        // printf("Error Type B at Line %d: Missing \"]\".\n", @1.first_line);
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
        errorReport(ERR_MISS_COMMA_VARLIST, @1.first_line);
        // printf("Error Type B at Line %d: Missing \",\" in variant list.\n", @1.first_line);
    }
    ;
FunDec : 
    ID LP VarList RP{
        $$ = createMultiTree("FunDec");
        insertTermAttr($$, "ID", $1->name, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
    }
    | ID LP RP{
        $$ = createMultiTree("FunDec");
        insertTermAttr($$, "ID", $1->name, @1.first_line);
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
    | LC DefList StmtList error{
        errorReport(ERR_MISS_RC, @3.last_line+1);
        // printf("Error Type B at Line %d: Missing \"}\".\n", @3.last_line+1);
    }
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
    
    ;
Stmt : 
    Exp SEMI{
        $$ = createMultiTree("Stmt");
        insertNon($$, $1);
        insertTerm($$, "SEMI", @2.first_line);
    }
    | Exp error{
        errorReport(ERR_MISS_SEMI, @1.first_line);
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
    | RETURN error SEMI
    {
        errorReport(ERR_MISS_RETURNEXP, @1.first_line);
        // printf("Error Type B at Line %d: Missing return expression.\n", @1.first_line);
    }
    | RETURN Exp error
    {
        errorReport(ERR_MISS_SEMI_RETURN,@1.first_line);
        // printf("Error Type B at Line %d: Missing \";\" in return sentence.\n", @1.first_line);
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
    | Specifier DecList error SEMI
    {
        errorReport(ERR_DEC, @1.first_line);
        // printf("Error Type B at Line %d: Missing \";\" after declaration.\n", @1.first_line);
    }
    | Specifier error SEMI
    {
        errorReport(ERR_DEC, @1.first_line);
        // printf("Error Type B at Line %d: Something wrong with declaration.\n", @1.last_line);
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
        errorReport(ERR_EXPECT_EXP, @1.first_line);
        // printf("Error Type B at Line %d: Expected expression.\n", @1.first_line);
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
        errorReport(ERR_EXPECT_EXP, @1.first_line);
        // printf("Error Type B at Line %d: Expected expression.\n", @1.first_line);
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
        insertTermAttr($$, "RELOP", yylval.relop_text[1], @2.first_line);
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
    | LP Exp error
    {
        errorReport(ERR_MISS_RP, @2.last_line);
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
        insertTermAttr($$, "ID", $1->name, @1.first_line);
        insertTerm($$, "LP", @2.first_line);
        insertNon($$, $3);
        insertTerm($$, "RP", @4.first_line);
    }
    | ID LP Args error
    {
        errorReport(ERR_MISS_RP_FUNC, @3.last_line);
    }
    | ID LP RP
    {
        $$ = createMultiTree("Exp");
        insertTermAttr($$, "ID", $1->name, @1.first_line);
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
    | Exp LB error RB
    {
        errorReport(ERR_INDEX, @1.first_line);
        // printf("Error Type B at Line %d: Something wrong with index.\n", @1.first_line);
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
        errorReport(ERR_MISS_COMMA_ARGS, @1.first_line);
        // printf("Error Type B at Line %d: Missing \",\" in argument list.\n", @1.first_line);
    }
    ;

%%
void yyerror(char* msg) {
    // fprintf(stderr, "error: %s; %s %d\n", msg, yytext, yylineno);
}