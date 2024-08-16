#include "double-ratchet.h"
#include "hkdf.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

// For definitions of function, see page 18 of doubleratchet.pdf
DHKeypair DoubleRatchet::GENERATE_DH() {
    return DHKeypair();
}

void DoubleRatchet::DH(const DHKeypair& dh_pair, BigUnsigned dh_pub, DH_output_bytes dh_out) {
    BigUnsigned dh_out_int = diffie_hellman(dh_pub, dh_pair.sk);
    uint8_t *out_buffer = key_to_uint8_t_array(dh_out_int);
    memcpy(dh_out, out_buffer, sizeof(DH_output_bytes));
    delete[] out_buffer;
}

void DoubleRatchet::KDF_RK(KDF_key rk, const DH_output_bytes dh_out, KDF_key chain_key_out) {
    int info_len_bytes;
    uint8_t *info_buffer = parse_string_to_bytes(KDF_RK_info, info_len_bytes);
    uint8_t *buffer = HKDF(rk, sizeof(KDF_key), dh_out, sizeof(DH_output_bytes), info_buffer, info_len_bytes, sizeof(KDF_key) + sizeof(KDF_key));
    memcpy(rk, buffer, sizeof(KDF_key));
    memcpy(chain_key_out, buffer + sizeof(KDF_key), sizeof(KDF_key));
    delete[] info_buffer; delete[] buffer;
}

void DoubleRatchet::KDF_CK(KDF_key ck, MESSAGE_key msg_key_out) {
    int info_len_bytes;
    uint8_t *info_buffer = parse_string_to_bytes(KDF_CK_info, info_len_bytes);
    uint8_t constant_buffer[8];
    for (int i = 0; i < 8; i++) {
        constant_buffer[i] = (CK_CONSTANT >> ((7 - i) * 8)) & 0xff;
    }
    uint8_t *buffer = HKDF(ck, sizeof(KDF_key), constant_buffer, sizeof(constant_buffer), info_buffer, info_len_bytes, sizeof(KDF_key) + sizeof(MESSAGE_key));
    memcpy(ck, buffer, sizeof(KDF_key));
    memcpy(msg_key_out, buffer + sizeof(KDF_key), sizeof(MESSAGE_key));
    delete[] info_buffer; delete[] buffer;
}

DoubleRatchet::DoubleRatchet(const KDF_key SK, const BigUnsigned& bob_dh_pubkey): KDF_RK_info("DoubleRatchet_KDF_RK_info"), KDF_CK_info("DoubleRatchet_KDF_CK_info") {
    DHs = GENERATE_DH();
    DHr = new BigUnsigned(bob_dh_pubkey);
    memcpy(RK, SK, sizeof(KDF_key));
    DH_output_bytes dh_out;
    DH(DHs, *DHr, dh_out);
    KDF_RK(RK, dh_out, CKs);
}

DoubleRatchet::DoubleRatchet(const KDF_key SK, const DHKeypair& bob_dh_keypair): KDF_RK_info("DoubleRatchet_KDF_RK_info"), KDF_CK_info("DoubleRatchet_KDF_CK_info") {
    DHs = bob_dh_keypair;
    DHr = nullptr;
    memcpy(RK, SK, sizeof(KDF_key));
}

DoubleRatchet::~DoubleRatchet() {
    if (DHr != nullptr) delete DHr;
}

DHKeypair DoubleRatchet::get_my_DH_keypair() {
    return DHs;
}

void DoubleRatchet::on_receiving_message(const message_header& msg_header, MESSAGE_key mk) {
    /* Your code here */
    //提示：判断何时应该执行一次DHRatchet
    if (DHr == nullptr || *DHr != msg_header.DH_pub_key) {
        if (DHr != nullptr)
            delete DHr;
        DHr = new BigUnsigned(msg_header.DH_pub_key);
        DH_output_bytes out;
        DH(DHs, *DHr, out);
        KDF_RK(RK, out, CKr);
        DHs = GENERATE_DH();
        DH(DHs, *DHr, out);
        KDF_RK(RK, out, CKs);
    }
    KDF_CK(CKr, mk);
}

void DoubleRatchet::on_sending_message(MESSAGE_key mk) {
    KDF_CK(CKs, mk);
}

void DoubleRatchet::ENCRYPT(uint8_t *plaintext, const size_t& plain_len_bytes, uint8_t *associated_data, const size_t& ad_len_bytes, uint8_t **ciphertext, uint8_t **tag, uint8_t IV[AES_GCM_IV_LEN_BYTES]) {
    MESSAGE_key mk;
    on_sending_message(mk);
    for (int i = 0; i < AES_GCM_IV_LEN_BYTES; i++) IV[i] = rand() & 0xff;
    AES_Authenticated_Encryption_GCM(plaintext, plain_len_bytes * 8, IV, associated_data, ad_len_bytes * 8, mk, ciphertext, tag);
}

/* return true if authentication success; return false if authentication fail */
bool DoubleRatchet::DECRYPT(const message_header& msg_header, uint8_t *ciphertext, const size_t& cipher_len_bytes, uint8_t *associated_data, const size_t& ad_len_bytes, uint8_t tag[AES_GCM_TAG_LEN_BYTES], const uint8_t IV[AES_GCM_IV_LEN_BYTES], uint8_t **plaintext) {
    MESSAGE_key mk;
    on_receiving_message(msg_header, mk);
    bool ret = AES_Authenticated_Decryption_GCM(ciphertext, cipher_len_bytes * 8, IV, associated_data, ad_len_bytes * 8, mk, plaintext, tag);
    if (ret == true) return true;
    return false;
}

message_header DoubleRatchet::HEADER(const DHKeypair& dh_pair) {
    message_header out;
    out.DH_pub_key = dh_pair.pk;
    return out;
}

uint8_t* DoubleRatchet::CONCAT(const message_header& header, size_t& msg_len_bytes) {
    uint8_t *output = key_to_uint8_t_array(header.DH_pub_key);
    msg_len_bytes = sizeof(DH_output_bytes);
    return output;
}