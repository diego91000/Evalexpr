#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    INT,
    ADD,
    SUB,
    DIV,
    MUL,
    POW,
    UADD,
    USUB,
    MOD,
    GPAR,
    DPAR
};

struct token
{
    enum token_type type;
    int value;
};

struct fifo;
struct fifo *lex_stdin(int rpn);

#endif /* ! TOKEN_H */
