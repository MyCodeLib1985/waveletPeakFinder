#ifndef _INCLUDE_RIDGELINETOOLS_
#define _INCLUDE_RIDGELINETOOLS_

struct ridgePoint {

    int col;
    int scale;
    int CWTCoeff;

};

struct peakInfo {

    int scaleMax;
    int center;
    int ridgeLength;

};

void extractPeakInfo (const std::vector<std::vector<float> > &maximaArray,
        std::vector<peakInfo> &peaksFound);

#endif
