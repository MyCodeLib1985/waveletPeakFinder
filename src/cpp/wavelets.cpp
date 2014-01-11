#include <cmath>

// Populate the vector for the Ricker wavelet at the appropriate scale.
void rickerArray (std::vector<float> &rickerVector, float scale) {

    float sigma = 1;

    // Offset to plot ricker wavelet around 0
    int offset = rickerVector.size()/2;

    for (int i=0;i<rickerVector.size();i++) {
        float expPart = -1*pow((i-offset)/scale,2)/(2*pow(sigma,2));
        rickerVector[i] = 2/((sqrt(3*sigma)) * pow(pi,0.25)) * ((1 -
                (pow((i-offset)/scale,2)/pow(sigma,2))) * exp(expPart));
    }
}
