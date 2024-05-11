#ifndef LAB1CRYPTO_LAB1_H
#define LAB1CRYPTO_LAB1_H

#include <vector>

bool millerRabinTest(__int128 n, int k);
bool baillie_psw(__int128 n);
std::vector <__int128> find_primes_with_bit_length(int bit_length, int k, int quantity, bool p);
void lab1();

#endif //LAB1CRYPTO_LAB1_H
