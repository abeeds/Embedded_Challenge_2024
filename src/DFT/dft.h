#ifndef DFT_H
#define DFT_H

class DFT {
private:
    int N;
    double* input;
    double* dft;
    unsigned int samplingRate;
    int LOWERBOUND = 3;
    int UPPERBOUND = 6;

    double* recursiveDFT();
public:
    // Constructor
    DFT(int size, double* input, int samplingRate);

    double percentageInFrequencyRange();

    double getIntensityRange();
};

#endif // DFT_H
