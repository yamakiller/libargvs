#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct argvs_array *construct_array(unsigned int cap) {
    struct argvs_array *parr = (struct argvs_array*)malloc(sizeof(struct argvs_array));
    assert(parr);
    parr->size = 0;
    parr->cap  = cap <  8 ? 8 : cap;

    parr->array = malloc(sizeof(void*) * parr->cap);
    assert(parr->array);

    return parr;
}

void  init_array(struct argvs_array* ar, unsigned int cap) {
    ar->size = 0;
    ar->cap  = cap <  8 ? 8 : cap;
    ar->array = malloc(sizeof(void*) * ar->cap);
    assert(ar->array);
}

void destory_array(struct argvs_array *p) {
    if (p == NULL) { return; }
    if (p->array) {
        free(p->array);
        p->array = NULL;
    }

    free(p);
    p = NULL;
}

void append_array(struct argvs_array *p, void *data) {
    if (p->size >= p->cap) {
        unsigned int new_cap = p->cap << 1;
        void **new_array = realloc(p->array, sizeof(void*) * new_cap);
        if (new_array != p->array) {
            memcpy(new_array, p->array, sizeof(void*) * p->size);
            p->array = new_array;
        }
    }

    p->array[p->size] = data;
    p->size++;
}

void remove_array_index(struct argvs_array *p, int index) {
    if (index >= p->size) { return; }
    p->array[index] = NULL;
    if (index == 0 && p->size > 1) {
        memmove(p->array[0], p->array[1], sizeof(void*) * (p->size - 1));
    } else if (index > 0 && p->size > 2) {
        memmove(p->array[index], p->array[index + 1], sizeof(void*) * (p->size - index - 1));
    }
    p->size--;
}

void* get_array_index(const struct  argvs_array *p, int index) {
    if (index >= p->size) { return NULL; }
    return p->array[index];
}

void  set_array_index(struct argvs_array *p, int index, void *data) {
    if (index >= p->size) { return; }
    p->array[index] = data;
}

int   get_array_size(const struct  argvs_array *p) {
    return (int)p->size;
}

int   get_array_cap(const struct  argvs_array *p) {
    return (int)p->cap;
}