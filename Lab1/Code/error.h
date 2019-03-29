#define ERR_MISS_SEMI 1
#define ERR_MISS_SEMI_RETURN 8
#define ERR_MISS_COMMA_HIGH 2
#define ERR_MISS_COMMA_VARLIST 7
#define ERR_MISS_RC 3
#define ERR_MISS_RB 5
#define ERR_MISS_RP 14
#define ERR_MISS_RP_FUNC 15
#define ERR_MISS_STRUCTTAG 4
#define ERR_MISS_RETURNEXP 9
#define ERR_MISS_COMMA_ARGS 13

#define ERR_VARDEC 6
#define ERR_DEC 10
#define ERR_INDEX 12

#define ERR_EXPECT_EXP 11

extern int HAS_ERROR;

extern void errorReport(int errno, int lineno);
