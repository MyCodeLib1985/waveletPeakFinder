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
bool isCloseEnough (std::vector<ridgePoint> &ridgeLines, float currentScale, float currentCol) {

    for (int i=0;i<ridgeLines.size();i++) {
        if (fabs(ridgeLines[i].scale - currentScale) < 3 && fabs(ridgeLines[i].col -
                    currentCol) < 5) {
            return true;
        }
    }

    return false;

}

// Filter out valid ridgelines
void getRidgeLines (std::vector<ridgePoint> &ridgeLines, std::vector<std::vector<float> >
    &maximaArray) {

    for (int scale=0;scale<SCALEMAX;scale++) {
        for (int j=0;j<maximaArray[scale].size();j++) {
            // Check if this point already belongs to a ridge line.
            if (isNewPoint(ridgeLines, scale, j)) {
                if (isCloseEnough(ridgeLines, scale, j)) {
                    ridgePoint point;
                    point.scale = scale;
                    point.col = j;
                    ridgeLines.push_back(point);
                }
            }
        }
    }

}
