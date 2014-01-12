#include <vector>
#include <cmath>
#include <deque>
//*** Only needed for debugging
#include <iostream>
#include <fstream>
//***
#include "../include/magicNums.h"
#include "../include/ridgeLineTools.h"

// Build deque of ridgeline points
void findRidgePoints (std::deque<ridgePoint> &ridgeCoords, const std::vector<std::vector<float> >
        &waveletSpace) {

    for (int i=1;i<SCALEMAX-1;i++) {
        for (int j=1;j<waveletSpace[i].size()-1;j++) {
            if (waveletSpace[i][j] > waveletSpace[i-1][j-1] &&
                    waveletSpace[i][j] > waveletSpace[i+1][j+1]) {
                        ridgePoint newPoint;
                        newPoint.scale = i;
                        newPoint.col = j;
                        ridgeCoords.push_back(newPoint);
            }
        }
    }
}

// Helper function to check if this ridge point belongs to another line.
bool is_used (std::vector<std::deque<ridgePoint> > &ridgeLines, ridgePoint &currentPoint) {

    for (int i=0;i<ridgeLines.size();i++) {
        for (int j=0;j<ridgeLines[i].size();j++) {
            if (ridgeLines[i][j].scale == currentPoint.scale && ridgeLines[i][j].col ==
                    currentPoint.col) {
                return true;
            }
        }
    }
    return false;
}

// Build ridgelines from the points in ridgeCoords. When we add a point to a ridge line, we
// delete it do that it can't be used by another line.
void findRidgeLines (std::deque<ridgePoint> &ridgeCoords, std::vector<std::deque<ridgePoint> >
        &ridgeLines) {

    int colWindow = 10;
    int scaleWindow = 3;

    // First we extract all of the points that lie at the lowest scale factor. These are the
    // starting points for each line. Once we have use a point for a line it is removed so that
    // it can't be used again.
    for (int i = 0; i<ridgeCoords.size();i++) {
        if (ridgeCoords[i].scale == 1) {
            std::deque<ridgePoint> newRidge;
            newRidge.push_back(ridgeCoords[i]);
            ridgeLines.push_back(newRidge);
        }
    }

    // We now build the ridge points from the starting lines. Checking if each point exists in a
    // ridge line already. This could be sped up greatly by removed points as they are added to
    // each line, so the search becomes smaller and we don't have to check every line in each
    // iteration.
    for (int i=0;i<ridgeLines.size();i++) {
        // Initialise the starting row and column.
        int currentCol = ridgeLines[i][0].col;
        int currentScale = ridgeLines[i][0].scale;
        for (int j=0;j<ridgeCoords.size();j++) {
            int scaleDiff = ridgeCoords[j].scale - currentScale;
            std::cout << scaleDiff << std::endl;
            int colDiff = abs(currentCol - ridgeCoords[j].col);
            if ((scaleDiff > 0 && scaleDiff < scaleWindow) && (colDiff < colWindow)
                    && !is_used(ridgeLines, ridgeCoords[j])) {
                std::cout << currentScale << std::endl;
                ridgeLines[i].push_back(ridgeCoords[j]);
                currentCol = ridgeCoords[j].col;
                currentScale = ridgeCoords[j].scale;
            }
        }
    }
}

void getRidgeLines (std::vector<std::deque<ridgePoint> > &ridgeLines,
         const std::vector<std::vector<float> > &maximaArray) {

    std::deque<ridgePoint> ridgeCoords;

    findRidgePoints(ridgeCoords,maximaArray);

    findRidgeLines(ridgeCoords,ridgeLines);

    // ************** DEBUGGING **************

    // Rebuild filtered ridgeline array for debugging
    std::vector<std::vector<float> > filteredArray(SCALEMAX,
            std::vector<float>(maximaArray[0].size(),0));

    // Write ridge points to file for debugging
    std::ofstream filtered_outputfile ("filteredArray.txt");
    for (int i=0;i<ridgeLines.size();i++) {
        for (int j=0;j<ridgeLines[i].size();j++) {
            filteredArray[ridgeLines[i][j].scale][ridgeLines[i][j].col] = 1;
        }
    }

    // Write maxima array to file
    for (int i=0;i<filteredArray.size();i++) {
        for (int j=0;j<filteredArray[i].size();j++) {
            filtered_outputfile << filteredArray[i][j];
            filtered_outputfile << "\n";
        }
    }

    // ************** DEBUGGING **************
}
