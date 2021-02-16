#include "api.h"
#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include <stddef.h>

void PQCLEAN_LIGHTDAGGER_CLEAN_MatrixVectorMul(poly c[DAGGER_L], const poly A[DAGGER_L][DAGGER_L], const poly s[DAGGER_L], int16_t transpose) {
    size_t i, j;

    if (transpose) {
        for (i = 0; i < DAGGER_L; i++) {
            PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(&c[i], &A[0][i], &s[0], 0);
            for (j = 1; j < DAGGER_L; j++) {
                PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(&c[i], &A[j][i], &s[j], 1);
            }
        }
    } else {
        for (i = 0; i < DAGGER_L; i++) {
            PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(&c[i], &A[i][0], &s[0], 0);
            for (j = 1; j < DAGGER_L; j++) {
                PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(&c[i], &A[i][j], &s[j], 1);
            }
        }
    }
}

void PQCLEAN_LIGHTDAGGER_CLEAN_InnerProd(poly *c, const poly b[DAGGER_L], const poly s[DAGGER_L]) {
    size_t i;

    PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(c, &b[0], &s[0], 0);
    for (i = 1; i < DAGGER_L; i++) {
        PQCLEAN_LIGHTDAGGER_CLEAN_poly_mul(c, &b[i], &s[i], 1);
    }
}

void PQCLEAN_LIGHTDAGGER_CLEAN_GenMatrix(poly A[DAGGER_L][DAGGER_L], const uint8_t seed[DAGGER_SEEDBYTES]) {
    size_t i;
    uint8_t buf[DAGGER_L * DAGGER_POLYVECBYTES];

    shake128(buf, sizeof(buf), seed, DAGGER_SEEDBYTES);

    for (i = 0; i < DAGGER_L; i++) {
        PQCLEAN_LIGHTDAGGER_CLEAN_BS2POLVECq(A[i], buf + i * DAGGER_POLYVECBYTES);
    }
}

void PQCLEAN_LIGHTDAGGER_CLEAN_GenSecret(poly s[DAGGER_L], const uint8_t seed[DAGGER_NOISESEEDBYTES]) {
    size_t i;
    uint8_t buf[DAGGER_L * DAGGER_POLYCOINBYTES];

    shake128(buf, sizeof(buf), seed, DAGGER_NOISESEEDBYTES);

    for (i = 0; i < DAGGER_L; i++) {
        PQCLEAN_LIGHTDAGGER_CLEAN_cbd(s[i].coeffs, buf + i * DAGGER_POLYCOINBYTES);
    }
}
