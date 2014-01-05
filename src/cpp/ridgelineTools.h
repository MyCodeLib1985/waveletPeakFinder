// Filter valid ridge lines from maxima found in 2D wavelet coefficient array.

struct ridgePoint {

        float scale;
        int col;
        int lineID;

    };

// Helper function to determine whether we have used this point already. Points can only belong
// to one ridgeline.
bool isNewPoint (std::vector<ridgePoint> &ridgeLines, float currentScale, int currentCol) {

    // For the current ridge point, check to see if there is a ridge point close enough to
    // justify continuing the line.
    for (int i=0;i<ridgeLines.size();i++) {
        if (ridgeLines[i].scale == currentScale & ridgeLines[i].col == currentCol) {
            return false;
        }
    }

    return true;

}

// Helper function to check if new ridge line points belong to any nearby ridge lines. This is
// done by checking that there is a point within 'scaleWindow' and 'colWindow' of the point.
// These values are currently hard coded, but can be made variable as the search broadens at
// higher scale factors.
bool isCloseEnough (std::vector<ridgePoint> &ridgeLines, float currentScale, float currentCol,
        int &lineID) {

    for (int i=0;i<ridgeLines.size();i++) {
        if (fabs(ridgeLines[i].scale - currentScale) < 3 && fabs(ridgeLines[i].col -
                    currentCol) < 5) {
            lineID = i;
            return true;
        }
    }

    return false;

}

// Helper function to remove invalid rigde points.
void cleanPoints (std::vector<ridgePoint> &ridgeLines, int lineID) {

    for (int i=0;i<ridgeLines.size();i++) {
        if (ridgeLines[i].lineID == lineID) {
            ridgeLines.erase (ridgeLines.begin() + i);
        }
    }
}

// Filter out valid ridgelines
void getRidgeLines (std::vector<ridgePoint> &ridgeLines, std::vector<std::vector<float> >
    &maximaArray) {

    for (int scale=0;scale<SCALEMAX;scale++) {
        for (int j=0;j<maximaArray[scale].size();j++) {
            // For each maxima...
            if (maximaArray[scale][j] != 0) {
                // For the first scale value, we want to take every point as the start of a
                // ridge line.
                if (scale == 0) {
                        ridgePoint point;
                        point.scale = scale;
                        point.col = j;
                        point.lineID = j;
                        ridgeLines.push_back(point);
                }
                // For each subsequent maximum, if this point is not currently used in a ridge
                // line and if it is close enough to a previous point to be considered part of
                // the same line, add it to the points vector.
                if (isNewPoint(ridgeLines, scale, j)) {
                    int lineID = 0;
                    if (isCloseEnough(ridgeLines, scale, j, lineID)) {
                        ridgePoint point;
                        point.scale = scale;
                        point.col = j;
                        point.lineID = lineID;
                        ridgeLines.push_back(point);

                    }
                }
            }
        }
    }
}
