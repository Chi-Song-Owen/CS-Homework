#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <string>
#include <stdint.h>
#include <iostream>
#include <string>
using namespace std;

#define rotateLeft(x, y) ((x << y) | (x >> (64 - y)))
#define swap(x) \
    ( \
        ((x) >> 56) | \
        (((x) >> 40) & 0x000000000000FF00ULL) | \
        (((x) >> 24) & 0x0000000000FF0000ULL) | \
        (((x) >>  8) & 0x00000000FF000000ULL) | \
        (((x) <<  8) & 0x000000FF00000000ULL) | \
        (((x) << 24) & 0x0000FF0000000000ULL) | \
        (((x) << 40) & 0x00FF000000000000ULL) | \
        ((x) << 56) \
    )
#define mod(x) (((x) < 5) ? (x) : ((x) - 5))

uint64_t hashing[100] = {0};
uint8_t buf[144] = {0};
uint64_t _num = 0;
int sizeof_block = 0;
int sizeof_buffer = 0;

const uint64_t XorMasks[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

void theta(uint64_t* coefficients) {
    for (unsigned i = 0; i < 5; i++) {
        coefficients[i] = hashing[i] ^ hashing[i + 5] ^ hashing[i + 10] ^ hashing[i + 15] ^ hashing[i + 20];
    }
    for (unsigned i = 0; i < 5; i++) {
        uint64_t one = coefficients[mod(i + 4)] ^ rotateLeft(coefficients[mod(i + 1)], 1);
        hashing[i     ] ^= one;
        hashing[i +  5] ^= one;
        hashing[i + 10] ^= one;
        hashing[i + 15] ^= one;
        hashing[i + 20] ^= one;
    }
}

void rho_pi(uint64_t last, uint64_t one) {
    one = hashing[10]; hashing[10] = rotateLeft(last,  1); last = one;
    one = hashing[ 7]; hashing[ 7] = rotateLeft(last,  3); last = one;
    one = hashing[11]; hashing[11] = rotateLeft(last,  6); last = one;
    one = hashing[17]; hashing[17] = rotateLeft(last, 10); last = one;
    one = hashing[18]; hashing[18] = rotateLeft(last, 15); last = one;
    one = hashing[ 3]; hashing[ 3] = rotateLeft(last, 21); last = one;
    one = hashing[ 5]; hashing[ 5] = rotateLeft(last, 28); last = one;
    one = hashing[16]; hashing[16] = rotateLeft(last, 36); last = one;
    one = hashing[ 8]; hashing[ 8] = rotateLeft(last, 45); last = one;
    one = hashing[21]; hashing[21] = rotateLeft(last, 55); last = one;
    one = hashing[24]; hashing[24] = rotateLeft(last,  2); last = one;
    one = hashing[ 4]; hashing[ 4] = rotateLeft(last, 14); last = one;
    one = hashing[15]; hashing[15] = rotateLeft(last, 27); last = one;
    one = hashing[23]; hashing[23] = rotateLeft(last, 41); last = one;
    one = hashing[19]; hashing[19] = rotateLeft(last, 56); last = one;
    one = hashing[13]; hashing[13] = rotateLeft(last,  8); last = one;
    one = hashing[12]; hashing[12] = rotateLeft(last, 25); last = one;
    one = hashing[ 2]; hashing[ 2] = rotateLeft(last, 43); last = one;
    one = hashing[20]; hashing[20] = rotateLeft(last, 62); last = one;
    one = hashing[14]; hashing[14] = rotateLeft(last, 18); last = one;
    one = hashing[22]; hashing[22] = rotateLeft(last, 39); last = one;
    one = hashing[ 9]; hashing[ 9] = rotateLeft(last, 61); last = one;
    one = hashing[ 6]; hashing[ 6] = rotateLeft(last, 20); last = one;
    hashing[ 1] = rotateLeft(last, 44);
}

void chi(int j, uint64_t one, uint64_t two) {
    hashing[j] ^= hashing[j + 2] & ~two;
    hashing[j + 1] ^= hashing[j + 3] & ~hashing[j + 2];
    hashing[j + 2] ^= hashing[j + 4] & ~hashing[j + 3];
    hashing[j + 3] ^= one & ~hashing[j + 4];
    hashing[j + 4] ^= two & ~one;
}

void iota(int i) {
    hashing[0] ^= XorMasks[i];
}

void block_process(const void* data) {
    const uint64_t* data64 = (const uint64_t*)data;
    for (unsigned i = 0; i < sizeof_block / 8; i++) {
        hashing[i] ^= data64[i];
    }
    for (unsigned round = 0; round < 24; round++) {
        uint64_t coefficients[5];
        theta(coefficients);
        uint64_t one;
        uint64_t last = hashing[1];
        rho_pi(last, one);
        for (unsigned j = 0; j < 100; j += 5) {
            uint64_t one = hashing[j];
            uint64_t two = hashing[j + 1];
            chi(j, one, two);
        }
        iota(round);
    }
    return;
}

void add(const void* data, size_t num) {
    const uint8_t* current = (const uint8_t*)data;
    if (sizeof_buffer > 0) {
        while (num > 0 && sizeof_buffer < sizeof_block) {
            buf[sizeof_buffer++] = *current++;
            num--;
        }
    }
    if (sizeof_buffer == sizeof_block) {
        block_process((void*)buf);
        _num += sizeof_block;
        sizeof_buffer = 0;
    }
    if (num == 0) {
        return;
    }
    while (num >= sizeof_block) {
        block_process(current);
        current += sizeof_block;
        _num += sizeof_block;
        num -= sizeof_block;
    }
    while (num > 0) {
        buf[sizeof_buffer++] = *current++;
        num--;
    }
    return;
}

string sha3() {
    uint64_t last[100];
    for (unsigned i = 0; i < 100; i++) {
        last[i] = hashing[i];
    }
    size_t offset = sizeof_buffer;
    buf[offset++] = 0x06;
    while (offset < sizeof_block) {
        buf[offset++] = 0;
    }
    buf[offset - 1] |= 0x80;
    block_process(buf);
    char hex[17] = "0123456789abcdef";
    unsigned hashLength = 4;
    string result;
    result.reserve(64);
    for (unsigned i = 0; i < hashLength; i++) {
        for (unsigned j = 0; j < 8; j++) {
            unsigned char _one = (unsigned char)(hashing[i] >> (8 * j));
            result += hex[_one >> 4];
            result += hex[_one & 15];
        }
    }
    unsigned remain = 256 - hashLength * 64;
    unsigned process = 0;
    while (process < remain) {
        unsigned char _one = (unsigned char)(hashing[hashLength] >> process);
        result += hex[_one >> 4];
        result += hex[_one & 15];
        process += 8;
    }
    for (unsigned i = 0; i < 100; i++) {
        hashing[i] = last[i];
    }
    return result;
}

void print(uint8_t *chars, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x ", chars[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    char choice = argv[1][0];
	string number = argv[2];
	string path;
	string result;
    if (choice == 'n') {
		path = "data/text_" + number + ".txt";
        result = "text_sha3_256_" + number + ".txt";
	}
	else if (choice = 't') {
		path = "input_8M.txt";
		result = "output_8M.txt";
	}
    FILE *fp;
    unsigned char buffer[1048576];
    unsigned int num;
    int i = 0;
    const char *input = path.c_str();
    fp = fopen(input, "r"); 
    while (1) {
        if (fscanf(fp, "%02x", &num) != 1) {
            break;
        }
        buffer[i] = (unsigned char)num;
        i++;
    }
    fclose(fp);
    string contents((char *)buffer, i);
    int length = contents.length() * 2;
	cout << "数据 = " << (double)length * 8 / 1000000 << "Mb" << endl;
    sizeof_block = 136;
    clock_t start_time = clock();
    add(contents.c_str(), contents.size());
    string hash = sha3();
    time_t end_time = clock();
    double time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    string hashresult = hash;
	cout << "时间 = " << time << "s" << endl;
	cout << "效率 = " << (double)length * 8 / 1000000 / time << "Mbps" << endl;
    ofstream fo(result);
    for (int i = 0; i < hashresult.length(); i+=2) {
        if (i == (hashresult.length() - 1)) {
            fo << hashresult[i];
            break;
        }
        else if (i == (hashresult.length() - 2)) {
            fo << hashresult[i] << hashresult[i + 1];
            break;
        }
        fo << hashresult[i] << hashresult[i + 1] << ' ';
    }
    fo.close();
    return 0;
}
