#ifndef PQCLEAN_DAGGER_AVX2_API_H
#define PQCLEAN_DAGGER_AVX2_API_H


#define PQCLEAN_DAGGER_AVX2_CRYPTO_ALGNAME "Dagger"
#define PQCLEAN_DAGGER_AVX2_CRYPTO_BYTES 32
#define PQCLEAN_DAGGER_AVX2_CRYPTO_CIPHERTEXTBYTES 1088
#define PQCLEAN_DAGGER_AVX2_CRYPTO_PUBLICKEYBYTES 992
#define PQCLEAN_DAGGER_AVX2_CRYPTO_SECRETKEYBYTES 2304

int PQCLEAN_DAGGER_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_DAGGER_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *k, const unsigned char *pk);

int PQCLEAN_DAGGER_AVX2_crypto_kem_dec(unsigned char *k, const unsigned char *ct, const unsigned char *sk);


#endif /* PQCLEAN_DAGGER_AVX2_API_H */
