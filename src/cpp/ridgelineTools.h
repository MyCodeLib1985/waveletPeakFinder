// Filter valid ridge lines from maxima found in 2D wavelet coefficient array.

// Helper function to determine whether we have used this point already. Points can only belong
// to one ridgeline.
bool newPoint (std::vector<ridgePoint> &ridgeLines, float currentScale, int currentCol) {

    for (int i=0;i<ridgeLines.size();i++) {
        if (ridgeLines[i].scale == currentScale & ridgeLines.col == currentCol) {
            return true;
        }
    }

    return false;

}

void filterRidgeLines (std::vector<ridgePoint> &ridgeLines) {

    for (int i=0;i<maximaArray.size();i++) {
        for (int j=0;j<maximaArray[i].size();j++) {

        }
    }

}
