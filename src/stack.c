#include "stack.h"

#include <stdlib.h>

struct stack_t *st_push(struct stack_t *s, struct token *t)
{
    struct stack_t *n = malloc(sizeof(*n));
    if (!n)
        return s;
    n->tok = t;
    n->next = s;
    return n;
}

struct stack_t *st_pop(struct stack_t *s, struct token **out)
{
    if (!s)
        return NULL;
    struct stack_t *n = s->next;
    if (out)
        *out = s->tok;
    free(s);
    return n;
}

int st_empty(struct stack_t *s)
{
    return s == NULL;
}

void st_destroy_keep(struct stack_t *s)
{
    while (s)
    {
        struct stack_t *n = s->next;
        free(s);
        s = n;
    }
}

void st_destroy_tokens(struct stack_t *s)
{
    while (s)
    {
        struct stack_t *n = s->next;
        free(s->tok);
        free(s);
        s = n;
    }
}
