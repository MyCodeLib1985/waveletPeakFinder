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

