#include "fifo.h"

#include <stdlib.h>

struct fifo *fifo_new(void)
{
    return NULL;
}

int fifo_empty(struct fifo *q)
{
    return q == NULL;
}

struct fifo *fifo_push(struct fifo *q, struct token *t)
{
    struct fifo *n = malloc(sizeof(*n));
    if (!n)
        return q;
    n->tok = t;
    n->next = NULL;
    if (!q)
        return n;
    struct fifo *p = q;
    while (p->next)
        p = p->next;
    p->next = n;
    return q;
}

struct fifo *fifo_pop(struct fifo *q, struct token **t)
{
    if (!q)
        return NULL;
    struct fifo *n = q->next;
    if (t)
        *t = q->tok;
    free(q);
    return n;
}

void fifo_destroy_tokens(struct fifo *q)
{
    while (q)
    {
        struct fifo *n = q->next;
        free(q->tok);
        free(q);
        q = n;
    }
}

void fifo_destroy_nodes(struct fifo *q)
{
    while (q)
    {
        struct fifo *n = q->next;
        free(q);
        q = n;
    }
}
