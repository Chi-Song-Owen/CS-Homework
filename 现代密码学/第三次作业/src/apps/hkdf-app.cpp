#include "common.h"
#include <iostream>
#include <fstream>
#include <string>
#include "hkdf.h"
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
    std::string IKM_str, salt_str, info_str;
    size_t IKM_len, salt_len, info_len;
    int L;
    uint8_t *IKM, *salt, *info, *ret;
    
    std::getline(in, IKM_str);
    std::getline(in, salt_str);
    std::getline(in, info_str);
    in >> L;
    
    IKM = process_string_to_hex(IKM_str, IKM_len);
    salt = process_string_to_hex(salt_str, salt_len);
    info = process_string_to_hex(info_str, info_len);

    clock_t start = clock();
    ret = HKDF(salt, salt_len, IKM, IKM_len, info, info_len, L);
    clock_t end = clock();

    for (int i = 0; i < L; i++)
        out << std::hex << std::setfill('0') << std::setw(2) << (int)ret[i];
    out << std::endl;
    if (((IKM_len + salt_len) + ((L+31)/32) * (info_len)) >= 1024*128) // if larger than 1Mb file, output speed
        std::cout << "Your speed " << ((IKM_len + salt_len) + ((L+31)/32) * (info_len))/(1024*128*(double)(end - start)/CLOCKS_PER_SEC) << " Mbps" << std::endl;
    return 0;
}