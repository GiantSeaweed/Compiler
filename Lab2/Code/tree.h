#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdint.h>

extern int yylineno;

typedef struct MultiNode{
    char name[50];
    char attr[50];
    int bufSize;
    int numChild;
    int firstLine;
    struct MultiNode **childList;
}MultiNode;

extern MultiNode *createMultiTree(const char *name);

extern void insertNon(MultiNode *parent, MultiNode *child);
extern void insertTerm(MultiNode *parent, char *childName, int lineno);
extern void insertTermAttr(MultiNode *parent, char *childName, char* attr, int lineno);

extern void printIndent(int num);
extern void printTree();

//node name
#define EMPTY "%empty"
#define EXTDEFLIST "ExtDefList"
#define EXTDECLIST "ExtDecList"
#define FUNDEC "FunDec"
#define COMPST "CompSt"
#define EXP "Exp"

#define RETURN "RETURN"
#define INT  "INT"
#define FLOAT "FLOAT"
#define SEMI "SEMI"
#define TYPE "TYPE"
#define IF "IF"
#define WHILE "WHILE"
#define NOT "NOT"
#define LP "LP"
#define LB "LB"
#define RP "RP"
#define RB "RB"
#define ID "ID"
#define DOT "DOT"

#define ASSIGNOP "ASSIGNOP"
#define AND "AND"
#define OR "OR"
#define RELOP "RELOP"
#define PLUS "PLUS"
#define MINUS "MINUS"
#define STAR "STAR"
#define DIV "DIV"
