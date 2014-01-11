#include <cmath>
#include "fft.h"

const float pi = 3.14159265359;

// Wavelet transform is preformed from a scale factor of 1-SCALEMAX.
const int SCALEMAX = 30;

void vectorProduct (std::vector<float> &vector1, std::vector<float> &vector2,
        std::vector<float> &output) {

    // Check vectors are same length.
    assert(vector1.size() == vector2.size());

    // Get product of vectors.
    for (int i=0;i<vector1.size();i++) {
        output[i] = vector1[i]*vector2[i];
    }
}

