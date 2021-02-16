#ifndef POLY_H
#define POLY_H
#include "DAGGER_params.h"
#include <stdint.h>

typedef union {
    uint16_t coeffs[DAGGER_N];
} poly;


void PQCLEAN_FIREDAGGER_CLEAN_MatrixVectorMul(poly c[DAGGER_L], const poly A[DAGGER_L][DAGGER_L], const poly s[DAGGER_L], int16_t transpose);

void PQCLEAN_FIREDAGGER_CLEAN_InnerProd(poly *c, const poly b[DAGGER_L], const poly s[DAGGER_L]);

void PQCLEAN_FIREDAGGER_CLEAN_GenMatrix(poly A[DAGGER_L][DAGGER_L], const uint8_t seed[DAGGER_SEEDBYTES]);

void PQCLEAN_FIREDAGGER_CLEAN_GenSecret(poly s[DAGGER_L], const uint8_t seed[DAGGER_NOISESEEDBYTES]);


void PQCLEAN_FIREDAGGER_CLEAN_poly_mul(poly *c, const poly *a, const poly *b, int accumulate);


#endif
