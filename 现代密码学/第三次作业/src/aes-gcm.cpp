#include <iostream>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "aes-gcm.h"
#include "aes.h"
using namespace std;
extern uint32_t w[44];

extern const unsigned short sbox[256];

extern __uint128_t gf02[256];

extern __uint128_t gf03[256];

extern __uint128_t shift[7];

extern uint8_t rcon[11];
/*
AES_GCM 认证加密函数
plaintext: 明文字节数组. plain_len_bits: 明文比特数.
IV: 初始向量96比特.
AD: 附加认证信息字节数组. AD_len_bits: AD比特数.
key: 128比特密钥
output_ciphertext: 输出密文数组指针地址，长度同密文 output_tag: 输出tag数组指针地址.
ciphertext长度同plaintext, tag长度为128比特(16字节).
*/
static void print(uint8_t* chars, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", chars[i]);
    }
    printf("\n");
}

void GF_Multiply(const uint8_t *X, uint8_t *Y, uint8_t *result) {
    uint64_t Z[2] = {0};
    uint64_t V[2];
    memcpy(V, Y, 16);
    // print(Y, 16);
    // print((uint8_t*)Z, 16);
    // print((uint8_t*)V, 16);
    for (int i = 0; i < 128; i++) {
        if (X[i / 8] & (1 << (7 - (i % 8)))) {
            Z[0] ^= V[0];
            Z[1] ^= V[1];
        }

        uint8_t* V_ptr = (uint8_t*)V;
        bool lsb = V_ptr[15] & 1;

        uint8_t* new_V = (uint8_t*)malloc(16);
        new_V[0] = V_ptr[0] >> 1;
        for (int j = 1; j <= 15; j++) {
            if (V_ptr[j - 1] & 1) {
                new_V[j] = (V_ptr[j] >> 1) + 0b10000000;
            } else {
                new_V[j] = (V_ptr[j] >> 1);
            }
        }
        memcpy(V, new_V, 16);
        // print(new_V, 16);
        free(new_V);
        V_ptr[0] ^= (lsb ? 0b11100001 : 0);
        // if (lsb) {
        //     V[0] ^= 0b1110000100000000000000000000000000000000000000000000000000000000;
        // }
        // if (i < 30) {
        //     cout << i << endl;
        //     cout << lsb << endl;
        //     print((uint8_t*)Z, 16);
        //     print((uint8_t*)V, 16);
        // }
    }

    memcpy(result, Z, 16);
}

void GHASH(uint8_t *H, const uint8_t *X, uint64_t X_len, uint8_t *result) {
    uint8_t Y[16] = {0};

    for (uint64_t i = 0; i < X_len; i += 16) {
        for (int j = 0; j < 16; ++j) {
            Y[j] ^= X[i + j];
        }

        uint8_t tmp[16];
        GF_Multiply(Y, H, tmp);
        memcpy(Y, tmp, 16);
    }

    memcpy(result, Y, 16);
}



void AES_Authenticated_Encryption_GCM(const uint8_t *plaintext, const uint64_t& plain_len_bits, const uint8_t IV[12], const uint8_t *AD, const uint64_t& AD_len_bits, const uint8_t key[16], uint8_t** output_ciphertext, uint8_t** output_tag) {
    /* Your code here */
    // cout << "AES_GCM_Encryption" << endl;

    // for (int i = 0; i < 12; ++i) {
    //     printf("%02x", IV[i]);
    // }
    // cout << endl;
    // for (int i = 0; i < 16; ++i) {
    //     printf("%02x", key[i]);
    // }
    // cout << endl;
    // for (int i = 0; i < plain_len_bits / 8; ++i) {
    //     printf("%02x", plaintext[i]);
    // }
    // cout << endl;
    uint64_t plain_len_bytes = plain_len_bits / 8;
    uint64_t AD_len_bytes = AD_len_bits / 8;

    *output_ciphertext = (uint8_t*)malloc(plain_len_bytes);
    *output_tag = (uint8_t*)malloc(16);

    uint8_t H[16] = {0};
    uint8_t J0[16] = {0};
    memcpy(J0, IV, 12);
    J0[15] = 1;
    uint8_t E_K_Y0[16] = {0};

    AES_CTR_Encrypt(key, E_K_Y0, E_K_Y0, H, 16);
    // print(H, 16);

    AES_CTR_Encrypt(key, J0, E_K_Y0, E_K_Y0, 16);
    // print(E_K_Y0, 16);

    uint8_t J0_Incremented[16];
    memcpy(J0_Incremented, J0, 16);
    J0_Incremented[15] += 1;
    // for (int i = 0; i < 16; ++i) {
    //     printf("%02x", J0_Incremented[i]);
    // }
    // cout << endl;
    // cout << 1 << endl;

    AES_CTR_Encrypt(key, J0_Incremented, plaintext, *output_ciphertext, plain_len_bytes);

    // Calculate GHASH input length
    uint64_t GHASH_input_len = AD_len_bytes + ((AD_len_bytes % 16) ? (16 - AD_len_bytes % 16) : 0) + plain_len_bytes + ((plain_len_bytes % 16) ? (16 - plain_len_bytes % 16) : 0) + 16;
    uint8_t* GHASH_input = (uint8_t*)malloc(GHASH_input_len);
    memset(GHASH_input, 0, GHASH_input_len);

    // Copy AD and pad with zeros to align to 128 bits
    memcpy(GHASH_input, AD, AD_len_bytes);
    // Copy ciphertext and pad with zeros to align to 128 bits
    memcpy(GHASH_input + ((AD_len_bytes + 15) / 16) * 16, *output_ciphertext, plain_len_bytes);

    // Append length of AD and ciphertext in bits
    uint64_t* lengths = (uint64_t*)(GHASH_input + GHASH_input_len - 16);
    lengths[0] = __builtin_bswap64(AD_len_bits); // store lengths in big-endian format
    lengths[1] = __builtin_bswap64(plain_len_bits);

    // Calculate GHASH
    uint8_t S[16];
    // print(GHASH_input, GHASH_input_len);
    // cout << plain_len_bits << endl;
    // cout << GHASH_input_len << endl;
    GHASH(H, GHASH_input, GHASH_input_len, S);
    free(GHASH_input);
    for (int i = 0; i < 16; ++i) {
        (*output_tag)[i] = E_K_Y0[i] ^ S[i];
    }
}

