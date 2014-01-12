#include <vector>
#include <cmath>
#include <deque>
//*** Only needed for debugging
#include <iostream>
//***
#include "../include/magicNums.h"
#include "../include/ridgeLineTools.h"

// Build deque of ridgeline points
void findRidgePoints (std::deque<ridgePoint> &ridgeCoords, std::vector<std::vector<float> >
        &waveletSpace) {

    for (int i=1;i<SCALEMAX-1;i++) {
        for (int j=1;j<waveletSpace[i].size()-1;j++) {
            if (waveletSpace[i][j] > waveletSpace[i-1][j-1] &&
                    waveletSpace[i][j] > waveletSpace[i+1][j+1]) {
                        ridgePoint newPoint;
                        newPoint.scale = 0;
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
            int colDiff = abs(ridgeCoords[j].col - currentCol);
            if (scaleDiff > 0 && scaleDiff < scaleWindow && colDiff < colWindow) {
                ridgeLines[i].push_back(ridgeCoords[i]);
                currentCol = ridgeCoords[i].col;
                currentScale = ridgeCoords[i].scale;
            }
        }
    }
}

void findMaxima (std::vector<std::vector<float> > &maximaArray, std::vector<std::vector<float> >
        &waveletSpace) {

    for (int i=1;i<SCALEMAX-1;i++) {
        for (int j=1;j<waveletSpace[i].size()-1;j++) {
            if (waveletSpace[i][j] > waveletSpace[i-1][j-1] &&
                    waveletSpace[i][j] > waveletSpace[i+1][j+1]) {
                        //maximaArray[i][j] = waveletSpace[i][j];
                        maximaArray[i][j] = 1;
            }
        }
    }
}

void buildRidgeLine (std::vector<ridgePoint> &ridgeLine, const
        std::vector<std::vector<float> > &maximaArray) {

    // Define the window sizes in which to search for new ridge points. Keep adding ridge
    // points until we reach the maximum scale value or until no point is found within the
    // window.
    int colWindow = 10;
    int scaleWindow = 5;

    // Get the starting column. We know the starting row value is that of the lowest scale
    // value.
    int currentCol = ridgeLine[0].col;
    int currentScale = ridgeLine[0].scale;

    // Switch to terminate search when we can't find anymore ridge points.
    bool keepSearching = true;

    while (keepSearching) {
        int previousScale = currentScale;
        for (int i=currentScale;i<currentScale+scaleWindow;i++) {
            std::cout << i << std::endl;
            for (int j=currentCol-colWindow;j<currentCol+colWindow;j++) {
                if (i > 0 && i < SCALEMAX && j > 0 && j < maximaArray.size()) {
                    // If a ridge point exists, add it to the ridge vector.
                    if (maximaArray[i][j] != 0) {
                        ridgePoint newPoint;
                        newPoint.scale = i;
                        newPoint.col = j;
                        ridgeLine.push_back(newPoint);
                        // Shift the window in which to look for new points.
                        currentScale = i;
                        currentCol = j;
                    }
                }
            }
        }
        // If we've not found a point and moved onto another scale value, terminate the search.
        if (previousScale == currentScale) {
            keepSearching = false;
        }
    }
}

void getRidgeLines (std::vector<std::vector<ridgePoint> > &ridgeLines,
         const std::vector<std::vector<float> > &maximaArray) {

    // Get the coordinates of all the points at the lowest scale value. All valid ridge lines
    // should terminate at the lowest scale value.
    for (int j=0;j<maximaArray[1].size();j++) {
        if (maximaArray[1][j] != 0) {
            // Start a new vector for each point.
            std::vector<ridgePoint> newRidge;
            ridgePoint newPoint;
            newPoint.scale = 0;
            newPoint.col = j;
            newRidge.push_back(newPoint);
            ridgeLines.push_back(newRidge);
            }
        }

    // Now that we have the starting points of all the ridge lines, we want to trace each
    // ridge line, adding all of the points found to line vector.
    for (int i=0;i<ridgeLines.size();i++) {
        buildRidgeLine(ridgeLines[i],maximaArray);
    }
}
