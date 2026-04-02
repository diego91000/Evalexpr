#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fifo.h"
#include "stack.h"
#include "token.h"

static int prec(enum token_type t)
{
    if (t == UADD || t == USUB)
        return 4;
    if (t == POW)
        return 3;
    if (t == MUL || t == DIV || t == MOD)
        return 2;
    if (t == ADD || t == SUB)
        return 1;
    return 0;
}

static int right_assoc(enum token_type t)
{
    if (t == POW)
        return 1;
    if (t == UADD || t == USUB)
        return 1;
    return 0;
}

static void op_to_out(struct stack_t **op, struct fifo **out)
{
    struct token *x;
    *op = st_pop(*op, &x);
    *out = fifo_push(*out, x);
}

static void flush_until_gpar(struct stack_t **op, struct fifo **out)
{
    while (!st_empty(*op) && (*op)->tok->type != GPAR)
        op_to_out(op, out);
    if (!st_empty(*op) && (*op)->tok->type == GPAR)
    {
        struct token *g;
        *op = st_pop(*op, &g);
        free(g);
    }
}

static void drain_ops(struct stack_t **op, struct fifo **out, struct token **pt)
{
    struct token *t = *pt;

    if (t->type == USUB && !st_empty(*op) && (*op)->tok->type == SUB)
    {
        (*op)->tok->type = ADD;
        free(t);
        *pt = NULL;
        return;
    }

    while (!st_empty(*op))
    {
        int ptop = prec((*op)->tok->type);
        int ptok = prec(t->type);
        int ra = right_assoc(t->type);
        if (ptop > ptok)
        {
            op_to_out(op, out);
            continue;
        }
        if (!ra && ptop >= ptok)
        {
            op_to_out(op, out);
            continue;
        }
        break;
    }
}

static struct fifo *shunt(struct fifo *in)
{
    struct fifo *out = fifo_new();
    struct stack_t *op = NULL;
    while (!fifo_empty(in))
    {
        struct token *t;
        in = fifo_pop(in, &t);
        if (t->type == INT)
            out = fifo_push(out, t);
        else if (t->type == GPAR)
            op = st_push(op, t);
        else if (t->type == DPAR)
        {
            flush_until_gpar(&op, &out);
            free(t);
        }
        else
        {
            drain_ops(&op, &out, &t);
            if (t)
                op = st_push(op, t);
        }
    }
    while (!st_empty(op))
        op_to_out(&op, &out);
    st_destroy_keep(op);
    fifo_destroy_nodes(in);
    return out;
}

/* parrties EVAL DU RPN */ //

static int calc_bin(enum token_type t, int a, int b)
{
    int r = 0;
    if (t == ADD)
        r = a + b;
    else if (t == SUB)
        r = a - b;
    else if (t == MUL)
        r = a * b;
    else if (t == DIV)
    {
        if (b != 0)
            r = a / b;
    }
    else if (t == MOD)
    {
        if (b != 0)
            r = a % b;
    }
    else if (t == POW)
    {
        int s = 1;
        while (b > 0)
        {
            s *= a;
            b--;
        }
        r = s;
    }
    return r;
}

static void depile_deux(struct stack_t **st, struct token **a, struct token **b)
{
    *b = NULL;
    *a = NULL;
    if (!st_empty(*st))
        *st = st_pop(*st, b);
    if (!st_empty(*st))
        *st = st_pop(*st, a);
}

static void pousse_zero(struct stack_t **st)
{
    struct token *z = malloc(sizeof(*z));
    if (!z)
        return;
    z->type = INT;
    z->value = 0;
    *st = st_push(*st, z);
}

static void fait_unaire(struct stack_t **st, struct token *t)
{
    if (!st_empty(*st))
    {
        struct token *x;
        *st = st_pop(*st, &x);
        if (x && x->type == INT && t->type == USUB)
            x->value = -x->value;
        *st = st_push(*st, x);
    }
    free(t);
}

static void fait_binaire(struct stack_t **st, struct token *t)
{
    struct token *a;
    struct token *b;
    depile_deux(*&st, &a, &b);
    if (a && b && a->type == INT && b->type == INT)
    {
        a->value = calc_bin(t->type, a->value, b->value);
        *st = st_push(*st, a);
        free(b);
    }
    else
    {
        if (a)
            free(a);
        if (b)
            free(b);
        pousse_zero(st);
    }
    free(t);
}

static int recup_res(struct stack_t **st)
{
    int res = 0;
    if (!st_empty(*st))
    {
        struct token *x;
        *st = st_pop(*st, &x);
        if (x && x->type == INT)
            res = x->value;
        if (x)
            free(x);
    }
    return res;
}

static int eval_rpn(struct fifo *q)
{
    struct stack_t *st = NULL;
    while (!fifo_empty(q))
    {
        struct token *t;
        q = fifo_pop(q, &t);
        if (t->type == INT)
        {
            st = st_push(st, t);
            continue;
        }
        if (t->type == UADD || t->type == USUB)
        {
            fait_unaire(&st, t);
            continue;
        }
        fait_binaire(&st, t);
    }
    int r = recup_res(&st);
    st_destroy_tokens(st);
    return r;
}

/*------------FINITO--------*/

int main(int ac, char **av)
{
    int rpn = 0;
    if (ac > 1)
    {
        if (strcmp(av[1], "-rpn") == 0)
            rpn = 1;
    }
    struct fifo *in = lex_stdin(rpn);
    struct fifo *r = in;
    if (!rpn)
        r = shunt(in);
    int res = eval_rpn(r);
    printf("%d\n", res);
    return 0;
}
