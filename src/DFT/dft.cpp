#include "DFT.h"
#include <Arduino.h>

DFT::DFT(unsigned int size, const double* inputArray, const int samplingRate) {
    N = size;
    input = new double[N];
    dft = new double[N];
    for (unsigned int i = 0; i < N; ++i) {
        input[i] = inputArray[i];
    }
    recursiveDFT();
}

double* DFT::recursiveDFT() {
    for (unsigned int k = 0; k < N; ++k) {
        dft[k] = 0;
        for (unsigned int n = 0; n < N; ++n) {
            double theta = 2 * M_PI * n * k / N;
            dft[k] += input[n] * cos(theta);
        }
        dft[k] *= samplingRate / N;
    }
    return dft;
}

double DFT::percentageInFrequencyRange() {
    unsigned int count = 0;
    for (unsigned int k = 0; k < N; ++k) {
        double frequency = dft[k];
        if (frequency >= LOWERBOUND && frequency <= UPPERBOUND) {
            ++count;
        }
    }
    double percentage = 100.0 * count / N;
    return percentage;
}


double DFT::getIntensityRange() {
    double intensity = 0;
    double total_intensity = 0;
    for (unsigned int k = 0; k < N; ++k) {
        double frequency = dft[k];
        if (frequency >= LOWERBOUND && frequency <= UPPERBOUND) {
            intensity += abs(dft[k]);
        }
        total_intensity += abs(dft[k]);
    }
    return intensity / total_intensity * 100;
}