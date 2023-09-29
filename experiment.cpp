#include <vector>
#include <complex>
#include <random>
#include "FFT.h"

double experiment(int SIZE) {
    // случайные генераторы
    std::vector<std::complex<double>> my_numbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> real_dist(-10.0, 10.0);
    std::uniform_real_distribution<double> imag_dist(-5.0, 5.0);

    // заполнение выборки случайными значениями
    for (int i = 0; i < SIZE; i++) {
        double real_part = real_dist(gen);
        double imaginary_part = imag_dist(gen);
        std::complex<double> complex_number(real_part, imaginary_part);
        my_numbers.push_back(complex_number);
    }

    // запоминаем начальное состояние
    std::vector<std::complex<double>> before = my_numbers;

    // прямое и обратное преобразование Фурье
    FFT my_fft(SIZE);
    my_fft.forward(my_numbers);
    my_fft.inverse(my_numbers);

    // разница между начальным значением и полученным
    double max = 0;
    for (int i = 0; i < my_numbers.size(); i++) {
        if (std::abs(my_numbers[i] - before[i]) > max) {
            max = std::abs(my_numbers[i] - before[i]);
        }
    }

    return max;
}

