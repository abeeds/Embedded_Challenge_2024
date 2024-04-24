#ifndef DFT_H
#define DFT_H

#include <Arduino.h>

class DFT {
private:
    unsigned int N;
    double* input;
    double* dft;
    unsigned int samplingRate;
    const int LOWERBOUND = 3;
    const int UPPERBOUND = 6;

    double* recursiveDFT();
public:
    // Constructor
    DFT(unsigned int size, const double* input, const int samplingRate);

    double percentageInFrequencyRange();

    double getIntensityRange();
};

#endif // DFT_H
