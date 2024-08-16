#include "message.h"
#include "common.h"

Message::Message(): data(nullptr), message_len(0), tag(nullptr), tag_len(0) {}
Message::~Message() {
    if (data != nullptr) {
        delete[] data;
    }
    if (tag != nullptr) {
        delete[] tag;
    }
}
Message::Message(const Message& r): header(r.header), message_len(r.message_len), tag_len(r.tag_len) {
    memcpy(IV, r.IV, sizeof(IV));
    if (r.data == nullptr) {
        data = nullptr;
    } else {
        data = new uint8_t[r.message_len];
        memcpy(data, r.data, r.message_len);
    }
    if (r.tag == nullptr) {
        tag = nullptr;
    } else {
        tag = new uint8_t[r.tag_len];
        memcpy(tag, r.tag, r.tag_len);
    }
}
Message::Message(Message&& r): header(r.header), message_len(r.message_len), tag_len(r.tag_len) {
    memcpy(IV, r.IV, sizeof(IV));
    data = r.data;
    r.data = nullptr;
    tag = r.tag;
    r.tag = nullptr;
}
Message& Message::operator=(const Message& r) {
    if (this == &r) return *this;
    if (data != nullptr) {
        delete[] data;
    }
    if (tag != nullptr) {
        delete[] tag;
    }
    header = r.header;
    message_len = r.message_len;
    tag_len = r.tag_len;
    memcpy(IV, r.IV, sizeof(IV));
    if (r.data == nullptr) {
        data = nullptr;
    } else {
        data = new uint8_t[r.message_len];
        memcpy(data, r.data, r.message_len);
    }
    if (r.tag == nullptr) {
        tag = nullptr;
    } else {
        tag = new uint8_t[r.tag_len];
        memcpy(tag, r.tag, r.tag_len);
    }
    return *this;
}