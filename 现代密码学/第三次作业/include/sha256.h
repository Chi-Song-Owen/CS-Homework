#pragma once
#include "common.h"

#ifndef SHA256_H_
#define SHA256_H_

#include <stddef.h>
#include <stdint.h>

#define SHA256_SIZE_BYTES    (32)

typedef struct {
    uint8_t  buf[64];
    uint32_t hash[8];
    uint32_t bits[2];
    uint32_t len;
    uint32_t rfu__;
    uint32_t W[64];
} text;

void main_hash(text *context, uint8_t *hash);

uint8_t* sha_256(const uint8_t *message, const int& m_len_bits);
bool SHA_check_vector();
#endif