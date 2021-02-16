// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_DAGGER_H
#define OQS_KEM_DAGGER_H

#include <oqs/oqs.h>

#ifdef OQS_ENABLE_KEM_dagger_lightdagger
#define OQS_KEM_dagger_lightdagger_length_public_key 672
#define OQS_KEM_dagger_lightdagger_length_secret_key 1568
#define OQS_KEM_dagger_lightdagger_length_ciphertext 736
#define OQS_KEM_dagger_lightdagger_length_shared_secret 32
OQS_KEM *OQS_KEM_dagger_lightdagger_new(void);
OQS_API OQS_STATUS OQS_KEM_dagger_lightdagger_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_dagger_lightdagger_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_dagger_lightdagger_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_dagger_dagger
#define OQS_KEM_dagger_dagger_length_public_key 992
#define OQS_KEM_dagger_dagger_length_secret_key 2304
#define OQS_KEM_dagger_dagger_length_ciphertext 1088
#define OQS_KEM_dagger_dagger_length_shared_secret 32
OQS_KEM *OQS_KEM_dagger_dagger_new(void);
OQS_API OQS_STATUS OQS_KEM_dagger_dagger_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_dagger_dagger_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_dagger_dagger_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#ifdef OQS_ENABLE_KEM_dagger_firedagger
#define OQS_KEM_dagger_firedagger_length_public_key 1312
#define OQS_KEM_dagger_firedagger_length_secret_key 3040
#define OQS_KEM_dagger_firedagger_length_ciphertext 1472
#define OQS_KEM_dagger_firedagger_length_shared_secret 32
OQS_KEM *OQS_KEM_dagger_firedagger_new(void);
OQS_API OQS_STATUS OQS_KEM_dagger_firedagger_keypair(uint8_t *public_key, uint8_t *secret_key);
OQS_API OQS_STATUS OQS_KEM_dagger_firedagger_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);
OQS_API OQS_STATUS OQS_KEM_dagger_firedagger_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
#endif

#endif

