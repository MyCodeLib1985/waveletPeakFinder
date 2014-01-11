#include <vector>
#include "const.h"
#include "ridgelineTools.h"

// Find local maxima of CWT matrix using a sliding window of size 'windowSize'. Leaving this
// hard coded to 2 is ok, but it is easy enough to include a variable window.
void findMaxima (std::vector<std::vector<float> > &maximaArray, std::vector<std::vector<float> >
        &waveletSpace) {

    for (int scale=1;scale<SCALEMAX-1;scale++) {
        for (int j=1;j<waveletSpace[scale].size()-1;j++) {
            if (waveletSpace[scale][j] > waveletSpace[scale-1][j-1] &&
                    waveletSpace[scale][j] > waveletSpace[scale+1][j+1])
                maximaArray[scale][j] = waveletSpace[scale][j];
        }
    }
}

void buildRidgeLine (std::vector<ridgePoint> &ridgeLine, const std::vector<std::vector<float> >
        &maximaArray) {

    // Define the window sizes in which to search for new ridge points. Keep adding ridge
    // points until we reach the maximum scale value or until no point is found within the
    // window.
    int colWindow = 5;
    int scaleWindow = 3;

    // Get the starting column. We know the starting row value is that of the lowest scale
    // value.
    int currentCol = ridgeLine[0].col;
    int currentScale = ridgeLine[0].scale;

    // Switch to terminate search when we can't find anymore ridge points.
    bool keepSearching = true;

    // We adjust the range of the for loops to prevent running over the edges of the array. By
    // comparing the previous and current scales we can determine if we have found any new
    // points, if not, we can break.
    while (keepSearching) {
        int previousScale = currentScale;
        for (int i=scaleWindow;i<SCALEMAX-scaleWindow;i++) {
            for (int j=colWindow;j<maximaArray[i].size()-colWindow;j++) {
                // If a ridge point exists, add it to the ridge vector.
                if (maximaArray[i][j] != 0) {
                    ridgePoint newPoint;
                    newPoint.scale = i;
                    newPoint.col = j;
                    ridgeLine.push_back(newPoint);
                    // Shift the window in which to look for new points.
                    currentCol = j;
                    currentScale = i;
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
    for (int j=0;j<maximaArray[0].size();j++) {
        if (maximaArray[0][j] != 0) {
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
    for (int line=0;line<ridgeLines.size();line++) {
        buildRidgeLine(ridgeLines[line],maximaArray);
    }
}
