#include "common.h"
#include "aes-gcm.h"
#include "double-ratchet.h"
#include "messenger.h"
#include "hmac.h"

enum Party { Alice, Bob };

/* 不允许改动该文件 */
int main(int argc, char* argv[])
{
    srand(time(NULL));
    DHKeypair bob_initial_pair;
    KDF_key kdf_rk;
    for (int i = 0; i < 32; i++) kdf_rk[i] = rand() & 0xff;
    Messenger alice(kdf_rk, bob_initial_pair.pk), bob(kdf_rk, bob_initial_pair);
    //Alice
    std::string message = "Hello, bob.", message2 = "undefined";
    Message encrpyted_message = alice.send_message(message);
    bool check_res = bob.receive_message(encrpyted_message, message2);
    printf("Check res is %d, Bob received message is %s\n", check_res, message2.c_str());
    for (int i = 0; i < 20; i++) {
        bool send_party = rand() & 0x1;
        message = "Name a random thing: " + std::to_string(rand());
        if (send_party) {
            message2 = "";
            encrpyted_message = alice.send_message(message);
            check_res = bob.receive_message(encrpyted_message, message2);
            printf("Test #%d, Alice sent %s, Bob received %s, check res is %d.\n", i, message.c_str(), message2.c_str(), check_res);
        } else {
            message2 = "";
            encrpyted_message = bob.send_message(message);
            check_res = alice.receive_message(encrpyted_message, message2);
            printf("Test #%d, Bob sent %s, Alice received %s, check res is %d.\n", i, message.c_str(), message2.c_str(), check_res);
        }
    }
    return 0;
}