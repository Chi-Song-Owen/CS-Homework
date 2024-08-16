#ifndef AES_H
#define AES_H
#pragma once
#include <iostream>
#include <bits/stdc++.h>
#include <bitset>
#include <cstring>
using namespace std;

#define get(t, index) ((t >> (15 - index) * 8) & (0xff))

#define change(t, index, num) (t -= get(t, index) << (15 - index) * 8, \
                                t += num << (15 - index) * 8, \
                                t)

extern uint32_t w[44];


extern const unsigned short sbox[256];

extern __uint128_t gf02[256];

extern __uint128_t gf03[256];

extern __uint128_t shift[7];

extern uint8_t rcon[11];

void AES_CTR_Encrypt(const uint8_t *key, const uint8_t *iv, const uint8_t *input, uint8_t *output, uint64_t length);
#endif // AES_H