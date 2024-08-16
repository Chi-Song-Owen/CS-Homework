// This file complies into messenger_test

#include "common.h"
#include "messenger.h"
#include "diffie-hellman.h"

// Test from stdin
// For each message, first ask the sender
// Then input the message
int main()
{
    srand(time(NULL));
    DHKeypair bob_initial_pair;
    KDF_key kdf_rk;
    for (int i = 0; i < 32; i++) kdf_rk[i] = rand() & 0xff;

    // Alice gets KDF key and the first pubkey, Bob get KDF key and first keypair
    Messenger alice(kdf_rk, bob_initial_pair.pk), bob(kdf_rk, bob_initial_pair);
    
    // Alice 
    std::string message;
    std::cout << "Alice's message: ";
    std::getline(std::cin, message);
    
    std::string plaintext = "";
    Message bobs_recieved_message = alice.send_message(message);
    if (bob.receive_message(bobs_recieved_message, plaintext))
    {
        std::cout << "Bob received: " << plaintext << std::endl;
    }
    else
    {
        std::cout << "received message authentication fail!" << std::endl;
        return -1;
    }

    while (1)
    {
        std::string party;
        int party_next_round;
        std::cout << "Next party to send message: ";
        std::getline(std::cin, party);
        if (party[0] == 'a' || party[0] == 'A') party_next_round = 0;
        else if (party[0] == 'b' || party[0] == 'B') party_next_round = 1;
        else { std::cout << "Party incorrect, ending" << std::endl; exit(-1); }

        std::string message;
        std::cout << "Message: ";
        std::getline(std::cin, message);
        if (party_next_round == 0) // alice to send message
        {
            Message bobs_recieved_message = alice.send_message(message);
            std::string plaintext = "";
            if (bob.receive_message(bobs_recieved_message, plaintext))
            {
                std::cout << "Bob received: " << plaintext << std::endl;
            }
            else
            {
                std::cout << "received message authentication fail!" << std::endl;
                return -1;
            }
        }
        else
        {
            Message alices_recieved_message = bob.send_message(message);
            std::string plaintext = "";
            if (alice.receive_message(alices_recieved_message, plaintext))
            {
                std::cout << "Alice received: " << plaintext << std::endl;
            }
            else
            {
                std::cout << "received message authentication fail!" << std::endl;
                return -1;
            }
        }
        
    }
    return 0;
}