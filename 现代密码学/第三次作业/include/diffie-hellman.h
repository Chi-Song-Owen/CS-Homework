// Diffie-hellman keypair

/*
https://datatracker.ietf.org/doc/html/rfc3526
2048-bit MODP Group

   This group is assigned id 14.

   This prime is: 2^2048 - 2^1984 - 1 + 2^64 * { [2^1918 pi] + 124476 }

   Its hexadecimal value is:

      FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1
      29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD
      EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245
      E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED
      EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D
      C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F
      83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D
      670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B
      E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9
      DE2BCBF6 95581718 3995497C EA956AE5 15D22618 98FA0510
      15728E5A 8AACAA68 FFFFFFFF FFFFFFFF

   The generator is: 2.
*/
#pragma once
#include "bigint/BigIntegerLibrary.hh"

namespace DH {
const BigUnsigned p = stringToBigUnsigned("32317006071311007300338913926423828248817941241140239112842009751400741706634354222619689417363569347117901737909704191754605873209195028853758986185622153212175412514901774520270235796078236248884246189477587641105928646099411723245426622522193230540919037680524235519125679715870117001058055877651038861847280257976054903569732561526167081339361799541336476559160368317896729073178384589680639671900977202194168647225871031411336429319536193471636533209717077448227988588565369208645296636077250268955505928362751121174096972998068410554359584866583291642136218231078990999448652468262416972035911852507045361090559");
// equal to 2^2048 - 2^1984 - 1 + 2^64 * { [2^1918 pi] + 124476 }

const BigUnsigned g(2);
}

class DHKeypair {
public:
   BigUnsigned sk;
   BigUnsigned pk;

   // Generate a random keypair
   DHKeypair();

   // Generate a DHkeypair from sk and pk; 
   // Note: the correctness of pair is not checked
   DHKeypair(BigUnsigned s, BigUnsigned p);

   // Generate a DHkeypair from secret key
   DHKeypair(BigUnsigned s); 
};

// Return a new common number when receiving another party's pubkey
BigUnsigned diffie_hellman(BigUnsigned receivedpk, BigUnsigned my_sk);

// Convert a diffle hellman key to uint8_t array. output size 256 byte
uint8_t* key_to_uint8_t_array(const BigUnsigned num);