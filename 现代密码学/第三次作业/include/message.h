#pragma once
#include "common.h"
#include "bigint/BigIntegerLibrary.hh"

struct message_header {
    BigUnsigned DH_pub_key;
};

class Message {
public:
    message_header header;
    int message_len, tag_len;
    uint8_t *data, *tag;
    uint8_t IV[12];
    Message();
    Message(const Message& r);
    Message(Message&& r);
    Message& operator=(const Message& r);
    ~Message();
};