#include "primeFactors.h"

//  разложение числа на простые множители
std::vector<int> primeFactors(int n) {
    std::vector<int> factors;

    while (n % 2 == 0) {
        factors.push_back(2);
        n = n / 2;
    }

    for (int i = 3; i * i <= n; i = i + 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n = n / i;
        }
    }

    if (n > 1) { factors.push_back(n); }

    return factors;
}
