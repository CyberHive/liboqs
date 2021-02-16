#ifndef INDCPA_H
#define INDCPA_H
#include "DAGGER_params.h"
#include <stdint.h>

void PQCLEAN_DAGGER_CLEAN_indcpa_kem_keypair(uint8_t pk[DAGGER_INDCPA_PUBLICKEYBYTES], uint8_t sk[DAGGER_INDCPA_SECRETKEYBYTES]);

void PQCLEAN_DAGGER_CLEAN_indcpa_kem_enc(uint8_t ciphertext[DAGGER_BYTES_CCA_DEC], const uint8_t m[DAGGER_KEYBYTES], const uint8_t noiseseed[DAGGER_NOISESEEDBYTES], const uint8_t pk[DAGGER_INDCPA_PUBLICKEYBYTES]);

void PQCLEAN_DAGGER_CLEAN_indcpa_kem_dec(uint8_t m[DAGGER_KEYBYTES], const uint8_t sk[DAGGER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[DAGGER_BYTES_CCA_DEC]);


#endif
