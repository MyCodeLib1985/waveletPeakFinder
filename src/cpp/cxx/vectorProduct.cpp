#include <cmath>
#include <vector>
#include <cassert>

void vectorProduct (std::vector<float> &vector1, std::vector<float> &vector2,
        std::vector<float> &output) {

    // Check vectors are same length.
    assert(vector1.size() == vector2.size());

    // Get product of vectors.
    for (int i=0;i<vector1.size();i++) {
        output[i] = vector1[i]*vector2[i];
    }
}

