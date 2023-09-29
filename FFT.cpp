#include "FFT.h"
#include "primeFactors.h"
#include <cmath>

FFT::FFT(int N) : N(N) {}

void FFT::forward(std::vector<std::complex<double>>& x) {
    fft_multiple(x, false);
}

void FFT::inverse(std::vector<std::complex<double>>& X) {
    fft_multiple(X, true);
    for (int i = 0; i < N; i++) {
        X[i] /= N;
    }
}

// быстрое преобразование Фурье для входных данных размера 2^n
void FFT::fft(std::vector<std::complex<double>>& x, bool inverse) {
    int N = x.size();
    if (N <= 1) {
        return;
    }

    // инициализируем пустые вектора
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);

    // заполняем вектора
    for (int i = 0; i < N / 2; i++) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    fft(even, inverse);
    fft(odd, inverse);

    double angle = (inverse ? 2.0 : -2.0) * M_PI / N;
    std::complex<double> w(1);
    std::complex<double> wn(std::cos(angle), std::sin(angle));

    // две части соединяются в одну
    for (int i = 0; i < N / 2; i++) {
        std::complex<double> t = w * odd[i];
        x[i] = even[i] + t;
        x[i + N / 2] = even[i] - t;
        w *= wn;
    }
}

// быстрое преобразование Фурье для входных данных любого размера
void FFT::fft_multiple(std::vector<std::complex<double>>& x, bool inverse) {
    int N = x.size();
    if (N <= 1) {
        return;
    }

    int p = primeFactors(N)[0];
    std::vector<std::vector<std::complex<double>>> parts(p);

    for (int i = 0; i < p; i++) {  // делю не на 2 части, а на "p" частей
        for (int j = 0; j < N/p; j++) { // каждая часть заполняется элементами (индекс по модулю "p")
            parts[i].push_back(x[j * p + i]);
        }
    }

    // рекурсивный вызов fft_multiple для каждой из частей
    for (auto& item: parts) {
        fft_multiple(item, inverse);
    }

    // вспомогательные переменные для коэффициентов, с которыми будут объединяться части
    double angle = (inverse ? 2.0 : -2.0) * M_PI;
    std::complex<double> exp1(0, 0);
    std::complex<double> exp2(0, 0);

    // конкатенация
    for (int k = 0; k < p; k ++) {  // цикл по модулям (N / p)
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


