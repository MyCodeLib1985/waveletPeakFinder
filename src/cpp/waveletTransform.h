#include <cmath>

const float pi = 3.14159265359;

const int SCALEMAX = 20;

float ricker (float t, float sigma) {

        float wavelet = 2/(sqrt(3*sigma) * pow(pi,0.25)) * (1 - (pow(t,2)/pow(sigma,2))
                * exp((-1*pow(t,2)/(2*pow(sigma,2)))));

        return wavelet;
}

float CWT (std::vector<float> &rawData, float scale, float trans, float t) {

    // Calculate the integral of the mother wavelet function over all space.

    float numericalIntegration = 0;

    for (int i=0; i<rawData.size()-1;i++) {
        numericalIntegration += ((ricker(rawData[i]-i/scale,1) + ricker(rawData[i+1]-i/scale,1))/2);
    }

    return 1/sqrt(fabs(scale)) * (t * numericalIntegration);
}

// Wavelet transform function. Currently transforms with fixed Ricker wavelet.
void waveletTransform (std::vector<float> &rawData, std::vector<std::vector<float> >
        &transformedData) {

    // Calculate the wavelet transform from scales 1-SCALEMAX
    for (int scale=1;scale<SCALEMAX;scale++) {
        std::cout << scale << std::endl;
        for (int j=0;j<rawData.size();j++) {
            transformedData[scale][j] = CWT(rawData,scale,j,rawData[j]);
        }
    }
}
