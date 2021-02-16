#include "DAGGER_indcpa.h"
#include "DAGGER_params.h"
#include "api.h"
#include "fips202.h"
#include "randombytes.h"
#include "verify.h"
#include <stddef.h>
#include <stdint.h>


int PQCLEAN_FIREDAGGER_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    size_t i;

    PQCLEAN_FIREDAGGER_CLEAN_indcpa_kem_keypair(pk, sk); // sk[0:DAGGER_INDCPA_SECRETKEYBYTES-1] <-- sk
    for (i = 0; i < DAGGER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + DAGGER_INDCPA_SECRETKEYBYTES] = pk[i];    // sk[DAGGER_INDCPA_SECRETKEYBYTES:DAGGER_INDCPA_SECRETKEYBYTES+DAGGER_INDCPA_SECRETKEYBYTES-1] <-- pk
    }

    sha3_256(sk + DAGGER_SECRETKEYBYTES - 64, pk, DAGGER_INDCPA_PUBLICKEYBYTES); // Then hash(pk) is appended.

    randombytes(sk + DAGGER_SECRETKEYBYTES - DAGGER_KEYBYTES, DAGGER_KEYBYTES); // Remaining part of sk contains a pseudo-random number.
    // This is output when check in PQCLEAN_FIREDAGGER_CLEAN_crypto_kem_dec() fails.
    return (0);
}

int PQCLEAN_FIREDAGGER_CLEAN_crypto_kem_enc(uint8_t *c, uint8_t *k, const uint8_t *pk) {

    uint8_t kr[64]; // Will contain key, coins
    uint8_t buf[64];

    randombytes(buf, 32);

    sha3_256(buf, buf, 32); // BUF[0:31] <-- random message (will be used as the key for client) Note: hash doesnot release system RNG output

    sha3_256(buf + 32, pk, DAGGER_INDCPA_PUBLICKEYBYTES); // BUF[32:63] <-- Hash(public key);  Multitarget countermeasure for coins + contributory KEM

    sha3_512(kr, buf, 64);               // kr[0:63] <-- Hash(buf[0:63]);
    // K^ <-- kr[0:31]
    // noiseseed (r) <-- kr[32:63];
    PQCLEAN_FIREDAGGER_CLEAN_indcpa_kem_enc(c, buf, kr + 32, pk); // buf[0:31] contains message; kr[32:63] contains randomness r;

    sha3_256(kr + 32, c, DAGGER_BYTES_CCA_DEC);

    sha3_256(k, kr, 64); // hash concatenation of pre-k and h(c) to k

    return (0);
}

int PQCLEAN_FIREDAGGER_CLEAN_crypto_kem_dec(uint8_t *k, const uint8_t *c, const uint8_t *sk) {
    size_t i;
    uint8_t fail;
    uint8_t cmp[DAGGER_BYTES_CCA_DEC];
    uint8_t buf[64];
    uint8_t kr[64]; // Will contain key, coins
    const uint8_t *pk = sk + DAGGER_INDCPA_SECRETKEYBYTES;

    PQCLEAN_FIREDAGGER_CLEAN_indcpa_kem_dec(buf, sk, c); // buf[0:31] <-- message

    // Multitarget countermeasure for coins + contributory KEM
    for (i = 0; i < 32; i++) { // Save hash by storing h(pk) in sk
        buf[32 + i] = sk[DAGGER_SECRETKEYBYTES - 64 + i];
    }

    sha3_512(kr, buf, 64);

    PQCLEAN_FIREDAGGER_CLEAN_indcpa_kem_enc(cmp, buf, kr + 32, pk);

    fail = PQCLEAN_FIREDAGGER_CLEAN_verify(c, cmp, DAGGER_BYTES_CCA_DEC);

    sha3_256(kr + 32, c, DAGGER_BYTES_CCA_DEC); // overwrite coins in kr with h(c)

    PQCLEAN_FIREDAGGER_CLEAN_cmov(kr, sk + DAGGER_SECRETKEYBYTES - DAGGER_KEYBYTES, DAGGER_KEYBYTES, fail);

    sha3_256(k, kr, 64); // hash concatenation of pre-k and h(c) to k

    return (0);
}
