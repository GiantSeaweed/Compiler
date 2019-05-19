#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdint.h>

#ifndef TREE_H
#define TREE_H
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

//node name; used in Transform.cpp
#define EMPTY "%empty"
#define EXTDEFLIST "ExtDefList"
#define EXTDECLIST "ExtDecList"
#define FUNDEC "FunDec"
#define COMPST "CompSt"
#define EXP "Exp"

#define RETURN_STR "RETURN"
#define INT_STR  "INT"
#define FLOAT_STR "FLOAT"
#define SEMI_STR "SEMI"
#define TYPE_STR "TYPE"
#define IF_STR "IF"
#define WHILE_STR "WHILE"
#define NOT_STR "NOT"
#define LP_STR "LP"
#define LB_STR "LB"
#define RP_STR "RP"
#define RB_STR "RB"
#define ID_STR "ID"
#define DOT_STR "DOT"

#define ASSIGNOP_STR "ASSIGNOP"
#define AND_STR "AND"
#define OR_STR "OR"
#define RELOP_STR "RELOP"
#define PLUS_STR "PLUS"
#define MINUS_STR "MINUS"
#define STAR_STR "STAR"
#define DIV_STR "DIV"

#endif