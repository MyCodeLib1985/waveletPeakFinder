#include <cmath>
#include "fft.h"

const float pi = 3.14159265359;

// Wavelet transform is preformed from a scale factor of 1-SCALEMAX.
const int SCALEMAX = 30;

// Calculate the value of the wavelet transform at (trans,scale).
void convolution (std::vector<float> &rawData, const std::vector<float> &rickerVector,
        std::vector<std::vector<float> > &transformedData, float scale) {

    // Calculate the convolution of the wavelet function and the data set at the current scale
    // value.
    for (int i=0; i<rawData.size();i++) {
        for (int j=1; j<rickerVector.size();j++) {
            if ((i-j) >=0 && (i-j) <= rawData.size()) {
                transformedData[scale][i] += rawData[i-j] * rickerVector[j];
            }
        }
    }
}

void vectorProduct (std::vector<float> &vector1, std::vector<float> &vector2,
        std::vector<float> &output) {

    // Check vectors are same length.
    assert(vector1.size() == vector2.size());

    // Get product of vectors.
    for (int i=0;i<vector1.size();i++) {
        output[i] = vector1[i]*vector2[i];
    }
}

