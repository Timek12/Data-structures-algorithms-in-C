#include <stdio.h>
#include <stdlib.h>
#include "circular_buffer.h"

int circular_buffer_create(struct circular_buffer_t *a, int N)
{
    if(a == NULL || N <= 0)
        return 1;

    a->ptr = (int *) malloc(N * sizeof(int));
    if(a->ptr == NULL)
        return 2;

    a->capacity = N;
    a->begin = 0;
    a->end = 0;
    a->full = 0;
    return 0;
}

int circular_buffer_create_struct(struct circular_buffer_t **cb, int N)
{
    if(cb == NULL || N <= 0 )
        return 1;

    *cb = malloc(sizeof(struct circular_buffer_t));
    if(*cb == NULL)
        return 2;

    (*cb)->ptr = malloc(N * sizeof(int));

    if((*cb)->ptr == NULL) {
        free(*cb);
        return 2;
    }

    (*cb)->capacity = N;
    (*cb)->begin = 0;
    (*cb)->end = 0;
    (*cb)->full = 0;
    return 0;

}

void circular_buffer_destroy(struct circular_buffer_t *a)
{
    if(a == NULL || a->ptr == NULL || a->capacity <= 0 || a->begin > a->end)
        return;
    free(a->ptr);
}

void circular_buffer_destroy_struct(struct circular_buffer_t **a)
{
    if(a == NULL || *a == NULL || (*a)->ptr == NULL)
        return;
    free((*a)->ptr);
    free(*a);
}


int circular_buffer_push_back(struct circular_buffer_t *cb, int value)
{
    if(cb == NULL || cb->ptr == NULL || cb->capacity <= 0 ||
       cb->end < 0 || cb->end >= cb->capacity ||
       cb->begin < 0 || cb->begin >= cb->capacity)
    {
        return 1;
    }

    *(cb->ptr + cb->end) = value;
    cb->end = (cb->end + 1) % cb->capacity;

    if (cb->full)
    {
        cb->begin = (cb->begin + 1) % cb->capacity;
    }

    cb->full = (cb->end == cb->begin);


    return 0;
}


int circular_buffer_pop_front(struct circular_buffer_t *cb, int *err_code) {
    if (cb == NULL || cb->ptr == NULL || cb->capacity <= 0 || cb->begin < 0 || cb->begin > cb->capacity || cb->end < 0 || cb->end > cb->capacity) {
        if(err_code != NULL)
            *err_code = 1;
        return 0;
    }

    if (circular_buffer_empty(cb) == 1) {
        if (err_code != NULL) {
            *err_code = 2;
        }
        return 0;
    }

    int first = *(cb->ptr + cb->begin);
    cb->begin = (cb->begin + 1) % cb->capacity;

    //if(circular_buffer_full(cb) == 1)
    cb->full = 0;

    if (err_code != NULL) {
        *err_code = 0;
    }

    return first;
}



int circular_buffer_pop_back(struct circular_buffer_t *cb, int *err_code)
{
    if (cb == NULL || cb->ptr == NULL || cb->capacity <= 0 || cb->begin < 0 || cb->begin > cb->capacity || cb->end < 0 || cb->end > cb->capacity) {
        if(err_code != NULL) {
            *err_code = 1;
        }
        return 0;
    }


    if (circular_buffer_empty(cb) == 1)
    {
        if(err_code != NULL) {
            *err_code = 2;
        }
        return 0;
    }

    if(cb->end == 0)
        cb->end = cb->capacity - 1;
    else
        cb->end--;

    if(cb->end == cb->capacity)
        cb->end = 0;

    int last = *(cb->ptr + cb->end);
    cb->full = 0;
    if(err_code != NULL) {
        *err_code = 0;
    }

    return last;
}



int circular_buffer_empty(const struct circular_buffer_t *cb)
{
    if (cb == NULL || cb->ptr == NULL || cb->capacity <= 0 || cb->capacity < cb->end || cb->capacity < cb->begin ||
        cb->end < 0 || cb->begin < 0) {
        return -1;
    }
    if(cb->end == cb->begin && cb->full == 0){
        return 1;

    }
    return 0;
}

int circular_buffer_full(const struct circular_buffer_t *cb)
{
    if (cb == NULL || cb->ptr == NULL || cb->capacity <= 0 || cb->capacity < cb->end || cb->capacity < cb->begin ||
        cb->end < 0 || cb->begin < 0) {
        return -1;
    }

    if(cb->full == 1){
        return 1;
    }

    return 0;
}

void circular_buffer_display(const struct circular_buffer_t *cb) {
    if (cb == NULL || cb->ptr == NULL || cb->capacity <= 0 || cb->capacity < cb->end || cb->capacity < cb->begin ||
        cb->end < 0 || cb->begin < 0 || circular_buffer_empty(cb)) {
        return;
    }

    int i = cb->begin;

    do {
        printf("%d ", *(cb->ptr + i));
        i = (i + 1) % cb->capacity;
    } while (i != (cb->end) % cb->capacity);

    printf("\n");
}

