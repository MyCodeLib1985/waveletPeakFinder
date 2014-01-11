#include <cmath>
#include <vector>
#include <wavelets.h>
#include <fft.h>
#include <vectorProduct.h>

void waveletTransform (std::vector<float> &rawData,
        std::vector<std::vector<float> > &transformedData) {

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
