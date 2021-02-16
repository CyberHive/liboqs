#ifndef POLY_H
#define POLY_H
#include "DAGGER_params.h"
#include <immintrin.h>
#include <stdint.h>

typedef union {
    uint16_t coeffs[DAGGER_N];
    __m256i dummy;
} poly;

typedef union {
    uint16_t coeffs[4 * DAGGER_N];
    __m256i dummy;
} toom4_points;

typedef union {
    uint16_t coeffs[8 * DAGGER_N];
    __m256i dummy;
} toom4_points_product;

void PQCLEAN_LIGHTDAGGER_AVX2_MatrixVectorMul(poly c[DAGGER_L], const poly A[DAGGER_L][DAGGER_L], const toom4_points s_eval[DAGGER_L], int transpose);

void PQCLEAN_LIGHTDAGGER_AVX2_InnerProd(poly *c, const poly b[DAGGER_L], const toom4_points s_eval[DAGGER_L]);

void PQCLEAN_LIGHTDAGGER_AVX2_GenMatrix(poly a[DAGGER_L][DAGGER_L], const uint8_t seed[DAGGER_SEEDBYTES]);

void PQCLEAN_LIGHTDAGGER_AVX2_GenSecret(poly s[DAGGER_L], const uint8_t seed[DAGGER_NOISESEEDBYTES]);


void PQCLEAN_LIGHTDAGGER_AVX2_toom4_interp(poly *res_avx, const toom4_points_product *c_eval);

void PQCLEAN_LIGHTDAGGER_AVX2_toom4_eval(toom4_points *b_eval, const poly *b);

void PQCLEAN_LIGHTDAGGER_AVX2_toom4_mul_A_by_B_eval(toom4_points_product *c_eval, const poly *a_avx, const toom4_points *b_eval, int accumulate);


#endif
