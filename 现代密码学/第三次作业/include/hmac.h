#pragma once
#include <stdint.h>
#include <stdlib.h>

#define HMAC_OUTPUT_LEN_BYTES 32

uint8_t* hmac_sha256(const uint8_t* key, size_t key_len_byte, const uint8_t* message, size_t message_len_byte) ;