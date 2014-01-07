#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include "waveletTransform.h"
#include "cwtArrayTools.h"
#include "ridgelineTools.h"

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
        for (int j=0;j<waveletSpace[i].size();j++) {
            outputFile << waveletSpace[i][j];
            outputFile << "\n";
        }
    }

    // Create an array of the local maxima of the ridge lines. We search for local maxima in a
    // sliding window with width 3 and save these maxima in a binary array, set to 1 for the
    // presence of a maxima and 0 otherwise. This is the array that will be used to build the
    // ridge lines.

    std::vector<std::vector<float> > maximaArray(SCALEMAX,
            std::vector<float>(rawData_intensities.size(),0));

    findMaxima(maximaArray,waveletSpace);

    // write the wavelet transform matrix to file for plotting/debugging.
    std::ofstream maxima_outputfile ("maximamatrix.txt");
    for (int i=0;i<maximaArray.size();i++) {
        for (int j=0;j<maximaArray[i].size();j++) {
            maxima_outputfile << maximaArray[i][j];
            maxima_outputfile << "\n";
        }
    }

    // Now that we have the maxima of the ridge lines, we can filter the lines to determine the
    // positions of the peaks. Ridge lines are stored as a vector of structs containing the
    // scale factor and the wavenumber of the point.

    // Array of objects of type ridgePoint to hold ridge lines.
    std::vector<std::vector<ridgePoint> > ridgeLines;

    // Extract the ridge lines from the maxima array.
    getRidgeLines(ridgeLines,maximaArray);

    // Rebuild filtered ridgeline array for debugging
    std::vector<std::vector<float> > filteredArray(SCALEMAX,
            std::vector<float>(rawData_intensities.size(),0));

    // write the wavelet transform matrix to file for plotting/debugging.
    std::ofstream filtered_outputfile ("Filteredmaximamatrix.txt");
    for (int i=0;i<filteredArray.size();i++) {
        for (int j=0;j<filteredArray[i].size();j++) {
            filtered_outputfile << filteredArray[i][j];
            filtered_outputfile << "\n";
        }
    }

    // Plot wavelet to file for debugging.
    std::ofstream ricker_outputfile ("ricker.txt");
    for (int i=0;i<rickerVector.size();i++) {
        ricker_outputfile << rickerVector[i];
        ricker_outputfile << "\n";
    }
    return 0;

}
