#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"


void PQCLEAN_FIREDAGGER_AVX2_MatrixVectorMul(poly c[DAGGER_L], const poly A[DAGGER_L][DAGGER_L], const toom4_points s_eval[DAGGER_L], int transpose) {
    size_t i, j;
    toom4_points_product c_eval;

    if (transpose) {
        for (i = 0; i < DAGGER_L; i++) {
            PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &A[0][i], &s_eval[0], 0);
            for (j = 1; j < DAGGER_L; j++) {
                PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &A[j][i], &s_eval[j], 1);
            }
            PQCLEAN_FIREDAGGER_AVX2_toom4_interp(&c[i], &c_eval);
        }
    } else {
        for (i = 0; i < DAGGER_L; i++) {
            PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &A[i][0], &s_eval[0], 0);
            for (j = 1; j < DAGGER_L; j++) {
                PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &A[i][j], &s_eval[j], 1);
            }
            PQCLEAN_FIREDAGGER_AVX2_toom4_interp(&c[i], &c_eval);
        }
    }
}

void PQCLEAN_FIREDAGGER_AVX2_InnerProd(poly *c, const poly b[DAGGER_L], const toom4_points s_eval[DAGGER_L]) {
    size_t i;
    toom4_points_product c_eval; //Holds results for 9 Karatsuba at a time

    PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &b[0], &s_eval[0], 0);
    for (i = 1; i < DAGGER_L; i++) {
        PQCLEAN_FIREDAGGER_AVX2_toom4_mul_A_by_B_eval(&c_eval, &b[i], &s_eval[i], 1);
    }

    PQCLEAN_FIREDAGGER_AVX2_toom4_interp(c, &c_eval);
}

void PQCLEAN_FIREDAGGER_AVX2_GenMatrix(poly A[DAGGER_L][DAGGER_L], const uint8_t seed[DAGGER_SEEDBYTES]) {
    size_t i;
    uint8_t buf[DAGGER_L * DAGGER_POLYVECBYTES];

    shake128(buf, sizeof(buf), seed, DAGGER_SEEDBYTES);

    for (i = 0; i < DAGGER_L; i++) {
        PQCLEAN_FIREDAGGER_AVX2_BS2POLVECq(A[i], buf + i * DAGGER_POLYVECBYTES);
    }
}

void PQCLEAN_FIREDAGGER_AVX2_GenSecret(poly s[DAGGER_L], const uint8_t seed[DAGGER_NOISESEEDBYTES]) {
    size_t i;
    uint8_t buf[DAGGER_L * DAGGER_POLYCOINBYTES];

    shake128(buf, sizeof(buf), seed, DAGGER_NOISESEEDBYTES);

    for (i = 0; i < DAGGER_L; i++) {
        PQCLEAN_FIREDAGGER_AVX2_cbd(s[i].coeffs, buf + i * DAGGER_POLYCOINBYTES);
    }
}
