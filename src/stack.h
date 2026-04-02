#ifndef STACK_H
#define STACK_H

#include "token.h"

struct stack_t
{
    struct token *tok;
    struct stack_t *next;
};

struct stack_t *st_push(struct stack_t *s, struct token *t);
struct stack_t *st_pop(struct stack_t *s, struct token **out);
int st_empty(struct stack_t *s);
void st_destroy_keep(struct stack_t *s);
void st_destroy_tokens(struct stack_t *s);

#endif /* ! STACK_H */
