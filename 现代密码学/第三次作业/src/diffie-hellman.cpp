#include "common.h"
#include "diffie-hellman.h"

BigUnsigned diffie_hellman(BigUnsigned received_pk, BigUnsigned my_sk)
{
   return modexp(received_pk, my_sk, DH::p);
}

DHKeypair::DHKeypair(BigUnsigned s, BigUnsigned p): sk(s), pk(p) {};

DHKeypair::DHKeypair(BigUnsigned s)
{
   sk = s;
   pk = modexp(DH::g, s, DH::p);
}

DHKeypair::DHKeypair()
{
   sk = 0;
   while (sk == 0 || sk == 1 || sk == DH::p - 1) // avoid unsafe secret key
   {
      for (int i = 0; i < 64; i++) {
         sk += (uint32_t)(rand());
         sk << 32;
      }
      sk = sk % DH::p;
   }
   pk = modexp(DH::g, sk, DH::p);
}

uint8_t* key_to_uint8_t_array(const BigUnsigned num)
{
   uint8_t *res = new uint8_t[256];
   std::string numstr = std::string(BigUnsignedInABase(num, 16));
   int zerolen = 256 - numstr.size();
   std::string resstr;
   if (zerolen > 0) 
   {
     resstr = std::string(zerolen, ' ') + numstr;
   } 
   else
   {
      resstr = numstr;
   }

   for (int i = 0; i < 256; i++)
   {
      res[i] = (hex_to_int(resstr[2*i]) << 4) + hex_to_int(resstr[2*i+1]);
   }
   return res;
}
