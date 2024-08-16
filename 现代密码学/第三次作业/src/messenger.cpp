#include "messenger.h"
#include <exception>
#include <string>
#include <string.h>

Messenger::Messenger(const KDF_key SK, const BigUnsigned& bob_dh_pubkey): ratchet(SK, bob_dh_pubkey), party(0) {}

Messenger::Messenger(const KDF_key SK, const DHKeypair& bob_dh_keypair): ratchet(SK, bob_dh_keypair), party(1) {}

Message Messenger::send_message(std::string const& plaintext) {
    int plain_len_bytes;
    uint8_t *plain_bytes = parse_string_to_bytes(plaintext, plain_len_bytes);
    uint8_t *ciphertext, *tag;
    uint8_t IV[AES_GCM_IV_LEN_BYTES];
    message_header header = ratchet.HEADER(ratchet.get_my_DH_keypair());
    size_t ad_len_bytes;
    // We use message header as associated data here.
    uint8_t *ad = ratchet.CONCAT(header, ad_len_bytes);
    ratchet.ENCRYPT(plain_bytes, plain_len_bytes, ad, ad_len_bytes, &ciphertext, &tag, IV);
    Message result;
    memcpy(result.IV, IV, sizeof(IV));
    result.header = header;
    result.data = ciphertext;
    result.message_len = plain_len_bytes;
    result.tag = tag;
    result.tag_len = AES_GCM_TAG_LEN_BYTES;
    delete[] ad; delete[] plain_bytes;
    return result;
}

/*
    Input: received_message
    Write decrypted plain text to string plaintext
    Return: false if authentication check fail; true if authentication check success
*/
bool Messenger::receive_message(Message const& received_message, std::string& plaintext) {
    plaintext = "Plaintext of message";
    /* Your code here */
    size_t len_ad;
    uint8_t *ad = ratchet.CONCAT(received_message.header, len_ad);
    uint8_t *plain_text;
    bool success = ratchet.DECRYPT(received_message.header, received_message.data, received_message.message_len, ad, len_ad, received_message.tag, received_message.IV, &plain_text);
    delete[] ad;
    
    if (success) {
        plaintext = std::string(reinterpret_cast<char*>(plain_text), received_message.message_len);
        delete[] plain_text;
        return true;
    }
    return false;
}