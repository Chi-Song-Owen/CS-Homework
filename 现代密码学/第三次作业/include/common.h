#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <stdio.h>

uint8_t* parse_string_to_bytes(std::string const& input_str, int& length);
std::string parse_bytes_to_string(const uint8_t* input_bytes, int const& length);
uint8_t hex_to_int(char c);
bool is_hex(char c);
uint8_t* process_string_to_hex(std::string const &input_str, size_t &len);
std::string byte_to_hex(uint8_t c);