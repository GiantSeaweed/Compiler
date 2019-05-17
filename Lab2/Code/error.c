#include<stdio.h>
#include "error.h"
#include "syntax.tab.h"
// int HAS_ERROR = 0;

void errorReport(int errno, int lineno){
    HAS_ERROR = 1;
    switch (errno)
    {
        /*** Miss sth ***/
        case ERR_MISS_SEMI:
            printf("Error Type B at Line %d: Missing \";\".\n", lineno);
            break;
        case ERR_MISS_SEMI_RETURN:
            printf("Error Type B at Line %d: Missing \";\" in return sentence.\n",lineno);
            break;
        case ERR_MISS_COMMA_HIGH:
            printf("Error Type B at Line %d: Missing \",\" in high-level declaration list.\n", lineno);
            break;
        case ERR_MISS_RC:
            printf("Error Type B at Line %d: Missing \"}\".\n", lineno);
            break;
        case ERR_MISS_RB:
            printf("Error Type B at Line %d: Missing \"]\".\n", lineno);
            break;
        case ERR_MISS_RP:
            printf("Error Type B at Line %d: Missing \")\".\n", lineno);
            break;
        case ERR_MISS_RP_FUNC:
            printf("Error Type B at Line %d: Missing \")\" in function call.\n", lineno);
            break;
        case ERR_MISS_STRUCTTAG:
            printf("Error Type B at Line %d: Missing tag of struct.\n", lineno);
            break;
        case ERR_MISS_COMMA_VARLIST:
            printf("Error Type B at Line %d: Missing \",\" in variant list.\n", lineno);
            break;
        case ERR_MISS_COMMA_ARGS:
            printf("Error Type B at Line %d: Missing \",\" in argument list.\n", lineno);
            break;
        case ERR_MISS_RETURNEXP:
            printf("Error Type B at Line %d: Missing return expression.\n", lineno);
            break;

        /*** Declaration ***/
        case ERR_VARDEC:
            printf("Error Type B at Line %d: Something wrong about varible declaration.\n", lineno);
            break;
        case ERR_DEC:
            printf("Error Type B at Line %d: Something wrong with declaration.\n", lineno);
            break;
        case ERR_INDEX:
            printf("Error Type B at Line %d: Something wrong with index.\n", lineno);
            break;
        case ERR_FUNDEC:
            printf("Error Type B at Line %d: C-- does not allow function declaration.\n", lineno);
            break;
        /*** Expected sth ***/
        case ERR_EXPECT_EXP:
            printf("Error Type B at Line %d: Expected expression.\n", lineno);
            break;
        default:
            break;
    }

    // char c = input();
    // while(c != '\n'){
    //     c = input();
    // }
}