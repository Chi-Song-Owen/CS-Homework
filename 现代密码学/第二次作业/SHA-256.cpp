#include <stdint.h>
#include <cstring>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <algorithm>

using namespace std;

#define shw(x, n) ((x << (n & 31)) & 0xffffffff)
#define shb(x, n) ((x >> (n & 31)) & 0xff)
#define _r(x, n) ((x >> n) | (x << (32 - n)))
#define Ch(x, y, z) ((x & y) ^ ((~x) & z))
#define Ma(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define S0(x) (_r(x, 2) ^ _r(x, 13) ^ _r(x, 22))
#define S1(x) (_r(x, 6) ^ _r(x, 11) ^ _r(x, 25))
#define G0(x) (_r(x, 7) ^ _r(x, 18) ^ (x >> 3))
#define G1(x) (_r(x, 17) ^ _r(x, 19) ^ (x >> 10))

uint8_t  buf[64] = {0};
uint32_t hashing[8] = {0};
uint32_t bits[2] = {0};
uint32_t len = 0;
uint32_t rfu__ = 0;
uint32_t W[64] = {0};

uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t _word(uint8_t *c) {
    return (shw(c[0], 24) | shw(c[1], 16) | shw(c[2], 8) | (c[3]));
}

void print(__uint128_t t) {
	uint8_t* ptr = (uint8_t*)(&t);
	for (int i = 0; i < 4; ++i, printf(" "))
		for (int j = 0; j < 4; ++j)
            printf("%02x", ptr[((3-i) * 4 + (3-j))]);
	puts("");
}

__uint128_t* exchange(const string& _hex) {
    __uint128_t* part = new __uint128_t[_hex.size() / 32 + 1];
    uint64_t high = 0, low = 0;
    int length = _hex.size();
    int index = 0;
    int count = 0;
    while (length > 0) {
        count++;
        __uint128_t result = *part;
        string hexstr = _hex;
        if (length < 32 && length > 0) {
            for (int i = 0; i < 32 - length; i++) {
                hexstr = hexstr + "0";
            }
        }
        std::istringstream iss(hexstr.substr(index, 16));
        iss >> std::hex >> high;
        std::istringstream iss2(hexstr.substr(index + 16, 16));
        iss2 >> std::hex >> low;
        *part = high;
        *part = (*part << 64) | low;
        if (length < 32) {
            *part = *part >> (32 - length) * 4;
        }        
        part = part + 1;
        length -= 32;
        index += 32;
    }
    part = part - count;
    return part;
}

void add(uint32_t n) {
    if (bits[0] > (0xffffffff - n)) {
        bits[1] = (bits[1] + 1) & 0xFFFFFFFF;
    }
    bits[0] = (bits[0] + n) & 0xFFFFFFFF;
}

