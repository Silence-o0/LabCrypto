#include <random>
#include <stack>
#include <bitset>
#include "additional_func.h"

using namespace std;

bool millerRabinTest(__int128 n, int k) {
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0)
        return false;

    __int128 q = n - 1;
    while (q % 2 == 0)
        q /= 2;

    for (int i = 0; i < k; ++i) {
        __int128 a = 2 + rand() % (n - 3);
        __int128 x = mod_pow(a, q, n);

        if (x == 1 || x == n - 1)
            continue;

        bool isPrime = false;
        for (long long r = 1; r < q; r *= 2) {
            x = (x * x) % n;
            if (x == 1)
                return false;
            if (x == n - 1) {
                isPrime = true;
                break;
            }
        }

        if (!isPrime)
            return false;
    }
    return true;
}


bool isStrongFermatProbablePrime(__int128 n) {
    if (n <= 1)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    __int128 r = mod_pow(2, n - 1, n);
    return r == 1;
}


std::pair<__int128, __int128> lucasRecurrence(__int128 P, __int128 Q, __int128 D, __int128 n) {
    __int128 U = 1, V = P;
    __int128 k = 1;
    __int128 Q_k = Q;

    stack <int> st;

    __int128 result_n = n + 1;
    while (result_n > 1) {
        st.push(result_n % 2);
        result_n /= 2;
    }

    __int128 U_temp, V_temp;
    while(!st.empty()) {
        int elem = st.top();
        st.pop();

        U = (U * V) % n;
        V = (V * V - 2 * Q_k) % n;
        Q_k = mod_pow(Q_k, 2, n);
        k *= 2;

        if (elem == 1) {
            U_temp = U;
            V_temp = V;

            __int128 s = P*U_temp + V_temp;
            __int128 m = (D*U_temp + P*V_temp);
            if (s & 1) {
                s += n;
            }
            if (m & 1) {
                m += n;
            }
            U = (s/2)%n;
            V = (m/2)%n;
            k += 1;
            Q_k = (Q_k * Q) % n;
        }
    }

    return make_pair(U, V);
}

bool isStrongLucasProbablePrime(__int128 n) {
    __int128 D = 5;
    while (jacobi(D, n) != -1) {
        D = -D + (D > 0 ? -2 : 2);
    }

    __int128 P = 1;
    __int128 Q = (1 - D) / 4;

    std::pair<__int128, __int128> uv = lucasRecurrence(P, Q, D, n);
    __int128 Un = uv.first;
    return Un % n == 0;
}

bool baillie_psw(__int128 n) {
    if (!isStrongFermatProbablePrime(n)) return false;
    if (!isStrongLucasProbablePrime(n)) return false;
    return true;
}

vector <__int128> find_primes_with_bit_length(int bit_length, int iterations, int quantity, bool p) {
    vector <__int128> prime_numbers;
    __int128 random_number;
    __int128 b;
    b = 1;

    for (int i = 0; i < bit_length; ++i) {
        b *= 2;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<__int128> distrib(2, b-1);

    int i = 0;
    while(i < quantity) {
        random_number = distrib(gen);
        if (baillie_psw(random_number)) {
            prime_numbers.push_back(random_number);
            i++;
            if (p) {
                cout << "base2: " << print_binary_num(random_number) << endl;
                cout << "base10: ";
                print(random_number);
                cout << endl;
                cout << "base64: " << print_base64_num(random_number) << endl;
                cout << "byte[]: ";
                print_byte(random_number);
                cout << endl;
                cout << endl;
            }
        }
    }
    return prime_numbers;
}

void lab1() {
    int bit_length = 52;
    int iterations = 10;
    int quantity = 3;

    cout << isStrongFermatProbablePrime(561);
    find_primes_with_bit_length(bit_length, iterations, quantity, true);
}