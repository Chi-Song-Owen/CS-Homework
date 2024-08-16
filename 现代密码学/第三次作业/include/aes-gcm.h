#pragma once
#include "common.h"

#define AES_GCM_TAG_LEN_BYTES 16
#define AES_GCM_IV_LEN_BYTES 12

void AES_Authenticated_Encryption_GCM(const uint8_t *plaintext, const uint64_t& plain_len_bits, const uint8_t IV[12], const uint8_t *AD, const uint64_t& AD_len_bits, const uint8_t key[16], uint8_t** output_ciphertext, uint8_t** output_tag);

// return true if authentication success. Return false if fail.
// input length checked outside GCM; we don't need to consider bad length
bool AES_Authenticated_Decryption_GCM(const uint8_t *ciphertext, const uint64_t& cipher_len_bits, const uint8_t IV[12], const uint8_t *AD, const uint64_t& AD_len_bits, const uint8_t key[16], uint8_t** output_plaintext, uint8_t* input_tag);

