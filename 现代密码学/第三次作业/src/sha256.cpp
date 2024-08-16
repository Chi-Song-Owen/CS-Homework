#include "sha256.h"

#define FN_ static inline __attribute__((const))
#define shw(x, n) ((x << (n & 31)) & 0xffffffff)
#define shb(x, n) ((x >> (n & 31)) & 0xff)
#define _r(x, n) ((x >> n) | (x << (32 - n)))
#define Ch(x, y, z) ((x & y) ^ ((~x) & z))
#define Ma(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define S0(x) (_r(x, 2) ^ _r(x, 13) ^ _r(x, 22))
#define S1(x) (_r(x, 6) ^ _r(x, 11) ^ _r(x, 25))
#define G0(x) (_r(x, 7) ^ _r(x, 18) ^ (x >> 3))
#define G1(x) (_r(x, 17) ^ _r(x, 19) ^ (x >> 10))

static const uint32_t K[64] = {
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

static uint32_t _word(uint8_t *c) {
    return (shw(c[0], 24) | shw(c[1], 16) | shw(c[2], 8) | (c[3]));
}

static void add(text *context, uint32_t n) {
    if (context->bits[0] > (0xffffffff - n)) {
        context->bits[1] = (context->bits[1] + 1) & 0xFFFFFFFF;
    }
    context->bits[0] = (context->bits[0] + n) & 0xFFFFFFFF;
}

static void hashing(text *context) {
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t[2];

    a = context->hash[0];
    b = context->hash[1];
    c = context->hash[2];
    d = context->hash[3];
    e = context->hash[4];
    f = context->hash[5];
    g = context->hash[6];
    h = context->hash[7];

    for (uint32_t i = 0; i < 64; i++) {
        if (i < 16) {
            context->W[i] = _word(&context->buf[shw(i, 2)]);
        }
        else {
            context->W[i] = G1(context->W[i - 2])  + context->W[i - 7] + G0(context->W[i - 15]) + context->W[i - 16];
        }

        t[0] = h + S1(e) + Ch(e, f, g) + K[i] + context->W[i];
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

    context->hash[0] += a;
    context->hash[1] += b;
    context->hash[2] += c;
    context->hash[3] += d;
    context->hash[4] += e;
    context->hash[5] += f;
    context->hash[6] += g;
    context->hash[7] += h;
}

void main_hash(text *context, uint8_t *hash)
{
    register uint32_t i, j;

    if (context != NULL) {
        j = context->len % sizeof(context->buf);
        context->buf[j] = 0x80;
        for (i = j + 1; i < sizeof(context->buf); i++) {
            context->buf[i] = 0x00;
        }

        if (context->len > 55) {
            hashing(context);
            for (j = 0; j < sizeof(context->buf); j++) {
                context->buf[j] = 0x00;
            }
        }

        add(context, context->len * 8);
        context->buf[63] = shb(context->bits[0],  0);
        context->buf[62] = shb(context->bits[0],  8);
        context->buf[61] = shb(context->bits[0], 16);
        context->buf[60] = shb(context->bits[0], 24);
        context->buf[59] = shb(context->bits[1],  0);
        context->buf[58] = shb(context->bits[1],  8);
        context->buf[57] = shb(context->bits[1], 16);
        context->buf[56] = shb(context->bits[1], 24);
        hashing(context);

        if (hash != NULL) {
            for (i = 0, j = 24; i < 4; i++, j -= 8) {
                hash[i +  0] = shb(context->hash[0], j);
                hash[i +  4] = shb(context->hash[1], j);
                hash[i +  8] = shb(context->hash[2], j);
                hash[i + 12] = shb(context->hash[3], j);
                hash[i + 16] = shb(context->hash[4], j);
                hash[i + 20] = shb(context->hash[5], j);
                hash[i + 24] = shb(context->hash[6], j);
                hash[i + 28] = shb(context->hash[7], j);
            }
        }
    }
}

uint8_t* sha_256(const uint8_t *message, const int& m_len_bits) {
    uint8_t *digest = new uint8_t[32];
    /* Your code here */
    text context;
    const uint8_t *bytes = (const uint8_t *)message;
    (&context)->bits[0] = (&context)->bits[1] = (&context)->len = 0;
    (&context)->hash[0] = 0x6a09e667;
    (&context)->hash[1] = 0xbb67ae85;
    (&context)->hash[2] = 0x3c6ef372;
    (&context)->hash[3] = 0xa54ff53a;
    (&context)->hash[4] = 0x510e527f;
    (&context)->hash[5] = 0x9b05688c;
    (&context)->hash[6] = 0x1f83d9ab;
    (&context)->hash[7] = 0x5be0cd19;

    if (((&context) != NULL) && (bytes != NULL) && ((&context)->len < sizeof((&context)->buf))) {
        for (size_t i = 0; i < m_len_bits / 8; i++) {
            (&context)->buf[(&context)->len++] = bytes[i];
            if ((&context)->len == sizeof((&context)->buf)) {
                hashing((&context));
                add((&context), sizeof((&context)->buf) * 8);
                (&context)->len = 0;
            }
        }
    }
    main_hash(&context, digest);
    return digest;
}

bool SHA_check_vector() {
    const uint8_t *message = (const uint8_t*)"abc";
    uint8_t *digest = sha_256(message, 24);
    uint8_t goal_digest[32] = {0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad};
    for (int i = 0; i < 32; i++) {
        if (digest[i] != goal_digest[i]) {
            printf("SHA-256 testvector1 not verified!\n");
            delete[] digest;
            return false;
        }
    }
    printf("SHA-256 testvector1 verified!\n");
    delete[] digest;
    message = (const uint8_t*)"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    digest = sha_256(message, 56 * 8);
    uint8_t goal_digest2[32] = {0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8, 0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39, 0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67, 0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1};
    for (int i = 0; i < 32; i++) {
        if (digest[i] != goal_digest2[i]) {
            printf("SHA-256 testvector2 not verified!\n");
            delete[] digest;
            return false;
        }
    }
    printf("SHA-256 testvector2 verified!\n");
    delete[] digest;
    return true;
}