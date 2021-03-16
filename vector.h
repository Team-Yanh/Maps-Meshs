#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

struct vector
{
    size_t capacity;
    size_t size;
    void **data;
};

struct vector *vector_new();

void vector_free(struct vector *v);

void double_capacity(struct vector *v);

void vector_push(struct vector *v,void *x);

int vector_pop(struct vector *v, void **x);

int vector_get(struct vector *v, size_t i, void **x);

void vector_insert(struct vector *v, size_t i, void *x);

int vector_remove(struct vector *v, size_t i, void **x);

#endif
