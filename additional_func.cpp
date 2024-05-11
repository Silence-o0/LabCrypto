#include <iostream>
#include <random>
#include <stack>
#include <bitset>
#include <string>
#include <cstring>

using namespace std;

__int128 mod_pow(__int128 a, __int128 q, __int128 n) {
    __int128 result = 1;
    a %= n;
    while (q > 0) {
        if (q & 1)
            result = (result * a) % n;
        a = (a * a) % n;
        q >>= 1;
    }
    return result;
}

void print(__int128 x) {
    if (x == 0) {
        cout << 0;
        return;
    }
    if (x < 0) {
        cout << '-';
        x = -x;
    }
    string s;
    while (x > 0) {
        s += char(x % 10 + '0');
        x /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s;
}


int jacobi(__int128 a, __int128 n) {
    if (n <= 0 || n % 2 == 0)
        return 0;

    int result = 1;
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5)
                result = -result;
        }
        swap(a, n);
        if (a % 4 == 3 && n % 4 == 3)
            result = -result;
        a %= n;
    }

    if (n == 1)
        return result;
    else
        return 0;
}

string print_binary_num(__int128 n) {
    string s;
    __int128 result_n = n;
    while (result_n > 1) {
        char symbol = '0' + (result_n % 2);
        s += symbol;
        result_n /= 2;
    }
    s += '1';
    reverse(s.begin(), s.end());
    return s;
}

string print_base64_num(__int128 n) {
    string s;
    __int128 result_n = n;
    while (result_n > 1) {
        char symbol;
        int r = result_n % 64;
        if (r < 26) symbol = static_cast<char>(r+65);
//            symbol = '0' + r;
        else if (r >= 26 and r < 52) {
            symbol = static_cast<char>(r+71);
        }
        else if (r == 62) symbol = '+';
        else if (r == 63) symbol = '/';
        else {
            symbol = '0' + r;
        }
        s += symbol;
        result_n /= 64;
    }
    reverse(s.begin(), s.end());
    return s;
}

void print_byte(__int128 n) {
    char byte_array[sizeof(__int128)];

    memcpy(byte_array, &n, sizeof(__int128));
    for (size_t i = 0; i < sizeof(__int128); ++i) {
        cout << " " << static_cast<int>(byte_array[i]);
    }
}

__int128 gcd(__int128 a, __int128 b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

__int128 lcm_method(__int128 a, __int128 b) {
    return (a / gcd(a, b)) * b;
}

__int128 get_coprime(__int128 a) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<__int128> distrib(1, a);

    __int128 b;
    while(1) {
        b = distrib(gen);
        if (gcd(a, b) == 1) {
            break;
        }
    }
    return b;
}
