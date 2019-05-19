#include "tree.h"

MultiNode *createMultiTree(const char *name){
    MultiNode* node = malloc(sizeof(MultiNode));
    if(!node){
        printf("out of space\n");
        exit(0);
    }
    node->bufSize = 8;
    node->numChild = 0;
    node->firstLine = INT32_MAX;
    strcpy(node->name, name);
    node->childList = (MultiNode **)malloc(node->bufSize * sizeof(MultiNode *));
    return node;
}

void insertNon(MultiNode *parent, MultiNode* child)
{
    if(!parent){
        printf("Null pointer parent!\n");
        exit(0);
    }
    
    if(parent->numChild >= parent->bufSize){
        printf("Too many children!\n");
        exit(0);
    }
    parent->childList[parent->numChild] = child;
    parent->numChild++;
    if (parent->firstLine > child->firstLine){
        parent->firstLine = child->firstLine;
    }
    // printf("Non: %s %d\n", child->name, parent->firstLine);
}

void insertTerm(MultiNode *parent, char *childName, int lineno)
{
    if (!parent){
        printf("Null pointer parent!\n");
        exit(0);
    }
    if (parent->numChild >= parent->bufSize){
        printf("Too many children!\n");
        exit(0);
    }

    MultiNode* node = malloc(sizeof(MultiNode));
    strcpy(node->name, childName);
    node->firstLine = lineno;
    node->numChild = 0;

    parent->childList[parent->numChild] = node;
    parent->numChild++;
    if (parent->firstLine > node->firstLine){
        parent->firstLine = node->firstLine;
    }
}

void insertTermAttr(MultiNode *parent, char *childName, 
                    char* attribute, int lineno)
{
    if (!parent){
        printf("Null pointer parent!\n");
        exit(0);
    }
    if (parent->numChild >= parent->bufSize){
        printf("Too many children!\n");
        exit(0);
    }

    MultiNode *node = malloc(sizeof(MultiNode));
    strcpy(node->name, childName);
    strcpy(node->attr, attribute);

    node->firstLine = lineno;
    node->numChild = 0;

    parent->childList[parent->numChild] = node;
    parent->numChild++;
    if (parent->firstLine > node->firstLine)
    {
        parent->firstLine = node->firstLine;
    }
}

void printIndent(int num){
    for(int i=0;i<num;i++){
        printf("  ");
    }
}

void printTree(MultiNode *node, int depth){
    if(!node){
        printf("This node is null/cannot be printed!\n");
        exit(0);
    }

    // syntax && \epsilon
    if(strcmp(node->name, EMPTY) == 0){
        return;
    }
    printIndent(depth);

    // syntax && not \epsilon
    if (strcmp(node->name, "ID") == 0 
     || strcmp(node->name, "TYPE") == 0){
        printf("%s: %s\n", node->name, node->attr);
    }
    else if(strcmp(node->name, "INT") == 0){
        printf("%s: %d\n", node->name, atoi(node->attr));
    }
    else if (strcmp(node->name, "FLOAT") == 0){
        printf("%s: %s\n", node->name, node->attr);
    }
    else if(node->numChild > 0){//Nonterminals
        printf("%s (%d)\n", node->name, node->firstLine);
    }
    // else if (strcmp(node->name, "RELOP") == 0){
    //     printf("RELOP: %s\n", node->attr);
    // }
    else{
        printf("%s\n", node->name);
    }

    for (int i = 0; i < node->numChild; i++){
        printTree(node->childList[i], depth + 1);
    }
}