/*
AES_GCM 认证解密
ciphertext：密文字节数组 cipher_len_bits：密文比特数
IV: 初始向量96比特.
AD: 附加认证信息字节数组. AD_len_bits: AD比特数.
key: 128比特密钥
output_plaintext：明文数组，认证成功，长度同密文。如果认证失败，长度为0
Return value: true if authentication success, false if fail
*/
bool AES_Authenticated_Decryption_GCM(const uint8_t *ciphertext, const uint64_t& cipher_len_bits, const uint8_t IV[12], const uint8_t *AD, const uint64_t& AD_len_bits, const uint8_t key[16], uint8_t** output_plaintext, uint8_t* input_tag) {
    uint64_t cipher_len_bytes = cipher_len_bits / 8;
    uint64_t AD_len_bytes = AD_len_bits / 8;

    uint8_t H[16] = {0};
    uint8_t J0[16] = {0};
    memcpy(J0, IV, 12);
    J0[15] = 1;
    uint8_t E_K_Y0[16] = {0};

    AES_CTR_Encrypt(key, E_K_Y0, E_K_Y0, H, 16);
    // print(H, 16);

    AES_CTR_Encrypt(key, J0, E_K_Y0, E_K_Y0, 16);
    // print(E_K_Y0, 16);

    uint8_t J0_Incremented[16];
    memcpy(J0_Incremented, J0, 16);
    J0_Incremented[15] += 1;
    *output_plaintext = (uint8_t*)malloc(cipher_len_bytes);

    // cout << 1 << endl;
    AES_CTR_Encrypt(key, J0_Incremented, ciphertext, *output_plaintext, cipher_len_bytes);

    // cout << 1 << endl;

    // Calculate GHASH input length
    uint64_t GHASH_input_len = AD_len_bytes + ((AD_len_bytes % 16) ? (16 - AD_len_bytes % 16) : 0) + cipher_len_bytes + ((cipher_len_bytes % 16) ? (16 - cipher_len_bytes % 16) : 0) + 16;
    uint8_t* GHASH_input = (uint8_t*)malloc(GHASH_input_len);
    memset(GHASH_input, 0, GHASH_input_len);

    // Copy AD and pad with zeros to align to 128 bits
    memcpy(GHASH_input, AD, AD_len_bytes);
    // Copy ciphertext and pad with zeros to align to 128 bits
    memcpy(GHASH_input + ((AD_len_bytes + 15) / 16) * 16, ciphertext, cipher_len_bytes);

    // Append length of AD and ciphertext in bits
    uint64_t* lengths = (uint64_t*)(GHASH_input + GHASH_input_len - 16);
    lengths[0] = __builtin_bswap64(AD_len_bits); // store lengths in big-endian format
    lengths[1] = __builtin_bswap64(cipher_len_bits);

    uint8_t S[16];
    // print(GHASH_input, GHASH_input_len);
    GHASH(H, GHASH_input, GHASH_input_len, S);
    free(GHASH_input);
    // print(S, 16);
    // AES_CTR_Encrypt(key, J0, S, S, 16);
    // uint8_t calculated_tag[16];
    for (int i = 0; i < 16; ++i) {
        S[i] = E_K_Y0[i] ^ S[i];
    }

    // print(S, 16);
    // print(input_tag, 16);
    if (memcmp(S, input_tag, 16) != 0) {
        // free(*output_plaintext);
        // *output_plaintext = nullptr;
        return false;
    }

    return true;
}