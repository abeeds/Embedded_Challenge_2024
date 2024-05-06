#include "DFT.h"
#include <Arduino.h>

DFT::DFT(const int size, volatile double* inputArray, int samplingRate) {
    N = size;
    for (int s = 0; s < N; ++s) {
        input[s] = inputArray[s];
    }
    recursiveDFT();
}

void DFT::recursiveDFT() {
    for (int k = 0; k < N; ++k) {
        dftr[k] = 0;
        dfti[k] = 0;
        for (int j = 0; j < N; ++j) {
            double theta = 2 * M_PI * j * k / N;
            dftr[k] += input[j] * cos(theta);
            dfti[k] -= input[j] * sin(theta);
        }
        dft[k] = sqrt(sq(dftr[k]) + sq(dfti[k])) * samplingRate / N;
    }
    // for (unsigned int i = 0; i < N; ++i) {
    //     Serial.print(">FFT:");
    //     Serial.println(dft[i]);
    // }
    // Serial.println();
}

double DFT::percentageInFrequencyRange() {
    unsigned int count = 0;
    for (int k = 0; k < N; ++k) {
        double frequency = dft[k];
        if (frequency >= LOWERBOUND && frequency <= UPPERBOUND) {
            ++count;
        }
    }
    double percentage = 100.0 * count / N;
    Serial.println(percentage);
    return percentage;
}


double DFT::getIntensityRange() {
    double intensity = 0;
    double total_intensity = 0;
    for (int k = 0; k < N; ++k) {
        double frequency = dft[k];
        if (frequency >= LOWERBOUND && frequency <= UPPERBOUND) {
            intensity += abs(dft[k]);
        }
        total_intensity += abs(dft[k]);
    }
    return intensity / total_intensity * 100;
}

// Plot the FFT data using teleplot
void DFT::plotData() {
    // for (unsigned int k = 0; k < N; ++k) {
    //     Serial.print(">FFT");
    //     Serial.println(dft[k]);
    // }
}
