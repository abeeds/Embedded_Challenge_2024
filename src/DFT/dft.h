#ifndef DFT_H
#define DFT_H

class DFT {
private:
    int N;
    double input[300];
    double dft[300];
    double dftr[300];
    double dfti[300];
    unsigned int samplingRate;
    int LOWERBOUND = 3;
    int UPPERBOUND = 8;

    void recursiveDFT();
public:
    // Constructor
    DFT(const int size, volatile double* input, int samplingRate);

    double percentageInFrequencyRange();

    double getIntensityRange();

    void plotData();
};

#endif // DFT_H
