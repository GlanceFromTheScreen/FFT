#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include "FFT.h"
#include "experiment.h"
#include <fstream>

int main() {
    std::vector<int> sizes = {20, 30, 50, 100, 500, 1000, 27000, 100000, 500000, 1000000};
    std::vector<double> answers;

    answers.reserve(sizes.size());
    for(int size : sizes) {
        answers.push_back(experiment(size));
    }

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    for (auto& item: answers) {
        outputFile << item << ", ";
    }

    outputFile.close();
    std::cout << "Data is in 'output.txt'." << std::endl;



    return 0;
}
