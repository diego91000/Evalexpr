#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"

static int isdig(int c)
{
    return c >= '0' && c <= '9';
}

static struct token *tok(enum token_type t, int v)
{
    struct token *x = malloc(sizeof(*x));
    if (!x)
        return NULL;
    x->type = t;
    x->value = v;
    return x;
}

static void push_op(struct fifo **q, enum token_type t)
{
    *q = fifo_push(*q, tok(t, -1));
}

static int read_int(int c)
{
    int v = c - '0';
    int d;
    while ((d = fgetc(stdin)) != EOF && isdig(d))
        v = v * 10 + (d - '0');
    if (d != EOF)
        ungetc(d, stdin);
    return v;
}

static int handle_par(int c, int rpn, struct fifo **q, int *expect_unary)
{
    if (!rpn && c == '(')
    {
        push_op(q, GPAR);
        *expect_unary = 1;
        return 1;
    }
    if (!rpn && c == ')')
    {
        push_op(q, DPAR);
        *expect_unary = 0;
        return 1;
    }
    return 0;
}

static int handle_pm(int c, int rpn, struct fifo **q, int *expect_unary)
{
    if (c == '+')
    {
        if (rpn)
            push_op(q, ADD);
        else
        {
            if (*expect_unary)
                push_op(q, UADD);
            else
                push_op(q, ADD);
        }
        *expect_unary = 1;
        return 1;
    }
    if (c == '-')
    {
        if (rpn)
            push_op(q, SUB);
        else
        {
            if (*expect_unary)
                push_op(q, USUB);
            else
                push_op(q, SUB);
        }
        *expect_unary = 1;
        return 1;
    }
    return 0;
}

static int handle_ops(int c, struct fifo **q, int *expect_unary)
{
    if (c == '*')
    {
        push_op(q, MUL);
        *expect_unary = 1;
        return 1;
    }
    if (c == '/')
    {
        push_op(q, DIV);
        *expect_unary = 1;
        return 1;
    }
    if (c == '%')
    {
        push_op(q, MOD);
        *expect_unary = 1;
        return 1;
    }
    if (c == '^')
    {
        push_op(q, POW);
        *expect_unary = 1;
        return 1;
    }
    return 0;
}

struct fifo *lex_stdin(int rpn)
{
    struct fifo *q = fifo_new();
    int c;
    int expect_unary = 1;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (isspace(c))
            continue;
        if (isdig(c))
        {
            q = fifo_push(q, tok(INT, read_int(c)));
            expect_unary = 0;
            continue;
        }
        if (handle_par(c, rpn, &q, &expect_unary))
            continue;
        if (handle_pm(c, rpn, &q, &expect_unary))
            continue;
        if (handle_ops(c, &q, &expect_unary))
            continue;
    }
    return q;
}
