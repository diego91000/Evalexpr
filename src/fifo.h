#ifndef FIFO_H
#define FIFO_H

#include "token.h"

struct fifo
{
    struct token *tok;
    struct fifo *next;
};

struct fifo *fifo_new(void);
int fifo_empty(struct fifo *q);
struct fifo *fifo_push(struct fifo *q, struct token *t);
struct fifo *fifo_pop(struct fifo *q, struct token **t);
void fifo_destroy_tokens(struct fifo *q);
void fifo_destroy_nodes(struct fifo *q);

#endif /* ! FIFO_H */
