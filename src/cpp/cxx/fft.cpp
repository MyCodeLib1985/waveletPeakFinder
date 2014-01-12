#include <vector>
#include <cmath>
#include "../include/magicNums.h"
#include "../include/fft.h"

void dft(std::vector<float> &inputData, std::vector<float> &outputData) {

    for (int i=0;i<inputData.size();i++) {
        float temp = 0;
        for (int j=0;j<inputData.size();j++) {
            float trigterm = 2*pi * i * j/inputData.size();
            temp += inputData[j]*cos(trigterm);
        }
        outputData[i] = temp;
    }
}

void idft (std::vector<float> &inputData, std::vector<float> &outputData) {

    // don't bother calculating complex parts.
    float norm = 1.0/inputData.size();

    for (int i=0;i<inputData.size();i++) {
        float temp = 0;
        for (int j=0;j<inputData.size();j++) {
            float trigterm = 2*pi * i * j/inputData.size();
            temp += norm * inputData[j]*cos(trigterm);
        }
        outputData[i] = temp;
    }
}

void padData(std::vector<float> &input,std::vector<float> &output) {

    // Check if data length is an even number.
    if (input.size() % 2 != 0) {
        input.push_back(0);
    }

    // Pad zeros on either side of data set to 2N
    for (int i=0;i<input.size()*2;i++) {
        if (i > input.size()/4 || i < input.size()/2 + input.size()/4) {
            output[i] = input[i];
        }
    }
}
