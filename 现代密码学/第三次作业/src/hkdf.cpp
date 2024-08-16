#include "hmac.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/* return prk */
static uint8_t* HKDF_Extract(const uint8_t *salt, const size_t& salt_len_bytes, const uint8_t *IKM, const size_t& IKM_len_bytes) {
    /* Your code here */
    uint8_t default_salt[32] = {0};
    const uint8_t *actual_salt = salt ? salt : default_salt;
    size_t actual_salt_len = salt ? salt_len_bytes : 32;
    uint8_t* prk = hmac_sha256(actual_salt, actual_salt_len, IKM, IKM_len_bytes);
    return prk;
}

/* return: array of L bytes */
static uint8_t* HKDF_Expand(const uint8_t *prk, const size_t& prk_len_bytes, const uint8_t *info, const size_t& info_len_bytes, const size_t& L_bytes) {
    /* Your code here */
    int number = (L_bytes + 31) / 32;
    uint8_t *okm = (uint8_t*)malloc(L_bytes);
    if (okm == nullptr) {
        return nullptr;
    }
    uint8_t T[32];
    uint8_t previous[32];
    int len_t = 0;
    int offset_okm = 0;
    
    for (int i = 0; i < number; i++) {
        int number_con = len_t + info_len_bytes + 1;
        uint8_t *data_con = (uint8_t*)malloc(number_con);
        if (data_con == nullptr) {
            free(okm);
            return nullptr;
        }
        memcpy(data_con, previous, len_t);
        memcpy(data_con + len_t, info, info_len_bytes);
        data_con[number_con - 1] = (uint8_t)(i + 1);
        uint8_t *new_t = hmac_sha256(prk, prk_len_bytes, data_con, number_con);
        memcpy(T, new_t, 32);
        memcpy(previous, T, 32);
        free(new_t);
        free(data_con);
        
        len_t = 32;
        size_t copy_len = (offset_okm + 32 <= L_bytes) ? 32 : (L_bytes - offset_okm);
        memcpy(okm + offset_okm, T, copy_len);
        offset_okm += copy_len;
    }
    return okm;
}

/*
    Return: pointer to okm
*/
uint8_t *HKDF(const uint8_t *salt, const size_t& salt_len_bytes, const uint8_t *IKM, const size_t& IKM_len_bytes, const uint8_t *info, const size_t& info_len_bytes, const size_t& L_bytes) {
    /* Your code here */
    uint8_t *prk = HKDF_Extract(salt, salt_len_bytes, IKM, IKM_len_bytes);
    if (prk == nullptr) {
        return nullptr;
    }
    else {
        uint8_t *okm = HKDF_Expand(prk, 32, info, info_len_bytes, L_bytes);
        free(prk);
        return okm;
    }
}
