#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comparators.h"

int comp_int(const void *ptr, const void *ptr2)
{
    if(ptr == NULL || ptr2 == NULL)
        return -2;


    const int *a = (const int *)ptr;
    const int *b = (const int *)ptr2;
    if(*a > *b){
        return 1;
    }
    else if(*a == *b)
        return 0;
    return -1;
}

int comp_double(const void *ptr, const void *ptr2)
{
    if(ptr == NULL || ptr2 == NULL){
        return -2;
    }
    const double *a = (const double *)ptr;
    const double *b = (const double *)ptr2;
    if(*a > *b){
        return 1;
    }
    else if(*a == *b)
        return 0;
    return -1;
}


int comp_point(const void *ptr, const void *ptr2)
{
    if(ptr == NULL || ptr2 == NULL)
        return -2;

    const struct point_t *p1 = (const struct point_t*)ptr;
    const struct point_t *p2 = (const struct point_t*)ptr2;

    if(p1->x > p2->x){
        return 1;
    }
    else if(p1->x == p2->x){
        if(p1->y > p2->y){
            return 1;
        }
        else if(p1->y == p2->y){
            return 0;
        }
        else{
            return -1;
        }
    }
    return -1;
}

int comparator(const void *ptr1, const void *ptr2);


int sort(void *tab, int size_tab, size_t size, int (*comparator)(const void *, const void *))
{
    if(tab == NULL || size_tab <= 0 || size == 0 || comparator == NULL)
        return 1;

    char *temp = malloc(size);
    if (temp == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    for(int i=0; i<size_tab; i++){
        for(int j=0; j<size_tab-i-1; j++){
            if(comparator((char*)tab + j * size, (char*)tab + (j+1) * size) > 0){
                memcpy(temp, (char*)tab + j * size, size);
                memcpy((char*)tab + j * size, (char*)tab + (j+1) * size, size);
                memcpy((char*)tab + (j+1) * size, temp, size);
            }
        }
    }

    free(temp);

    return 0;
}
