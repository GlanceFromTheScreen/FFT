cmake_minimum_required(VERSION 3.21)
project(FFT)

set(CMAKE_CXX_STANDARD 17)

add_executable(FFT main.cpp FFT.h FFT.cpp primeFactors.cpp primeFactors.h experiment.cpp experiment.h)
