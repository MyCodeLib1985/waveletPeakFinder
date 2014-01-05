#include <cmath>

const float pi = 3.14159265359;

// Wavelet transform is preformed from a scale factor of 1-SCALEMAX.
const int SCALEMAX = 60;

// Ricker wavelet
float ricker (float t) {

    // Fixed standard deviation of ricker wavelet function.
    const float sigma = 1;

    // Calculate the exponential part of the Ricker wavelet.
    float expPart = -1*pow(t,2)/(2*pow(sigma,2));

    float wavelet = 2/((sqrt(3*sigma)) * pow(pi,0.25)) * (1 - (pow(t,2)/pow(sigma,2)
            * exp(expPart)));

    return wavelet;
}

// Calculate the value of the wavelet transform at (trans,scale).
float CWT (std::vector<float> &rawData, float scale, float t) {

    // Calculate the integral of the mother wavelet function over all space.

    float convolution = 0;

    for (int i=0; i<rawData.size()-1;i++) {
        convolution += (rawData[i]*ricker((rawData[i]-t)/scale));
    }

    return (1/sqrt(fabs(scale))) * convolution;
}

// Wavelet transform function. Currently transforms with fixed Ricker wavelet. Can be easily
// modified to use various other mother wavelet functions.
void waveletTransform (std::vector<float> &rawData, std::vector<std::vector<float> >
        &transformedData) {

    // Calculate the wavelet transform from scales 1-SCALEMAX
    for (int scale=1;scale<SCALEMAX;scale++) {
        std::cout << scale << std::endl;
        for (int j=0;j<rawData.size();j++) {
            transformedData[scale][j] = CWT(rawData,scale,rawData[j]);
        }
    }
}
