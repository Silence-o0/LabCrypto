#ifndef LAB1CRYPTO_ADDITIONAL_FUNC_H
#define LAB1CRYPTO_ADDITIONAL_FUNC_H

#include <string>
#include <iostream>
using namespace std;

__int128 mod_pow(__int128 a, __int128 q, __int128 n);
void print(__int128 x);
int jacobi(__int128 a, __int128 n);
string print_binary_num(__int128 n);
string print_base64_num(__int128 n);
void print_byte(__int128 n);
__int128 gcd(__int128 a, __int128 b);
__int128 lcm_method(__int128 a, __int128 b);
__int128 get_coprime(__int128 a);

#endif //LAB1CRYPTO_ADDITIONAL_FUNC_H
