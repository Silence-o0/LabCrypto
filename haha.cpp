#include <iostream>
#include <cmath>
using namespace std;

// Функція для обчислення символу Якобі (a/n)
int jacobiSymbol(int a, int n) {
    if (n <= 0 || n % 2 == 0)
        return 0; // Символ Якобі визначений тільки для непарних n > 0

    int result = 1;
    a = a % n;

    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            int r = n % 8;
            if (r == 3 || r == 5)
                result = -result;
        }
        // Обмін значень a та n
        int temp = a;
        a = n;
        n = temp;

        if (a % 4 == 3 && n % 4 == 3)
            result = -result;

        a = a % n;
    }

    if (n == 1)
        return result;
    else
        return 0; // a та n не є взаємно простими
}

// Функція для перевірки, чи є число n числом Люка з використанням дискримінанту D
bool lucasPseudoprimeTest(int n, int D) {
    int P = 1, Q = (1 - D) / 4; // Початкові значення P і Q

}

int main() {
    int n;
    cout << "Введіть число n: ";
    cin >> n;

    // Знаходимо перше число D, для якого (D/n) = -1
    int D = 5;
    while (jacobiSymbol(D, n) != -1) {
        D++;
    }

    // Виконуємо тест псевдопростоти Люка з використанням дискримінанту D
    if (lucasPseudoprimeTest(n, D)) {
        cout << n << " - дуже ймовірне просте число." << endl;
    } else {
        cout << n << " - складене число." << endl;
    }

    return 0;
}



