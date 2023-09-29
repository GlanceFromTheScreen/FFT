#include <complex>
#include <vector>
#include <cmath>
#include <iostream>
#include <random>

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


class FFT {
public:
    FFT(int N);
    void forward(std::vector<std::complex<double>>& x);
    void inverse(std::vector<std::complex<double>>& X);

private:
    int N;
    void fft(std::vector<std::complex<double>>& x, bool inverse);
    void fft_boost(std::vector<std::complex<double>>& x, bool inverse);
};

FFT::FFT(int N) : N(N) {}

void FFT::forward(std::vector<std::complex<double>>& x) {
    fft_boost(x, false);
}

void FFT::inverse(std::vector<std::complex<double>>& X) {
    fft_boost(X, true);
    for (int i = 0; i < N; i++) {
        X[i] /= N; // Нормализация
    }
}

void FFT::fft(std::vector<std::complex<double>>& x, bool inverse) {
    int N = x.size();
    if (N <= 1) {
        return;
    }

    // init empty vectors
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);

    // fill empty vectors
    for (int i = 0; i < N / 2; i++) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    fft(even, inverse);
    fft(odd, inverse);

    double angle = (inverse ? 2.0 : -2.0) * M_PI / N;
    std::complex<double> w(1);
    std::complex<double> wn(std::cos(angle), std::sin(angle));

    // 2 parts correct concatenation
    for (int i = 0; i < N / 2; i++) {
        std::complex<double> t = w * odd[i];
        x[i] = even[i] + t;
        x[i + N / 2] = even[i] - t;
        w *= wn;
    }
}

void FFT::fft_boost(std::vector<std::complex<double>>& x, bool inverse) {
    int N = x.size();
    if (N <= 1) {
        return;
    }

    int p = primeFactors(N)[0];
    std::vector<std::vector<std::complex<double>>> parts(p);

    for (int i = 0; i < p; i++) {  // заполняю все части
        for (int j = 0; j < N/p; j++) { // заполняю элементы частей
            parts[i].push_back(x[j * p + i]);
        }
    }

    for (auto& item: parts) {
        fft_boost(item, inverse);
    }

    double angle = (inverse ? 2.0 : -2.0) * M_PI;
    std::complex<double> exp1(0, 0);
    std::complex<double> exp2(0, 0);

    for (int k = 0; k < p; k ++) {  // цикл по модулям N / p
        for (int i = 0; i < N/p; i++) {  // заполняем все x[i]
            std::complex<double> summ(0, 0);
            for (int j = 0; j < p; j++) {  // считаем сумму справа от x[i]
                exp1 = std::complex<double>(std::cos(angle * j * i / N), std::sin(angle * j * i / N));
                exp2 = std::complex<double>(std::cos(angle * j * k / p), std::sin(angle * j * k / p));
                summ += parts[j][i] * exp1 * exp2;
            }
            x[i + (N/p) * k] = summ;
        }
    }

}



int main() {
    std::vector<std::complex<double>> my_numbers;
    std::random_device rd; // Генератор случайных чисел на основе аппаратных источников
    std::mt19937 gen(rd()); // Мерсенн-Твист генератор
    std::uniform_real_distribution<double> real_dist(-10.0, 10.0); // Равномерное распределение для действительной части
    std::uniform_real_distribution<double> imag_dist(-5.0, 5.0);   // Равномерное распределение для мнимой части

    int SIZE = 25;
    // Заполнение вектора десятью случайными значениями
    for (int i = 0; i < SIZE; i++) {
        double real_part = real_dist(gen);    // Генерация случайной действительной части
        double imaginary_part = imag_dist(gen); // Генерация случайной мнимой части
        std::complex<double> complex_number(real_part, imaginary_part);
        my_numbers.push_back(complex_number);
    }

    // Вывод значений вектора
    std::cout << "vector values:\n";
    for (const auto& num : my_numbers) {
        std::cout << num << std::endl;
    }

    FFT my_fft(SIZE);
    my_fft.forward(my_numbers);
    my_fft.inverse(my_numbers);

    std::cout << "vector values after FFT -> and <-\n";
    for (const auto& num : my_numbers) {
        std::cout << num << std::endl;
    }

//    std::complex<double> exp1(1, 2);
//    exp1 = std::complex<double>(std::cos(4), 2);
//    std::cout << exp1;

    return 0;
}













