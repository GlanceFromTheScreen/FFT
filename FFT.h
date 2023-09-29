#ifndef FFT_FFT_H
#define FFT_FFT_H

#include <complex>
#include <vector>

class FFT {
public:
    FFT(int N);
    void forward(std::vector<std::complex<double>>& x);
    void inverse(std::vector<std::complex<double>>& X);

private:
    int N;
    void fft(std::vector<std::complex<double>>& x, bool inverse);
    void fft_multiple(std::vector<std::complex<double>>& x, bool inverse);
};

#endif //FFT_FFT_H
