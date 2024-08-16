#pragma once
#include "common.h"
#include "message.h"
#include "double-ratchet.h"

class Messenger {
    int mode; // 0 for input from file; 1 for input from stdin;
    int party; // 0 for alice; 1 for bob
    DoubleRatchet ratchet;
public:
    Messenger(const KDF_key SK, const BigUnsigned& bob_dh_pubkey);
    Messenger(const KDF_key SK, const DHKeypair& bob_dh_keypair);
    Message send_message(std::string const& plaintext);
    bool receive_message(Message const& received_message, std::string& plaintext);
};