#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdint.h>

extern int yylineno;

typedef struct MultiNode{
    char name[30];
    char attr[32];
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

#define EMPTY "%empty"