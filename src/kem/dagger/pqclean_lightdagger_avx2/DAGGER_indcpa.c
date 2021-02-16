#include "DAGGER_indcpa.h"
#include "DAGGER_params.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "randombytes.h"
#include <stdint.h>
#include <string.h>

#define h1 (1 << (DAGGER_EQ - DAGGER_EP - 1))
#define h2 ((1 << (DAGGER_EP - 2)) - (1 << (DAGGER_EP - DAGGER_ET - 1)) + (1 << (DAGGER_EQ - DAGGER_EP - 1)))

void PQCLEAN_LIGHTDAGGER_AVX2_indcpa_kem_keypair(uint8_t pk[DAGGER_INDCPA_PUBLICKEYBYTES], uint8_t sk[DAGGER_INDCPA_SECRETKEYBYTES]) {
    size_t i, j;

    poly A[DAGGER_L][DAGGER_L];
    poly *skpv1 = A[0]; // use first row of A to hold sk temporarily
    toom4_points skpv1_eval[DAGGER_L];
    poly res[DAGGER_L];

    uint8_t rand[DAGGER_NOISESEEDBYTES];
    uint8_t *seed_A = pk + DAGGER_POLYVECCOMPRESSEDBYTES;

    randombytes(seed_A, DAGGER_SEEDBYTES);
    shake128(seed_A, DAGGER_SEEDBYTES, seed_A, DAGGER_SEEDBYTES); // for not revealing system RNG state

    randombytes(rand, DAGGER_NOISESEEDBYTES);
    PQCLEAN_LIGHTDAGGER_AVX2_GenSecret(skpv1, rand);
    PQCLEAN_LIGHTDAGGER_AVX2_POLVECq2BS(sk, skpv1); // pack secret key

    for (j = 0; j < DAGGER_L; j++) {
        PQCLEAN_LIGHTDAGGER_AVX2_toom4_eval(&skpv1_eval[j], &skpv1[j]);
    }

    PQCLEAN_LIGHTDAGGER_AVX2_GenMatrix(A, seed_A); // sample matrix A
    PQCLEAN_LIGHTDAGGER_AVX2_MatrixVectorMul(res, (const poly (*)[DAGGER_L])A, (const toom4_points *)skpv1_eval, 1); // Matrix in transposed order

    // rounding
    for (i = 0; i < DAGGER_L; i++) {
        for (j = 0; j < DAGGER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= DAGGER_EQ - DAGGER_EP;
            res[i].coeffs[j] &= DAGGER_Q - 1;
        }
    }

    PQCLEAN_LIGHTDAGGER_AVX2_POLVECp2BS(pk, res); // pack public key
}


void PQCLEAN_LIGHTDAGGER_AVX2_indcpa_kem_enc(uint8_t ciphertext[DAGGER_BYTES_CCA_DEC], const uint8_t m[DAGGER_KEYBYTES], const uint8_t noiseseed[DAGGER_NOISESEEDBYTES], const uint8_t pk[DAGGER_INDCPA_PUBLICKEYBYTES]) {
    size_t i, j;

    poly A[DAGGER_L][DAGGER_L];
    poly res[DAGGER_L];
    toom4_points skpv1_eval[DAGGER_L];

    poly *temp = A[0]; // re-use stack space
    poly *vprime = &A[0][0];
    poly *message = &A[0][1];

    const uint8_t *seed_A = pk + DAGGER_POLYVECCOMPRESSEDBYTES;
    uint8_t *msk_c = ciphertext + DAGGER_POLYVECCOMPRESSEDBYTES;

    PQCLEAN_LIGHTDAGGER_AVX2_GenSecret(temp, noiseseed);
    for (j = 0; j < DAGGER_L; j++) {
        PQCLEAN_LIGHTDAGGER_AVX2_toom4_eval(&skpv1_eval[j], &temp[j]);
    }

    PQCLEAN_LIGHTDAGGER_AVX2_GenMatrix(A, seed_A);
    PQCLEAN_LIGHTDAGGER_AVX2_MatrixVectorMul(res, (const poly (*)[DAGGER_L])A, (const toom4_points *)skpv1_eval, 0); // 0 => not transposed

    // rounding
    for (i = 0; i < DAGGER_L; i++) { //shift right EQ-EP bits
        for (j = 0; j < DAGGER_N; j++) {
            res[i].coeffs[j] += h1;
            res[i].coeffs[j] >>= DAGGER_EQ - DAGGER_EP;
            res[i].coeffs[j] &= DAGGER_Q - 1;
        }
    }
    PQCLEAN_LIGHTDAGGER_AVX2_POLVECp2BS(ciphertext, res);

    // vector-vector scalar multiplication with mod p
    PQCLEAN_LIGHTDAGGER_AVX2_BS2POLVECp(temp, pk);
    PQCLEAN_LIGHTDAGGER_AVX2_InnerProd(vprime, temp, skpv1_eval);
    PQCLEAN_LIGHTDAGGER_AVX2_BS2POLmsg(message, m);

    for (i = 0; i < DAGGER_N; i++) {
        vprime->coeffs[i] += h1 - (message->coeffs[i] << (DAGGER_EP - 1));
        vprime->coeffs[i] &= DAGGER_P - 1;
        vprime->coeffs[i] >>= DAGGER_EP - DAGGER_ET;
    }

    PQCLEAN_LIGHTDAGGER_AVX2_POLT2BS(msk_c, vprime);
}


void PQCLEAN_LIGHTDAGGER_AVX2_indcpa_kem_dec(uint8_t m[DAGGER_KEYBYTES], const uint8_t sk[DAGGER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[DAGGER_BYTES_CCA_DEC]) {
    size_t i;

    poly temp[DAGGER_L];
    toom4_points sksv_eval[DAGGER_L];

    const uint8_t *packed_cm = ciphertext + DAGGER_POLYVECCOMPRESSEDBYTES;
    poly *v = &temp[0];
    poly *cm = &temp[1];

    PQCLEAN_LIGHTDAGGER_AVX2_BS2POLVECq(temp, sk);
    for (i = 0; i < DAGGER_L; i++) {
        PQCLEAN_LIGHTDAGGER_AVX2_toom4_eval(&sksv_eval[i], &temp[i]);
    }

    PQCLEAN_LIGHTDAGGER_AVX2_BS2POLVECp(temp, ciphertext);
    PQCLEAN_LIGHTDAGGER_AVX2_InnerProd(v, temp, sksv_eval);

    PQCLEAN_LIGHTDAGGER_AVX2_BS2POLT(cm, packed_cm);

    for (i = 0; i < DAGGER_N; i++) {
        v->coeffs[i] += h2 - (cm->coeffs[i] << (DAGGER_EP - DAGGER_ET));
        v->coeffs[i] &= DAGGER_P - 1;
        v->coeffs[i] >>= DAGGER_EP - 1;
    }

    PQCLEAN_LIGHTDAGGER_AVX2_POLmsg2BS(m, v);
}
