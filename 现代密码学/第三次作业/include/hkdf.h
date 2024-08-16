#pragma once
#include "common.h"

uint8_t *HKDF(const uint8_t *salt, const size_t& salt_len_bytes, const uint8_t *IKM, const size_t& IKM_len_bytes, const uint8_t *info, const size_t& info_len_bytes, const size_t& L_bytes);