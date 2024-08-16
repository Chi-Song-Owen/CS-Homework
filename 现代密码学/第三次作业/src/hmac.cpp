#include "sha256.h"
#include "common.h"
#include "hmac.h"
#define BLOCK_SIZE 64
extern uint8_t  buf[64];
extern uint32_t hashing[8];
extern uint32_t bits[2];
extern uint32_t len;
extern uint32_t rfu__;
extern uint32_t W[64];
// static void print(__uint128_t t) {
// 	uint8_t* ptr = (uint8_t*)(&t);
// 	for (int i; i < 4; ++i, printf(" "))
// 		for (int j; j < 4; ++j)
//             printf("%02x", ptr[((3-i) * 4 + (3-j))]);
// 	puts("");
// }
static void print(uint8_t* chars, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", chars[i]);
    }
    printf("\n");
}
/*
    Output:
        Pointer to tag
        HMAC_OUTPUT_LEN_BYTES = 32
*/
uint8_t* hmac_sha256(const uint8_t* key, size_t key_len_byte, const uint8_t* message, size_t message_len_byte) 
{
    uint8_t *tag = new uint8_t[HMAC_OUTPUT_LEN_BYTES];   
    /* Your code Here! */
    uint8_t key_block[BLOCK_SIZE];
    if (key_len_byte > BLOCK_SIZE) {
        // Hash the key if it's longer than the block size
        int key_len_bits = key_len_byte * 8;
        // std::cout << key_len_byte << std::endl;

        uint8_t* hashed_key = sha_256(key, key_len_bits);

        memcpy(key_block, hashed_key, 32);
        // print(key_block, 32);
        memset(key_block + 32, 0, BLOCK_SIZE - 32);
        // print(key_block, BLOCK_SIZE);
        delete[] hashed_key;
    } else {
        memcpy(key_block, key, key_len_byte);
        memset(key_block + key_len_byte, 0, BLOCK_SIZE - key_len_byte);
    }
    // print(key_block, BLOCK_SIZE);
    // XOR key with ipad and opad values
    uint8_t key_ipad[BLOCK_SIZE];
    uint8_t key_opad[BLOCK_SIZE];
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        key_ipad[i] = key_block[i] ^ 0x36;
        key_opad[i] = key_block[i] ^ 0x5C;
    }
    // print(key_ipad, BLOCK_SIZE);
    // Perform inner SHA-256
    size_t inner_data_len = BLOCK_SIZE + message_len_byte;
    uint8_t* inner_data = new uint8_t[inner_data_len];
    memcpy(inner_data, key_ipad, BLOCK_SIZE);
    memcpy(inner_data + BLOCK_SIZE, message, message_len_byte);
    int inner_data_len_bits = inner_data_len * 8;
    uint8_t* inner_hash = sha_256(inner_data, inner_data_len_bits);
    delete[] inner_data;

    // print(inner_hash, 32);

    size_t outer_data_len = BLOCK_SIZE + 32;
    uint8_t* outer_data = new uint8_t[outer_data_len];
    memcpy(outer_data, key_opad, BLOCK_SIZE);
    memcpy(outer_data + BLOCK_SIZE, inner_hash, 32);
    uint8_t* outer_hash = sha_256(outer_data, outer_data_len * 8);
    delete[] outer_data;

    memcpy(tag, outer_hash, 32);
    delete[] inner_hash;
    delete[] outer_hash;
    // print(tag, HMAC_OUTPUT_LEN_BYTES);
    return tag;
}
