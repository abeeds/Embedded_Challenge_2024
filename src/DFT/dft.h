#ifndef DFT_H
#define DFT_H

class DFT {
private:
    int N;
    double input[60];
    double dft[60];
    unsigned int samplingRate;
    int LOWERBOUND = 3;
    int UPPERBOUND = 6;

    double* recursiveDFT();
public:
    // Constructor
    DFT(int size, double* input, int samplingRate);

    double percentageInFrequencyRange();

    double getIntensityRange();

    void plotData();
};

#endif // DFT_H
