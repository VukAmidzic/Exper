#include "asm_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int64_t shlf(int64_t a, int64_t b) {return a<<b;};

int64_t shrf(int64_t a, int64_t b) {return a>>b;};

int64_t cmp_less(int64_t a, int64_t b) {return a < b;};

int64_t cmp_great(int64_t a, int64_t b) {return a > b;};

int64_t cmp_eq(int64_t a, int64_t b) {return a == b;};

int64_t cmp_neq(int64_t a, int64_t b) {return a != b;};

int64_t cmp_leq(int64_t a, int64_t b) {return a <= b;};

int64_t cmp_geq(int64_t a, int64_t b) {return a >= b;};

int64_t* dyn_malloc(int n, int val) {
    int64_t* arr = malloc(n * sizeof(int64_t));
    
    for (size_t i = 0; i < n; ++i) {
        arr[i] = val;
    }
    
    return arr;
};

void dyn_set(int64_t* arr, int64_t pos, int64_t val) {arr[pos] = val;};

int64_t dyn_get(int64_t* arr, int64_t pos) {return arr[pos];};
