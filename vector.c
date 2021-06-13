#include <err.h>
#include <stdio.h>
#include "vector.h"

struct vector *vector_new(size_t size)
{
    struct vector *newvec = malloc(sizeof(struct vector));
    if(!newvec)
        errx(1,"Not enough memory");
    newvec->data = calloc(1,sizeof(void *) * size);
    if(!(newvec->data))
        errx(1,"Not enough memory!");
    newvec->capacity = size;
    newvec->size = 0;
    return newvec;
}

void vector_free(struct vector *v)
{
    for(size_t i = 0 ; i < v->size;i ++)
    {
        void *p = v->data[i];
        free(p);
    }
    free(v->data);
    free(v);
}

static void double_capacity(struct vector *v)
{
    v->capacity *= 2;
    v->data = realloc(v->data,v->capacity * sizeof(void *));
}

void vector_push(struct vector *v, void *x)
{
    v->size ++;
    if(v->size == v->capacity)
        double_capacity(v);
    v->data[v->size - 1] = x;
}

int vector_pop(struct vector *v, void **x)
{
    if(v->size == 0)
        return 0;
    v->size --;
    *x = v->data[v->size];
    v->data[v->size]= 0;
    return 1;
}

int vector_get(struct vector *v, size_t i, void **x)
{
    if (i > v->size){
        return 0;
    }
    *x = v->data[i - 1];
    return 1; 
}

void vector_insert(struct vector *v, size_t i, void *x)
{
    if( i > v->size){
        return ;
    }
    v->size ++;
    if (v->size > v->capacity)
        double_capacity(v);
    void *temp;
    for(size_t j = i - 1; j < v->size;j ++){
        temp = v->data[j];
        v->data[j] = x;
        x = temp;
    }
}

int vector_remove(struct vector *v, size_t i, void **x)
{
    if( i > v->size){
        return 0;
    }
    v->size --;
    *x = v->data[i-1];
    for(size_t j = i - 1; j < v->size; j++){
        v->data[j] = v->data[j+1];
    }
    return 1;
}
