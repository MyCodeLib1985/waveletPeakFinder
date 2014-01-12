// Standard includes.
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>

// Project includes.
#include "../include/magicNums.h"
#include "../include/CWT.h"
#include "../include/ridgelineTools.h"

int main(int argc, char** argv) {

    std::vector<float> rawData_intensities;
    std::vector<float> rawData_wavenumbers;

    // Check if we have the address of an input file
    if (argc < 2) {
        std::cout << "Please enter an input file" << std::endl;
    }

    // Read Raman data file, expects two tab delimited columns of wavenumbers and intensities.
    std::ifstream inFile(argv[1]);

    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);

        std::string row;

        int count = 0;

        while(std::getline(iss,row,'\t')) {

            std::istringstream iss2(row);

            if (count == 0) {
                rawData_wavenumbers.push_back(atof(row.c_str()));
            }
            if (count == 1) {
                rawData_intensities.push_back(atof(row.c_str()));
            }
            count += 1;
        }

    }

    // Create a (SCALEMAX,len(intensities) 2D array to hold the resultant data from the wavelet
    // transform.
    std::vector<std::vector<float> >
        waveletSpace(SCALEMAX,std::vector<float>(rawData_intensities.size(),0));

    // Take the wavelet transform of the data.
    waveletTransform(rawData_intensities, waveletSpace);

    // Write the wavelet transform matrix to file for plotting/debugging. These are not really
    // need except for illustration purposes.
    std::ofstream outputFile ("waveletMatrix.txt");
    for (int i=0;i<waveletSpace.size();i++) {
        for (int j=waveletSpace[i].size();j>0;j--) {
            outputFile << waveletSpace[i][j];
            outputFile << "\n";
        }
    }

    // Now that we have the maxima of the ridge lines, we can filter the lines to determine the
    // positions of the peaks. Ridge lines are stored as a vector of structs containing the
    // scale factor and the wavenumber of the point.

    // Array of objects of type ridgePoint to hold ridge lines.
    std::vector<std::deque<ridgePoint> > ridgeLines;

    // Extract the ridge lines from the maxima array.
    getRidgeLines(ridgeLines,waveletSpace);

    return 0;
}
