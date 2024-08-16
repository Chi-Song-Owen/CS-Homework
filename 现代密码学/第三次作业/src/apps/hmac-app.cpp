#include "common.h"
#include <iostream>
#include <fstream>
#include <string>
#include "hmac.h"
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
    std::string key_str, message_str;
    size_t key_len, message_len;
    uint8_t *key, *message, *tag;
    std::getline(in, key_str);
    std::getline(in, message_str);
    
    key = process_string_to_hex(key_str, key_len);
    message = process_string_to_hex(message_str, message_len);

    clock_t start = clock();
    tag = hmac_sha256(key, key_len, message, message_len);
    clock_t end = clock();

    for (int i = 0; i < 32; i++)
        out << std::hex << std::setfill('0') << std::setw(2) << (int)tag[i];
    out << std::endl;
    if (message_len + key_len >= 1024*128) // if larger than 1Mb file, output speed
        std::cout << "Your speed " << (message_len+key_len)/(1024*128*(double)(end - start)/CLOCKS_PER_SEC) << " Mbps" << std::endl;
    return 0;
}