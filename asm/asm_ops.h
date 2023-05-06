#include <stdio.h>
#include <stdint.h>

#ifndef ASM_OPS_H
#define ASM_OPS_H

int64_t shlf(int64_t a, int64_t b);
int64_t shrf(int64_t a, int64_t b);

int64_t cmp_less(int64_t a, int64_t b);
int64_t cmp_great(int64_t a, int64_t b);
int64_t cmp_eq(int64_t a, int64_t b);
int64_t cmp_neq(int64_t a, int64_t b);
int64_t cmp_leq(int64_t a, int64_t b);
int64_t cmp_geq(int64_t a, int64_t b);

int64_t* dyn_malloc(int n, int val);
void dyn_set(int64_t* arr, int64_t pos, int64_t val);
int64_t dyn_get(int64_t* arr, int64_t pos);

#endif
