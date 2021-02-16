#ifndef PQCLEAN_LIGHTDAGGER_AVX2_API_H
#define PQCLEAN_LIGHTDAGGER_AVX2_API_H


#define PQCLEAN_LIGHTDAGGER_AVX2_CRYPTO_ALGNAME "LightDagger"
#define PQCLEAN_LIGHTDAGGER_AVX2_CRYPTO_BYTES 32
#define PQCLEAN_LIGHTDAGGER_AVX2_CRYPTO_CIPHERTEXTBYTES 736
#define PQCLEAN_LIGHTDAGGER_AVX2_CRYPTO_PUBLICKEYBYTES 672
#define PQCLEAN_LIGHTDAGGER_AVX2_CRYPTO_SECRETKEYBYTES 1568

int PQCLEAN_LIGHTDAGGER_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_LIGHTDAGGER_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *k, const unsigned char *pk);

int PQCLEAN_LIGHTDAGGER_AVX2_crypto_kem_dec(unsigned char *k, const unsigned char *ct, const unsigned char *sk);


#endif /* PQCLEAN_LIGHTDAGGER_AVX2_API_H */
