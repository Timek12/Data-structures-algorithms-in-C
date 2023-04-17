#ifndef ZAD7_8_COMPARATORS_H
#define ZAD7_8_COMPARATORS_H
#include <stdio.h>
struct point_t
{
    double x;
    double y;
};

int comp_int(const void *ptr, const void *ptr2);
int comp_double(const void *ptr, const void *ptr2);
int comp_point(const void *ptr, const void *ptr2);
int sort(void *tab, int size_tab, size_t size, int (*comparator)(const void *, const void *));
#endif //ZAD7_8_COMPARATORS_H
