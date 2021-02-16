#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
#include "DAGGER_params.h"
#include "poly.h"
#include <stdint.h>
#include <stdio.h>

void PQCLEAN_DAGGER_CLEAN_POLT2BS(uint8_t bytes[DAGGER_SCALEBYTES_KEM], const poly *data);

void PQCLEAN_DAGGER_CLEAN_BS2POLT(poly *data, const uint8_t bytes[DAGGER_SCALEBYTES_KEM]);


void PQCLEAN_DAGGER_CLEAN_POLVECq2BS(uint8_t bytes[DAGGER_POLYVECBYTES], const poly data[DAGGER_L]);

void PQCLEAN_DAGGER_CLEAN_POLVECp2BS(uint8_t bytes[DAGGER_POLYVECCOMPRESSEDBYTES], const poly data[DAGGER_L]);


void PQCLEAN_DAGGER_CLEAN_BS2POLVECq(poly data[DAGGER_L], const uint8_t bytes[DAGGER_POLYVECBYTES]);

void PQCLEAN_DAGGER_CLEAN_BS2POLVECp(poly data[DAGGER_L], const uint8_t bytes[DAGGER_POLYVECCOMPRESSEDBYTES]);


void PQCLEAN_DAGGER_CLEAN_BS2POLmsg(poly *data, const uint8_t bytes[DAGGER_KEYBYTES]);

void PQCLEAN_DAGGER_CLEAN_POLmsg2BS(uint8_t bytes[DAGGER_KEYBYTES], const poly *data);


#endif
