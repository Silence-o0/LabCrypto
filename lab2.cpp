#include <iostream>
#include <string>
#include <chrono>
#include "lab1.h"
#include "additional_func.h"

using namespace std;

class Public_key {
    public:
        __int128 e;
        __int128 n;
        Public_key(__int128 e, __int128 n) {
            this->e = e;
            this->n = n;
        }
};

class RSAMessage {
public:
    vector <__int128> m;
    __int128 sign;
    Public_key public_key;
    Public_key public_sign_key;
    RSAMessage(vector <__int128> m, __int128 sign, Public_key key, Public_key sign_key): public_key(key),
                                                                                         public_sign_key(sign_key) {
        this->m = m;
        this->sign = sign;
        this->public_key = key;
        this->public_sign_key = sign_key;
    }
};

__int128 chineseRemainderDecrypt(__int128 c, __int128 d, __int128 p, __int128 q) {
    __int128 c_q = c % q;
    __int128 c_p = c % p;
    __int128 d_q = d % (q-1);
    __int128 d_p = d % (p-1);
    __int128 m_q = mod_pow(c_q, d_q, q);
    __int128 m_p = mod_pow(c_p, d_p, p);

    __int128 m = 1;

    while (true)
    {
        if (m%p == m_p and m%q == m_q)
            return m;
        m++;
    }
}

__int128 extendedEuclidean(__int128 a, __int128 b) {
    __int128 x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    __int128 b_temp = b;
    while (b_temp != 0) {
        __int128 q = a / b_temp;
        __int128 temp = b_temp;
        b_temp = a % b_temp;
        a = temp;
        temp = x1;
        x1 = x0 - q * x1;
        x0 = temp;
        temp = y1;
        y1 = y0 - q * y1;
        y0 = temp;
    }
    if (x0 < 0) {
        x0 += b;
    }
    return x0;
}

void testEncryptDecrypt(__int128 m) {
    __int128 p, q, e, d, c, m_decr;

    cout << "m: "; print(m); cout << endl;
    vector <__int128> prime_numbers = find_primes_with_bit_length(16, 10, 2, false);
    p = prime_numbers[0];
    q = prime_numbers[1];
    __int128 n = p * q;
    cout << "n: "; print(n); cout << endl;
    cout << "p: "; print(p); cout << endl;
    cout << "q: "; print(q); cout << endl;

    __int128 lambda_n = lcm_method(p-1 , q-1);
    cout << "lambda_n: "; print(lambda_n); cout << endl;

    e = get_coprime(lambda_n);
    cout << "e: "; print(e); cout << endl;
    d = extendedEuclidean(e, lambda_n);
    cout << "d: "; print(d); cout << endl;

    c = mod_pow(m, e, n);
    cout << "Encrypted: "; print(c); cout << endl;

    m_decr = chineseRemainderDecrypt(c, d, p, q);
    cout << "Decrypted: "; print(m_decr); cout << endl;
}

vector <__int128> getEncodedMessage(string message) {
    vector <__int128> vector_message;
    for (char c : message) {
        vector_message.push_back(static_cast<int>(c));
    }
    return vector_message;
}

string getDecodedMessage(vector <__int128> encodedMessage) {
    string message;
    for (int symbol: encodedMessage) {
        message += static_cast<char>(symbol);
    }
    return message;
}

uint64_t polynomial_rolling_hash(const std::string& str, uint64_t p, uint64_t m) {
    uint64_t hash_value = 0;
    uint64_t p_pow = 1;

    for (char c : str) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

RSAMessage EncryptMessage(string message, Public_key key, uint64_t p_hash, uint64_t m_hash) {
    vector <__int128> prime_numbers_sign = find_primes_with_bit_length(32, 10, 2, false);
    __int128 p_sign = prime_numbers_sign[0];
    __int128 q_sign = prime_numbers_sign[1];
    __int128 n_sign = p_sign * q_sign;
    __int128 lambda_n = lcm_method(p_sign-1 , q_sign-1);
    __int128 e_sign = get_coprime(lambda_n);
    __int128 d_sign = extendedEuclidean(e_sign, lambda_n);

    uint64_t hashed_message = polynomial_rolling_hash(message, p_hash, m_hash);
    uint64_t sign = mod_pow(hashed_message, d_sign, n_sign);

    Public_key sign_key (e_sign, n_sign);
    vector <__int128> encodedMessage = getEncodedMessage(message);
    vector <__int128> encryptedMessage;
    __int128 c;

    for (int encodedSymbol : encodedMessage) {
        c = mod_pow(encodedSymbol, key.e, key.n);
        encryptedMessage.push_back(c);
    }

    RSAMessage rsaMessage(encryptedMessage, sign, key, sign_key);
    return rsaMessage;
}

string DecryptMessage(RSAMessage rsaMessage, __int128 p, __int128 q, __int128 lambda_n, uint64_t p_hash, uint64_t m_hash) {
    __int128 d, m_decr;
    d = extendedEuclidean(rsaMessage.public_key.e, lambda_n);
    vector <__int128> decryptedEncodedMessage;

    for (int encodedSymbol : rsaMessage.m) {
        m_decr = chineseRemainderDecrypt(encodedSymbol, d, p, q);
        decryptedEncodedMessage.push_back(m_decr);
    }

    string message;
    message = getDecodedMessage(decryptedEncodedMessage);
    uint64_t hashed_message = polynomial_rolling_hash(message, p_hash, m_hash);


    uint64_t m = mod_pow(rsaMessage.sign, rsaMessage.public_sign_key.e, rsaMessage.public_sign_key.n);
    if(hashed_message != m) {
        cout << "Error verifying." << endl;
    }
    return message;
}

void lab2() {
    string s = "RSA (Rivest Shamir Adleman) is a public-key cryptosystem, one of the oldest widely used for secure data transmission.";
//    cout << "Write your message:";
//    getline(cin, s);
    auto start = chrono::high_resolution_clock::now();

    uint64_t p_hash = find_primes_with_bit_length(16, 10, 1, false)[0];
    uint64_t m_hash = find_primes_with_bit_length(16, 10, 1, false)[0];

    __int128 p, q, e;
    vector <__int128> prime_numbers = find_primes_with_bit_length(16, 10, 2, false);
    p = prime_numbers[0];
    q = prime_numbers[1];
    __int128 n = p * q;
    __int128 lambda_n = lcm_method(p-1 , q-1);
    e = get_coprime(lambda_n);

    Public_key key (e, n);
    RSAMessage rsaMessage = EncryptMessage(s, key, p_hash, m_hash);

    cout << "Encrypted message: ";
    for (int encodedSymbol : rsaMessage.m) {
        cout << encodedSymbol << ", ";
    }
    cout << endl;

    string res = DecryptMessage(rsaMessage, p, q, lambda_n, p_hash, m_hash);
    auto end = chrono::high_resolution_clock::now();

    cout << "Your decrypted message: " << res << endl;

    chrono::duration<double, milli> duration = end - start;
    cout << "Time: " << duration.count() << "ms." << endl;
}


