#include <cmath>

const float pi = 3.14159265359;

// Wavelet transform is preformed from a scale factor of 1-SCALEMAX.
const int SCALEMAX = 30;

std::vector<float> rickerVector (1024,0);

// Populate the vector for the Ricker wavelet at the appropriate scale.
void rickerArray (std::vector<float> &rickerVector, float scale) {

    float sigma = scale;

    // Offset to plot ricker wavelet around 0
    int offset = rickerVector.size()/2;

    for (int i=0;i<rickerVector.size();i++) {
        float expPart = -1*pow((i-offset),2)/(2*pow(sigma,2));
        rickerVector[i] = 2/((sqrt(3*sigma)) * pow(pi,0.25)) * ((1 -
                (pow((i-offset),2)/pow(sigma,2))) * exp(expPart));
    }
}

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

// Wavelet transform function. Currently transforms with fixed Ricker wavelet. Can be easily
// modified to use various other mother wavelet functions.
void waveletTransform (std::vector<float> &rawData, std::vector<std::vector<float> >
        &transformedData) {

    // Calculate the wavelet transform from scales 1-SCALEMAX
    for (int scale=1;scale<SCALEMAX;scale++) {
        std::cout << scale << std::endl;
        rickerArray(rickerVector,scale);
        convolution(rawData,rickerVector,transformedData, scale);
    }
}
