#include <cmath>
#include "fft.h"

const float pi = 3.14159265359;

// Wavelet transform is preformed from a scale factor of 1-SCALEMAX.
const int SCALEMAX = 30;

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


// Wavelet transform function. Currently transforms with fixed Ricker wavelet. Can be easily
// modified to use various other mother wavelet functions.
void waveletTransform (std::vector<float> &rawData, std::vector<std::vector<float> >
        &transformedData) {

    std::vector<float> fftOutput (2*rawData.size(),0);

    // Vector is twice the size of the data set to accommodate padding.
    std::vector<float> rickerVector (2*rawData.size(),0);

    // Vector to hold padded raw spectral data.
    std::vector<float> rawDataPadded (2*rawData.size(),0);

    // Pad spectral data
    padData(rawData,rawDataPadded);

    // Take fft of the padded spectral data.
    dft(rawDataPadded,fftOutput);

    // Calculate the wavelet transform from scales 1-SCALEMAX
    for (int scale=1;scale<SCALEMAX;scale++) {
        std::cout << scale << std::endl;
        rickerArray(rickerVector,scale);
        std::vector<float> fftRickerArray (rawDataPadded.size(),0);
        std::vector<float> output (rawDataPadded.size(),0);
        std::vector<float> transformedOutput (rawDataPadded.size(),0);
        dft(rickerVector,fftRickerArray);
        vectorProduct(fftRickerArray,fftOutput,output);
        idft(output,transformedOutput);
        for (int j=0;j<rawDataPadded.size()-rawData.size()/2;j++) {
            transformedData[scale][j] = transformedOutput[j];
        }
    }
}