void _hashing(){
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t[2];
    a = hashing[0];
    b = hashing[1];
    c = hashing[2];
    d = hashing[3];
    e = hashing[4];
    f = hashing[5];
    g = hashing[6];
    h = hashing[7];
    for (uint32_t i = 0; i < 64; i++) {
        if (i < 16) {
            W[i] = _word(&buf[shw(i, 2)]);
        } else {
            W[i] = G1(W[i - 2])  + W[i - 7] +
                        G0(W[i - 15]) + W[i - 16];
        }
        t[0] = h + S1(e) + Ch(e, f, g) + K[i] + W[i];
        t[1] = S0(a) + Ma(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t[0];
        d = c;
        c = b;
        b = a;
        a = t[0] + t[1];
    }
    hashing[0] += a;
    hashing[1] += b;
    hashing[2] += c;
    hashing[3] += d;
    hashing[4] += e;
    hashing[5] += f;
    hashing[6] += g;
    hashing[7] += h;
}

void sha256(const __uint128_t* data, size_t length) {
    const uint8_t *_data = (uint8_t* )(data);
    // print(*data);
    int leng = length / 2;
    // cout << leng << endl;
    if (len < sizeof(buf)) {
        int count = 16;
        int i = 0;
        int idx = leng - 1;
        while (i < leng) {
            if ((leng - i) <= 16) {
                buf[len++] = _data[idx];
                i++;
                idx--;
                if (len == sizeof(buf)) {
                    _hashing();
                    add(sizeof(buf) * 8);
                    len = 0;
                }
            }
            else {
                count = 16;
                int index = i + 15;
                while (count > 0) {
                    buf[len++] = _data[index];
                    // cout << sizeof(buf) << endl;
                    // cout << len << endl;
                    if (len == sizeof(buf)) {
                        _hashing();
                        add(sizeof(buf) * 8);
                        len = 0;
                    }
                    index--;
                    count--;
                }
                i = i + 16;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    char choice = argv[1][0];
	string number = argv[2];
	string path;
	string result;
    if (choice == 'n') {
		path = "data/text_" + number + ".txt";
        result = "text_sha_256_" + number + ".txt";
	}
	else if (choice = 't') {
		path = "input_8K.txt";
		result = "output_8K.txt";
	}
    ifstream fi(path);
    stringstream buffer;
    buffer << fi.rdbuf();
    fi.close();
    string contents(buffer.str());
    contents.erase(remove(contents.begin(), contents.end(), ' '), contents.end());
    uint8_t hashingresult[32];
    __uint128_t* data = exchange(contents);
    int length = contents.size();
	cout << length << endl;
	cout << "数据 = " << (double)length * 8 / 1000000 << "Mb" << endl;
    clock_t start_time = clock();
    // init
    bits[0] = bits[1] = len = 0;
    hashing[0] = 0x6a09e667;
    hashing[1] = 0xbb67ae85;
    hashing[2] = 0x3c6ef372;
    hashing[3] = 0xa54ff53a;
    hashing[4] = 0x510e527f;
    hashing[5] = 0x9b05688c;
    hashing[6] = 0x1f83d9ab;
    hashing[7] = 0x5be0cd19;
    // hash
    sha256(data, length);
    // result
    uint32_t i, j;
    j = len % sizeof(buf);
    buf[j] = 0x80;
    for (i = j + 1; i < sizeof(buf); i++) {
        buf[i] = 0x00;
    }
    if (len > 55) {
        _hashing();
        for (j = 0; j < sizeof(buf); j++) {
            buf[j] = 0x00;
        }
    }
    add(len * 8);
    buf[63] = shb(bits[0],  0);
    buf[62] = shb(bits[0],  8);
    buf[61] = shb(bits[0], 16);
    buf[60] = shb(bits[0], 24);
    buf[59] = shb(bits[1],  0);
    buf[58] = shb(bits[1],  8);
    buf[57] = shb(bits[1], 16);
    buf[56] = shb(bits[1], 24);
    _hashing();
    if (hashing != NULL) {
        for (i = 0, j = 24; i < 4; i++, j -= 8) {
            hashingresult[i +  0] = shb(hashing[0], j);
            hashingresult[i +  4] = shb(hashing[1], j);
            hashingresult[i +  8] = shb(hashing[2], j);
            hashingresult[i + 12] = shb(hashing[3], j);
            hashingresult[i + 16] = shb(hashing[4], j);
            hashingresult[i + 20] = shb(hashing[5], j);
            hashingresult[i + 24] = shb(hashing[6], j);
            hashingresult[i + 28] = shb(hashing[7], j);
        }
    }
    time_t end_time = clock();
    double time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    ofstream fo(result);
    for (int t = 0; t < 32; t++) {
        if (t == 31) {
            fo << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hashingresult[t]);
        }
        else {
            fo << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hashingresult[t]) << ' ';
        }    
    }
	cout << "时间 = " << time << "s" << endl;
	cout << "效率 = " << (double)length * 8 / 1000000 / time << "Mbps" << endl;
    return 0;
}