#include <iostream>
#include <string>
#include <chrono>
#include "lab1.h"
#include "additional_func.h"

using namespace std;

class Public_key {
    public:
        __int128 e;
        __int128 p;
        __int128 q;
        Public_key(__int128 e, __int128 p, __int128 q) {
            this->e = e;
            this->p = p;
            this->q = q;
        }
};

class RSAMessage {
public:
    vector <__int128> m;
    Public_key public_key;
    RSAMessage(vector <__int128> m, Public_key key): public_key(key) {
        this->m = m;
        this->public_key = key;
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

RSAMessage EncryptMessage(string message) {
    vector <__int128> encodedMessage = getEncodedMessage(message);
    vector <__int128> encryptedMessage;
    __int128 p, q, e, c;

    vector <__int128> prime_numbers = find_primes_with_bit_length(16, 10, 2, false);
    p = prime_numbers[0];
    q = prime_numbers[1];
    __int128 n = p * q;
    __int128 lambda_n = lcm_method(p-1 , q-1);
    e = get_coprime(lambda_n);

    for (int encodedSymbol : encodedMessage) {
        c = mod_pow(encodedSymbol, e, n);
        encryptedMessage.push_back(c);
    }

    Public_key key (e, p, q);
    RSAMessage rsaMessage(encryptedMessage, key);
    return rsaMessage;
}

string DecryptMessage(RSAMessage rsaMessage) {
    __int128 d, m_decr;
    __int128 lambda_n = lcm_method(rsaMessage.public_key.p-1 , rsaMessage.public_key.q-1);
    d = extendedEuclidean(rsaMessage.public_key.e, lambda_n);
    vector <__int128> decryptedEncodedMessage;

    for (int encodedSymbol : rsaMessage.m) {
        m_decr = chineseRemainderDecrypt(encodedSymbol, d, rsaMessage.public_key.p, rsaMessage.public_key.q);
        decryptedEncodedMessage.push_back(m_decr);
    }

    string message;
    message = getDecodedMessage(decryptedEncodedMessage);
    return message;
}

void lab2() {
    string s = "RSA (Rivest Shamir Adleman) is a public-key cryptosystem, one of the oldest widely used for secure data transmission.";
//    cout << "Write your message:";
//    getline(cin, s);
//    auto start = chrono::high_resolution_clock::now();

    RSAMessage rsaMessage = EncryptMessage(s);

    cout << "Encrypted message: ";
    for (int encodedSymbol : rsaMessage.m) {
        cout << encodedSymbol << ", ";
    }
    cout << endl;

    string res = DecryptMessage(rsaMessage);
//    auto end = chrono::high_resolution_clock::now();

    cout << "Your decrypted message: " << res;

//    chrono::duration<double, milli> duration = end - start;
//    cout << "Time: " << duration.count() << "ms." << endl;
}


