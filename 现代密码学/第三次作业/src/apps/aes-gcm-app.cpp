// This file compiles into aes-gcm-test

#include "common.h"
#include "aes-gcm.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Bad argument!" << std::endl;
        return -1;
    }
    std::ifstream in(argv[1], std::ios::in);
    if (in.fail()) {
        std::cerr << "Input file read fail!" << std::endl;
        return -1;
    }
    std::ofstream out(argv[2], std::ios::out);
    if (out.fail()) {
        std::cerr << "Output file open fail!" << std::endl;
        return -1;
    }  

    int mode;
    bool valid; // if decryption tag is valid
    uint8_t *key, *message, *IV, *AD, *ciphertext, *tag;
    size_t key_len, message_len, IV_len, AD_len, tag_len;
    std::string key_str, IV_str, message_str, AD_str, mode_str, ciphertext_str, tag_str; 

    std::getline(in, mode_str);
    if (mode_str[0] == '0') mode = 0; //Encrypt
    else if (mode_str[0] == '1') mode = 1; // Decrypt
    else { std::cerr << "Bad mode input" << std::endl; return -1; }

    std::getline(in, key_str);
    key = process_string_to_hex(key_str, key_len);
    if (key_len != 16) {
        std::cerr << "Bad key input! keylen: " << key_len << std::endl;
        return -1;
    }

    std::getline(in, IV_str);
    IV = process_string_to_hex(IV_str, IV_len);
    if (IV_len != 12) {
        std::cerr << "Bad IV input!" << std::endl;
        return -1;
    }

    if (mode == 0) {
        std::getline(in, message_str);
        message = process_string_to_hex(message_str, message_len);
    }
    else {
        std::getline(in, ciphertext_str);
        ciphertext = process_string_to_hex(ciphertext_str, message_len);
    }
   
    std::getline(in, AD_str);
    AD = process_string_to_hex(AD_str, AD_len);

    if (mode == 1) {
        std::getline(in, tag_str);
        tag = process_string_to_hex(tag_str, tag_len);
    }

    clock_t start, end;
    start = clock();
    if (mode == 0)
        AES_Authenticated_Encryption_GCM(message, message_len*8, IV, AD, AD_len*8, key, &ciphertext, &tag);
    else
        valid = AES_Authenticated_Decryption_GCM(ciphertext, message_len*8, IV, AD, AD_len*8, key, &message, tag);
    end = clock();
    
    if (mode == 0) {
        for (int i = 0; i < message_len; i++)
            out << std::hex << std::setfill('0') << std::setw(2) << (int)ciphertext[i];
        out << std::endl;
        for (int i = 0; i < 16; i++)
            out << std::hex << std::setfill('0') << std::setw(2) << (int)tag[i];
        out << std::endl;
    }
    else {
        for (int i = 0; i < message_len; i++)
            out << std::hex << std::setfill('0') << std::setw(2) << (int)message[i];
        out << std::endl;
        out << valid << std::endl;
    }
    if (AD_len + message_len >= 1024*128) // if larger than 1Mb file, output speed
        std::cout << "Your speed: " << (message_len+AD_len)/(1024*128*(double)(end - start)/CLOCKS_PER_SEC) << " Mbps" << std::endl;
    return 0;
}