#pragma once
#include "bigint/BigIntegerLibrary.hh"
#include "message.h"
#include "diffie-hellman.h"
#include "common.h"
#include "aes-gcm.h"
#include <stdint.h>

typedef uint8_t KDF_key[32];
typedef uint8_t MESSAGE_key[16];
typedef uint8_t DH_output_bytes[256];

#define CK_CONSTANT 0x2333ull

class DoubleRatchet {
private:
    const std::string KDF_RK_info;
    const std::string KDF_CK_info;
    DHKeypair DHs;
    // DHR is a pointer to the received dh pubkey. If no dh pubkey has been received yet, set DHr to nullptr.
    BigUnsigned *DHr; 
    KDF_key RK;
    KDF_key CKs, CKr;
    DHKeypair GENERATE_DH();
    void DH(const DHKeypair& dh_pair, BigUnsigned dh_pub, DH_output_bytes dh_out);
    void KDF_RK(KDF_key rk, const DH_output_bytes dh_out, KDF_key chain_key_out);
    void KDF_CK(KDF_key ck, MESSAGE_key msg_key_out);
public:
    ~DoubleRatchet();
    DoubleRatchet(const KDF_key SK, const BigUnsigned& bob_dh_pubkey);
    DoubleRatchet(const KDF_key SK, const DHKeypair& bob_dh_keypair);
    DHKeypair get_my_DH_keypair();
    void on_receiving_message(const message_header& msg_header, MESSAGE_key mk);
    void on_sending_message(MESSAGE_key mk);
    void ENCRYPT(uint8_t *plaintext, const size_t& plain_len_bytes, uint8_t *associated_data, const size_t& ad_len_bytes, uint8_t **ciphertext, uint8_t **tag, uint8_t IV[AES_GCM_IV_LEN_BYTES]);
    bool DECRYPT(const message_header& msg_header, uint8_t *ciphertext, const size_t& cipher_len_bytes, uint8_t *associated_data, const size_t& ad_len_bytes, uint8_t tag[AES_GCM_TAG_LEN_BYTES], const uint8_t IV[AES_GCM_IV_LEN_BYTES], uint8_t **plaintext);
    message_header HEADER(const DHKeypair& dh_pair);
    uint8_t *CONCAT(const message_header& header, size_t& msg_len_bytes);
};