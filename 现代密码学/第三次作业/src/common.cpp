#include "common.h"

uint8_t* parse_string_to_bytes(std::string const& input_str, int& length) {
    uint8_t *byte_arrary = new uint8_t[input_str.length()];
    memcpy(byte_arrary, input_str.c_str(), input_str.length());
    length = input_str.length();
    return byte_arrary;
}

std::string parse_bytes_to_string(const uint8_t* input_bytes, int const& length) {
    std::string result;
    for (int i = 0; i < length; i++) result.append(1, (char)(input_bytes[i]));
    return result;
}

uint8_t hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c-'0';
    else if (c >= 'a' && c <= 'f') return c-'a' + 10;
    else return c-'A'+10;
}

bool is_hex(char c) {
    if (c >= '0' && c <= '9') return 1;
    else if (c >= 'a' && c <= 'f') return 1;
    else if (c >= 'A' && c <= 'F') return 1;
    return 0;
}

uint8_t* process_string_to_hex(std::string const &input_str, size_t &len) {
    std::string res_str = "";
    // remove non-hex characters
    for (char c : input_str) if (is_hex(c)) res_str.push_back(c);
    if (res_str.size() % 2 == 1) {
        std::cerr << "Bad hex string length" << std::endl;
        exit(-1);
    }
    len = res_str.size() / 2;
    uint8_t *res;
    res = (uint8_t*)malloc(len);
    for (int i = 0; i < len; i++) res[i] = (hex_to_int(res_str[2*i])<<4) + hex_to_int(res_str[2*i+1]);
    return res;
}
